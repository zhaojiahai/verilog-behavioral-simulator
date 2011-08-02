// Verilog Behavioral Simulator
// Copyright (C) 2000-2001,2011 George Varughese, Jimen Ching
//
// With modifications from:
// Jimen Ching <jimen.ching@gmail.com>
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dumpvcd.cc

#include <ctime> // For ctime.

#include "common/scp_tab.h"
#include "common/time_whl.h"
#include "common/st_net.h"
#include "common/st_eval.h"
#include "common/dumpstrm.h"
#include "common/dumpvcd.h"
#include "expr/rangeid.h"
#include "expr/eeval.h"

extern scope_table scopetable;
extern time_wheel<stmt_base> timewheel;

dump_vcd::dump_vcd(dump_stream &d)
	: dump_base(d),
	  _get_next_name_string("~!@#$%^&*()"
		  "abcdefghijklmnopqrstuvwxyz"
		  "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
	{
	_get_next_name_index  = 0;
	_get_next_name_end    = 0;
	_get_next_name        = "~";
	_get_next_name_chars  = _get_next_name_string.length();
	}

dump_vcd::~dump_vcd()
	{
	// this keeps dinotrace happy to display last segment
	strstream_type str_out;
	str_out << "#" << timewheel.current_time() + 1 << endl;
	_dump_out << str_out;
	}

dump_vcd::str_type
dump_vcd::get_next_name() 
	{
	if (_get_next_name_index == _get_next_name_chars)
		{
		_get_next_name_index = 0;
		++_get_next_name_end;
		_get_next_name += _get_next_name_string[_get_next_name_index++];
		} 
	else
		_get_next_name[_get_next_name_end] = _get_next_name_string[_get_next_name_index++];
	return _get_next_name;
	}

void
dump_vcd::pre_second_pass()
	{ 
	strstream_type str_out;
	str_out << "$scope module "
			<< scopetable.top_level()
			<< " $end"
			<< endl;
	_dump_out << str_out;
	}

void
dump_vcd::second_pass(ident_type *rid, int lvls, net_list *n)
	{
	strstream_type str_out;

	// Ignore if nothing to output.
	if (n->size() == 0)
		return;

	str_type nm = rid->name();
	str_type top = scopetable.top_level();
#if __GNUC__ == 2 && __GNUC_MINOR__ >= 95
	if (nm.compare(top, 0, top.size()) == 0)
#else
	if (nm.compare(0, top.size(), top) == 0)
#endif
		nm.replace(0, top.length()+1, "");
	if (scopetable.is_net())
		{
		char * myscope = 0;
		myscope = scopetable.get_scope(nm.c_str());
		if (myscope != 0)
			str_out << "$scope module " << scopetable.get_scope(nm.c_str())
					<< " $end" << endl;
		}
	else
		{
		if (nm.size() > 0 && nm != top)
			str_out << "$scope module " << nm << " $end" << endl;
		}

	net_list::reverse_iterator n1(n->rbegin());
	net_list::reverse_iterator n2(n->rend());
	int last_scope = (*n1)->scope();
	for (; n1 != n2; ++n1)
		{
		if (last_scope != (*n1)->scope()) 
			{
			if (last_scope != 1)
				str_out << "$upscope $end" << endl;
			if ((*n1)->scope() == 1)
				str_out << "$upscope $end" << endl;
			else
				str_out << "$scope module " << scopetable.find((*n1)->scope())
						<< " $end" << endl;
			}
		str_type vcd_sig_name = get_next_name();
		name_table[*n1] = vcd_sig_name;
		str_out << "$var reg " << (*n1)->size()  << " "
		<< vcd_sig_name  << " " << (*n1)->name();
		if ((*n1)->size() > 1)
			{
			str_out << " [" << (*n1)->port_msb() << ":"
					<< (*n1)->port_lsb() << "]";
			}
		str_out << " $end" << endl;
		last_scope = (*n1)->scope();
		}
	if (!scopetable.is_net() && top != rid->name())
		str_out << "$upscope $end" << endl;
	_dump_out << str_out;
	for (n1 = n->rbegin(), n2 = n->rend(); n1 != n2; ++n1)
		{
		if ((*n1)->depth() > 0) // Skip memory variables.
			continue;
		n_lst.insert(n_lst.end(),*n1);
		}
	return;
	}

void
dump_vcd::post_second_pass()
	{
	strstream_type str_out;
	str_out << "$upscope $end" << endl;
	str_out << "$enddefinitions $end" << endl; 
	str_out << "#0" << endl;
	str_out << "$dumpvars" << endl;
	net_list::iterator nstart = n_lst.begin();
	net_list::iterator nstop  = n_lst.end();
	for (; nstart != nstop ; nstart++)
		{
		if ((*nstart)->depth() > 0) // Skip memory variables.
			continue;
		const num_type &n((*nstart)->evaluate(evaluate_st_node()));
		if ((*nstart)->size() > 1) 
			str_out << "b" <<  n << " " << name_table[*nstart] << endl;
		else
			str_out << n << name_table[*nstart] << endl;
		(*nstart)->changed(false);
		}
	str_out << "$end" << endl;
	_dump_out << str_out;
	}

void
dump_vcd::create_header()
	{
	strstream_type str_out;
	time_t _tmp;
	time_t _tm = time(&_tmp);

	str_out << "$date" << endl 
			<<  "\t" << ctime(&_tm) // ctime appends newline already.
			<< "$end"  << endl 
			<< "$version" << endl
			<< "\tVBS Value Change Dump (VCD)" << endl
			<< "$end" << endl
			<< "$timescale" << endl
			<< "\t1ns" << endl
			<< "$end" << endl;
	_dump_out << str_out;
	// The rest are created during setup.
	}

void
dump_vcd::dump_values()
	{
	strstream_type str_out;
	net_list::iterator nstart(n_lst.begin());
	net_list::iterator nstop(n_lst.end());
	str_out << "#" << timewheel.current_time() << endl;
	for (; nstart != nstop; ++nstart)
		{
		if ((*nstart)->changed())
			{
			const num_type &n((*nstart)->evaluate(evaluate_st_node()));
			if ((*nstart)->size() > 1)
				str_out << "b" << n << " " << name_table[*nstart] << endl;
			else
				str_out << n << name_table[*nstart] << endl;
			(*nstart)->changed(false); 
			}
		}
	_dump_out << str_out;
	}


// Dump the current value of all selected signals
// seperating this from dump_values ( with an argument ) makes dump
// values a little faster..
void
dump_vcd::dump_all()
	{
	strstream_type str_out;
	net_list::iterator nstart(n_lst.begin());
	net_list::iterator nstop(n_lst.end());
	str_out << "#" << timewheel.current_time() << endl;
	str_out << "$dumpall" << endl;
	for (; nstart != nstop; ++nstart)
		{
		const num_type &n((*nstart)->evaluate(evaluate_st_node()));
		if ((*nstart)->size() > 1)
			str_out << "b" << n << " " << name_table[*nstart] << endl;
		else
			str_out << n << name_table[*nstart] << endl;
		}
	str_out << "$end" << endl;
	_dump_out << str_out;
	} 

// indicating a dumpon
void
dump_vcd::dump_on()
	{
	strstream_type str_out;
	str_out << "#" << timewheel.current_time() << endl;
	str_out << "$dumpon" << endl;
	net_list::iterator nstart = n_lst.begin();
	net_list::iterator nstop  = n_lst.end();
	for (; nstart != nstop ; nstart++)
		{
		const num_type &n((*nstart)->evaluate(evaluate_st_node()));
		if ((*nstart)->size() > 1)
			str_out << "b" << n << " " << name_table[*nstart] << endl;
		else
			str_out << n << name_table[*nstart] << endl;
		}
	str_out << "$end" << endl;
	_dump_out << str_out;
	}


// indicating a dumpoff
void
dump_vcd::dump_off()
	{
	strstream_type str_out;
	str_out << "#" << timewheel.current_time() << endl;
	str_out << "$dumpoff" << endl;
	net_list::iterator nstart(n_lst.begin());
	net_list::iterator nstop(n_lst.end());
	for (; nstart != nstop ; nstart++)
		{
		if ((*nstart)->size() > 1)
			str_out << "bx" <<  " " << name_table[*nstart] << endl;
		else
			str_out << "x" << name_table[*nstart] << endl;
		}
	str_out << "$end" << endl;
	_dump_out << str_out; 
	}
