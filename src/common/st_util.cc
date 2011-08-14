// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_util.cc

#include <cctype> // isxdigit()
#include <cstring>
#include <fstream>
#include "common/event.h"
#include "common/st_util.h"
#include "common/st_net.h"
#include "common/st_inst.h"
#include "common/sym_tab.h"
#include "expr/rangeid.h"
#include "expr/eeval.h"
#include "vbs.h"

// Utility functions for st_task/st_function's evaluate and setup
// and trigger functions.

void
print_escapecode(const vbs_error::str_type &str, size_t &idx)
	{
	vbs_error::strstream_type buf;
	int i = 0;
	switch(str[idx])
		{
		case 'n': cout << endl; break;
		case 't': cout << '\t'; break;
		case '\\': cout << '\\'; break;
		case '\"': cout << '\"'; break;
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
			while (str[idx] >= '0' && str[idx] <= '7')
				{
				buf << str[idx];
				++i;
				// Only 1-3 octal digits.
				if (i > 2)
					break;
				++idx;
				}
			// Now, convert the octal number to the equivalent
			// integer.  Then print out the ASCII value.
			cout << (char) strtol(buf.str().c_str(), 0, 8);
			break;
		default: cout << str[idx]; break;
		}
	}

bool
find_format(size_t &j, const vbs_error::str_type &fmt,
		vbs_error::str_type &str, vbs_error::value_type &e)
	{
	// Extract the format string from this format.
	for (; j < fmt.size(); ++j)
		{
		if (fmt[j] == '\"')
			break;
		else if (fmt[j] == '%')
			{
			// Explicit percent sign, ignore...
			str += fmt[j];
			return true;
			}
		else if (fmt[j] >= '0' && fmt[j] <= '9')
			{
			// Size specifier, just save.
			str += fmt[j];
			}
		else if (fmt[j] == 'd' || fmt[j] == 'h'
				|| fmt[j] == 'b' || fmt[j] == 'o'
				|| fmt[j] == 's' || fmt[j] == 't'
				|| fmt[j] == 'T' || fmt[j] == 'm')
			{
			// Found the end.  These are our supported types.
			str += fmt[j];
			return true;
			}
		else if (fmt[j] == ' ')
			{
			e = vbs_error::SE_FMTSPEC;
			return false;
			}
		else
			{
			str += fmt[j];
			e = vbs_error::SE_FMTSYM;
			return false;
			}
		}
	e = vbs_error::SE_FMTNOTERM;
	return false;
	}

bool 
check_arguments(std::list<hash_value> *tn, std::list< basic_ptr<expr_base> > *t)
	{
	// Check to make sure that the arguments exist if arguments are
	// taken by the function or task.
	// Return true if ok, false if something is wrong.
	if (tn == 0)
		{
		if (t != 0)
			return false;
		return true;
		}
	else
		{
		if (t == 0)
			return false;
		return tn->size() == t->size();
		}
	}

vbs_error::value_type
check_format(const vbs_error::str_type &fmt)
	{
	for (size_t i = 1; i < fmt.size(); ++i)
		{
		switch(fmt[i])
			{
			case '%':
				{
				vbs_error::value_type tmp;
				vbs_error::str_type s;
				i++;	// Move to next character.
				if (!find_format(i, fmt, s, tmp))
					return tmp;
				break;
				}
			case '\\':
				break;
			default:
				break;
			}
		}
	return vbs_error::SE_NONE;
	}

void
entry_iovars(std::list<hash_value> &localvars, std::list< basic_ptr<expr_base> > &arg)
	{
	// Go through the list and initialize all input variables.
	symbol_table &symboltable = vbs_engine::symboltable();
	st_net::io_list::iterator idx(localvars.begin());
	st_net::io_list::iterator last(localvars.end());
	st_net::arg_list::iterator exp(arg.begin());
	st_net *node;
	for (; idx != last; ++idx, ++exp)
		{
		// Can not be null, since we created this node.
		node = symboltable.get(*idx)->get_net();
		// We do not use assignment here because this is not
		// an assignment.  We're just updating the local variables.
		if (node->_iodirection == st_net::IN
		 || node->_iodirection == st_net::INOUT)
			*node->_storage = (*exp)->evaluate(evaluate_expr());
		}
	}

void
exit_iovars(std::list<hash_value> &localvars, std::list< basic_ptr<expr_base> > &arg)
	{
	// Go through the list and set all output variables.
	symbol_table &symboltable = vbs_engine::symboltable();
	st_net::io_list::iterator idx(localvars.begin());
	st_net::io_list::iterator last(localvars.end());
	st_net::arg_list::iterator exp(arg.begin());
	st_net *them, *us;
	for (; idx != last; ++idx, ++exp)
		{
		// Can not be null, since we created this node.
		us = symboltable.get(*idx)->get_net();
		if (us->_iodirection == st_net::OUT
		 || us->_iodirection == st_net::INOUT)
			{
			range_id *rid = (*exp)->get_range_id();
			if (rid != 0)
				{
				range_id::position_type ms, ls;
				rid->get_select(ms, ls);
				them = symboltable.get(rid->index())->get_net();
				// <ms>/<ls> are not used here because it's
				// upto <them> to select which bits to keep.
				// We just send them everything we have.
				them->assignment(*us->_storage, -1, -1, ms, ls);
				}
			}
		}
	}

void
check_monitor(st_net::monitor_list &el,
		st_net::num_type &before, st_net::num_type &after)
	{
	event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
	event_base<stmt_base> *ev = 0;
	st_net::monitor_list::iterator itp(el.begin());
	st_net::monitor_list::iterator stop(el.end());
	for (; itp != stop; ++itp)
		{
		bool queued = false;
		ev = (*itp).get();
		if (!ev->is_active())
			{
			// This event monitor is idle...
			if (ev->monitor_bit() != 0)
				{
				// Keep track of previous value.
				const event_base<stmt_base>::bit_type *bt = ev->monitor_bit();
				const st_net::num_type &cbit(bt->evaluate(evaluate_expr()));
				ev->previous_bit(cbit);
				}
			continue;
			}
		if (ev->monitor_bit() != 0)
			{
			// We only care about a single bit.
			const event_base<stmt_base>::bit_type *bt = ev->monitor_bit();
			const st_net::num_type &cbit(bt->evaluate(evaluate_expr()));
			const st_net::num_type &pbit(ev->previous_bit());
			st_net::num_type::logic_type cb, pb;
			cb = cbit(cbit.start_pos());
			pb = pbit(pbit.start_pos());
			// Determine whether to queue event.
			if (!ev->is_queued() && pb != cb)
				{
				// Not queued, queue it if the type is correct.
				event_base<stmt_base>::trig_type t =
					TRIG_CHNG | TRIG_MONITOR | TRIG_STROBE;
				if (edge_detect(pb, cb) == NEGEDGE)
					t = t | TRIG_NEGEDGE;
				else if (edge_detect(pb, cb) == POSEDGE)
					t = t | TRIG_POSEDGE;
				if ((ev->type() & t) != TRIG_NONE)
					{
					eventqueue.add_event(ev);
					queued = true;
					}
				}
			ev->previous_bit(cbit);
			}
		else if (compare_c(before, after) == LO)
			{
			// Determine whether to queue event.
			if (!ev->is_queued())
				{
				event_base<stmt_base>::trig_type t =
					TRIG_CHNG | TRIG_MONITOR | TRIG_STROBE;
				if ((ev->type() & t) != TRIG_NONE)
					{
					eventqueue.add_event(ev);
					queued = true;
					}
				}
			}
		// Else, this variable didn't change.  Take care of post-
		// queuing procedure.
		if (queued)
			{
			// This event is being processed, mark as in-active.
			ev->deactivate();
			}
		}
	}

static int
readmem_data(std::ifstream &ifs, int base, st_net_assign::num_type &num)
	{
	typedef st_net_assign::num_type num_type;

	// Read one token at a time.
	const int line_length_max = 1024;
	char s[line_length_max];
	while (!ifs.fail() && !ifs.eof())
		{
		ifs >> s;
		if (s[0] == '/' && s[1] == '/')
			{
			// Discard the rest of this line.
			ifs.getline(s, line_length_max);
			}
		else
			break;
		}
	if (ifs.fail() || ifs.eof())
		return -1;

	// Validate input.
	int size = strlen(s);
	int numdig = 0;
	for (int i = 0; i < size; ++i)
		{
		switch (s[i])
			{
			case 'x': case 'X': case 'z': case 'Z':
				numdig += 1;
				break;
			case '_':
				break;
			case '@':
				// If an address, indicate it to caller.
				if (i != 0)
					return -2;
				num = num_type(&s[1], num_type::UNSIGNED, num_type::BASE16);
				return 1;
			default:
				if (base == 16)
					{
					if (!isxdigit(s[i]))
						return -3;
					}
				else
					{
					if (s[i] != '0' && s[i] != '1')
						return -4;
					}
				numdig += 1;
				break;
			}
		}
	num_type::size_type numbits;
	if (base == 16)
		{
		if (numdig < 2)
			numdig = 2;
		numbits = 8 * ((numdig + 1) / 2);
		}
	else
		numbits = numdig;
	if (numbits != num.size())
		return -5;
	num_type::base_type b = base == 16 ? num_type::BASE16 : num_type::BASE2;
	num_type::signed_type st = numbits == 1 ? num_type::UNSIGNED : num_type::SIGNED;
	num_type tmp(s, st, b, numbits);
	num = tmp;
	return 0;
	}

void
st_net_assign::operator()(const char *fn, int base,
						  position_type start, position_type stop)
	{
	num_type num;
	bool is_mem = (_net._upper_mem_idx != -1) ? true : false;
	if (!is_mem)
		{
		vbs_err.set_data(vbs_error::SE_TYPE, 0);
		vbs_err.out(_net.name());
		}

	// Open file for processing.
	std::ifstream ifs(fn);
	if (ifs.fail())
		{
		vbs_err.set_data(vbs_error::SE_FILEREAD, 0);
		vbs_err.out(fn);
		}

	// Read data in the direction specified by start/stop.
	int incr;
	if (start < 0)
		start = _net._lower_mem_idx;
	if (stop < 0)
		stop = _net._upper_mem_idx;
	if (start < stop)
		{
		if (start < _net._lower_mem_idx || stop > _net._upper_mem_idx)
			{
			vbs_err.set_data(vbs_error::SE_RANGE, 0);
			vbs_err.out(_net.name());
			}
		incr = 1;
		}
	else
		{
		if (start > _net._upper_mem_idx || stop < _net._lower_mem_idx)
			{
			vbs_err.set_data(vbs_error::SE_RANGE, 0);
			vbs_err.out(_net.name());
			}
		incr = -1;
		}

	int status, line = 0;
	num = _net._storage[start]; // Set size of temporary.
	int cnt = stop - start + 1;
	while (cnt)
		{
		status = readmem_data(ifs, base, num);
		++line;
		if (status < 0)
			{
			vbs_error::strstream_type s;
			s << fn << '[' << line << "]: ";
			switch (status)
				{
				case -1:
					// EOF reached, and we have not finished...
					s << "readmem" << (base == 16 ? 'h' : 'b');
					s << " warning--not enough input data.";
					cout << s.str() << endl;
					return;
				case -2:
					s << "extra address specifier." << endl;
					break;
				case -3:
					s << "invalid hex digit." << endl;
					break;
				case -4:
					s << "invalid binary digit." << endl;
					break;
				case -5:
					s << "incompatible word size, should be ";
					s << num.size() << " bits." << endl;
					break;
				default:
					break;
				}
			vbs_err.set_data(vbs_error::SE_FILEREAD, 0);
			vbs_err.out(s.str());
			}
		else if (status == 1)
			{
			// Continue at this address.
			start = num.to_signed_int();
			}
		else
			{
			if (start > _net._upper_mem_idx)
				break; // Exceeded our memory boundary...
			_net._storage[start] = num;
			start += incr;
			--cnt;
			}
		}

	// Read one last time to reach EOF.
	status = readmem_data(ifs, base, num);
	if (!ifs.eof() || status != -1)
		{
		vbs_error::strstream_type s;
		s << fn << '[' << line << "]: ";
		s << "readmem" << (base == 16 ? 'h' : 'b');
		s << " warning--too much input data.";
		cout << s.str() << endl;
		}
	}

void
st_net_assign::operator()(const num_type &num, position_type ms, position_type ls,
						  position_type m, position_type l)
	{
	bool is_mem = (_net._upper_mem_idx != -1) ? true : false;
	if (is_mem && (m > _net._upper_mem_idx || m < _net._lower_mem_idx))
		return; /* Ignore!!! */

	// Store value.
	num_type before(is_mem ? _net._storage[m] : *_net._storage);
	if (is_mem)
		{
		// Memory write.
		if (ms != -1)
			_net._storage[m] = num(ms, ls);
		else
			_net._storage[m] = num;
		}
	else
		{
		// Net write.
		if (m != -1)
			{
			if (ms != -1)
				(*_net._storage)(m, l) = num(ms, ls);
			else
				(*_net._storage)(m, l) = num;
			}
		else
			{
			if (ms != -1)
				*_net._storage = num(ms, ls);
			else
				*_net._storage = num;
			}
		}

	// If we haven't changed due to the assignment, there's nothing
	// to do after this.  It is though this assignment never occured.
	num_type after(is_mem ? _net._storage[m] : *_net._storage);
	if (compare_c(before, after) == HI)
		{
		// _net.changed(false);
		return;
		}

	// Handle port type.
	if (_net._port_index != 0 && (_net._iodirection == st_net::OUT ||
			_net._iodirection == st_net::INOUT))
		{
		symbol_table &symboltable = vbs_engine::symboltable();
		// We only want to update the port if it's output or
		// inout direction.  We never update the port if it's
		// input or undefined.
		st_net *node = symboltable.get(*_net._port_index)->get_net();
		node->assignment(after, -1, -1, _net._port_ms, _net._port_ls);
		// Even if we are a port, we must still handle the local
		// variables.  So continue;
		}

	// Note value change for $dump API.
	_net.changed(true);

	// Now check to see whether this variable was monitored.
	// If so, add the statements in the list to the event queue.
	if (_net._monitor != 0)
		check_monitor(*(_net._monitor), before, after);
	}

const st_net_evaluate::num_type &
st_net_evaluate::operator()(position_type p_m, position_type p_l,
		position_type m, position_type l, bool port_reference) const
	{
	if ((_net._iodirection == st_net::IN || _net._iodirection == st_net::INOUT)
	 && _net._port_index != 0)
		{
		symbol_table &symboltable = vbs_engine::symboltable();
		net_type *net = symboltable.get(*_net._port_index)->get_net();
		st_net_evaluate eval(*net);
		*_net._result = eval(p_m, p_l, m, l, true);
		}
	else
		{
		if (_net._upper_mem_idx != -1)
			{
			// Memory access.
			if (m > _net._upper_mem_idx || m < _net._lower_mem_idx)
				*_net._result = DC;
			else
				return _net._storage[m];
			}
		else // Net access.
			{
			// Is it a port reference?  If so, use port indices.
			if (port_reference)
				{
				m = p_m;
				l = p_l;
				}
			if (m != -1)
				{
				// Not a memory, so it is part select.
				*_net._result = (*_net._storage)(m, l);
				}
			else
				{
				// We don't accept m < 0.
				return *_net._storage;
				}
			}
		}
	return *_net._result;
	}

hash_value
st_node_find(const char *name, Stack<int> &sc, int *s)
	{
	typedef hash_value hash_type;
	typedef Stack<int> scopelist_type;
	typedef st_instantiation instantiation_type;
	symbol_table &symboltable = vbs_engine::symboltable();

	// There is a hierarchy, need to step through the list.
	hash_type hv;
	scopelist_type tmpscp;
	char *tok, *tokr, *sym;
	instantiation_type *inst;

	// Make a copy of the data for processing.
	sym = new char[strlen(name) + 1];
	strcpy(sym, name);

	// Save pointer to last component.
	tokr = strrchr(sym, '.');
	if (tokr == 0)
		tokr = sym;
	else
		++tokr;

	// Use original scope list for first component of identifier.
	tok = strtok(sym, ".");
	if (tok == 0)
		{
		// Not a scope reference, just lookup the symbol directly.
		hv = symboltable.find(sym, sc);
		delete [] sym;
		return hv;
		}
	hv = symboltable.find(tok, sc, tok != tokr);

	// Search for the rest of the identifier.
	while (hv._scope >= 0)
		{
		inst = symboltable.get(hv)->get_instantiation();
		tok = strtok(0, ".");
		if (tok == 0)
			{
			if (inst != 0)
				{
				// Non-last component must be instantiation.
				hv._value = vbs_error::SE_STDEF;
				hv._scope = -1;
				}
			// Symbol has been resolved.
			break;
			}
		else
			{
			if (inst != 0)
				{
				// Replace with this scope (see below).
				tmpscp.push(inst->instance_scope());
				if (s != 0)
					*s = inst->instance_scope();
				}
			else
				{
				// Non-last component must be instantiation.
				hv._value = vbs_error::SE_STDEF;
				hv._scope = -1;
				break;
				}
			}
		hv = symboltable.find(tok, tmpscp, tok != tokr);
		// Must follow hierarchy, so replace this scope.
		tmpscp.pop();
		}
	delete [] sym;
	return hv;
	}
