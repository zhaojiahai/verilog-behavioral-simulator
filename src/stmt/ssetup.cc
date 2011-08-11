// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// ssetup.cc

#include "common/error.h"
#include "common/sym_tab.h"
#include "common/scp_tab.h"
#include "common/dumpinfo.h"
#include "common/st_func.h"
#include "common/st_task.h"
#include "common/st_setup.h"
#include "common/st_util.h"
#include "expr/qstr.h"
#include "expr/number.h"
#include "expr/rangeid.h"
#include "expr/eeval.h"
#include "expr/esetup.h"
#include "misc/decsetup.h"
#include "misc/lvalue.h"
#include "misc/msetup.h"
#include "stmt/finish.h"
#include "stmt/display.h"
#include "stmt/write.h"
#include "stmt/monitor.h"
#include "stmt/readmem.h"
#include "stmt/strobe.h"
#include "stmt/dumptsk.h"
#include "stmt/xtime.h"
#include "stmt/stime.h"
#include "stmt/random.h"
#include "stmt/seqblk.h"
#include "stmt/taskenbl.h"
#include "stmt/assign.h"
#include "stmt/ifelse.h"
#include "stmt/case.h"
#include "stmt/loopstmt.h"
#include "stmt/ssetup.h"
#include "vbs.h"

// Case item for case statement.

struct setup_case_item : public case_item_setup
	{
	typedef Stack<int> scopelist_type;
	typedef stmt_base stmt_type;

	setup_case_item(scopelist_type &s, stmt_type *st)
		: _scope(s)
		{ _parent = st; }
	virtual ~setup_case_item()
		{}

	void operator()(case_item *) const;

	scopelist_type &_scope;
	stmt_type *_parent;
	};

void
setup_case_item::operator()(case_item *c) const
	{
	c->_stmt->setup(setup_stmt(_scope, _parent));
	// Default cases do not have expression lists.
	if (c->is_default())
		return;
	case_item::expr_list::iterator itp(c->_expr->begin());
	case_item::expr_list::iterator stop(c->_expr->end());
	for (; itp != stop; ++itp)
		(*itp)->setup(setup_expr(_scope));
	}


// All statements.

void
setup_stmt::operator()(systask_finish *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_display *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_write *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_monitor *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_readmemb *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_readmemh *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_strobe *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_dumpon *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_dumpoff *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_dumpall *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_dumpvars *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	dumpinfo.dump_on(true);
	t->_done_setup = true;
	}

void
setup_stmt::operator()(systask_dumpfile *p) const
	{
	task_type *t = new task_type(p->_name, p);
	// System tasks always have global scope.
	t->scope(0);
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type::hash_type hv(symboltable.add(t));
	if (hv._scope < 0)
		{
		delete t;
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System tasks do not need to be setup.
	t->_done_setup = true;
	}

void
setup_stmt::operator()(sysfunc_time *p) const
	{
	func_type *f = new func_type(p->_name, p);
	// System functions always have global scope.
	f->scope(0);
	f->_storage = func_type::num_type(63, 0);
	// Need to save index to get the symbol table node, to store
	// the return value (time).  This is a function call.
	symbol_table &symboltable = vbs_engine::symboltable();
	p->_index = symboltable.add(f);
	if (p->_index._scope < 0)
		{
		delete f;
		vbs_err.set_data((vbs_error::value_type) p->_index._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System functions do not need to be setup.
	f->_done_setup = true;
	}

void
setup_stmt::operator()(sysfunc_stime *p) const
	{
	func_type *f = new func_type(p->_name, p);
	// System functions always have global scope.
	f->scope(0);
	f->_storage = func_type::num_type(31, 0);
	// Need to save index to get the symbol table node, to store
	// the return value (time).  This is a function call.
	symbol_table &symboltable = vbs_engine::symboltable();
	p->_index = symboltable.add(f);
	if (p->_index._scope < 0)
		{
		delete f;
		vbs_err.set_data((vbs_error::value_type) p->_index._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System functions do not need to be setup.
	f->_done_setup = true;
	}

void
setup_stmt::operator()(sysfunc_random *p) const
	{
	func_type *f = new func_type(p->_name, p);
	// System functions always have global scope.
	f->scope(0);
	f->_storage = func_type::num_type(32, 0);
	// Need to save index to get the symbol table node, to store
	// the return value (random).  This is a function call.
	symbol_table &symboltable = vbs_engine::symboltable();
	p->_index = symboltable.add(f);
	if (p->_index._scope < 0)
		{
		delete f;
		vbs_err.set_data((vbs_error::value_type) p->_index._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	// System functions do not need to be setup.
	f->_done_setup = true;
	}


// The rest of the function definitions are for statement setup.


inline void
setup_stmt::handle_dec(stmt_type *p) const
	{
	if (p->_ec != 0)
		p->_ec->setup(setup_dec(_scope, p, _parent));
	if (p->_dec != 0)
		p->_dec->setup(setup_dec(_scope, p, _parent));
	if (!p->_always && p->_ec != 0 && p->_dec != 0)
		{
		vbs_err.set_data(vbs_error::SE_SUPPORT, p->_lineno);
		vbs_err.out("combined delay and event control");
		}
	}

void
setup_stmt::operator()(null_stmt *p) const
	{
	handle_dec(p);
	}

void
setup_stmt::operator()(seq_block_stmt *p) const
	{
	handle_dec(p);
	seq_block_stmt::stmt_list::iterator itp(p->_stmt->begin());
	seq_block_stmt::stmt_list::iterator stop(p->_stmt->end());
	for (; itp != stop; ++itp)
		(*itp)->setup(setup_stmt(_scope, _parent));

	// The following construct will result in an infinite loop.
	//    always lval <= #parm expr;
	// We converted this non-blocking statement into a sequential
	// block, so now we need to check this scenario.  Note, this
	// doesn't prevent the case where we have a list of nb
	// assignment statements.
	if (_parent != 0 && _parent->_always && p->_stmt->size() == 1
	 && _parent->_dec == 0 && _parent->_ec == 0)
		{
		itp = p->_stmt->begin();
		stmt_type *s = itp->get();
		assignment_stmt *as = s->get_assign();
		if (as != 0 && as->_nonblocking)
			{
			vbs_err.set_data(vbs_error::SE_SUPPORT, _parent->_lineno);
			vbs_err.out("causes infinite loop");
			}
		}
	}

void
setup_stmt::reset()
	{
	delete _postponedsetup;
	_postponedsetup = new stmt_list;
	}

void
setup_stmt::second_pass()
	{
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	scope_table &scopetable = vbs_engine::scopetable();
	task_enable_type::arg_list::iterator itp;
	task_enable_type::arg_list::iterator stop;
	stmt_list::iterator dtp = _postponedsetup->begin();
	stmt_list::iterator dstop = _postponedsetup->end();
	dumpinfo.pre_second_pass();
	for (; dtp != dstop; ++dtp)
		{
		task_enable_stmt *p = (*dtp)->get_task_enable();
		if ( p->_argument != 0)
			{
			itp = p->_argument->begin();
			stop = p->_argument->end();
			number *n = (*itp)->get_number();
			if (n == 0)
				{
				strstream_type buf;
				buf << *(*itp);
				vbs_err.set_data(vbs_error::SE_TYPE, (*itp)->_lineno);
				vbs_err.out(buf);
				}
			int lvls = static_cast<unsigned long>(*n);
			++itp;
			for (; itp != stop; ++itp)
				{
				dump_info::ident_type *i = (*itp)->get_range_id();
				scopetable.clear_dump_list();
				if (i != 0)
					{
					scope_table::net_list n_lst;
					n_lst = scopetable.get_nets(i->name().c_str(), lvls);
					scopetable.add(n_lst);
					}
	
				// Output VCD header if enabled.
				dumpinfo.second_pass(i, lvls, scopetable.get_dump_list());
				}
			}
		else
			{ // no args
			scope_table::net_list n_lst;
			n_lst = scopetable.get_nets(1,0);
			scopetable.add(n_lst) ;	
			dump_info::ident_type *i =
				new dump_info::ident_type(dumpinfo.top_level().c_str());
			int lvls = 0;
			dumpinfo.second_pass(i, lvls, scopetable.get_dump_list());
			}
		}
	dumpinfo.post_second_pass();
	}

void
setup_stmt::operator()(task_enable_stmt *p) const
	{
	handle_dec(p);
	if (_scope.size() > 1)
		{
		// The parent scope was placed on top for deep function calls.
		// Skip that...
		scopelist_type::iterator itp(_scope.begin());
		++itp;
		p->_scope = *itp;
		}
	else
		{
		// Only one scope level, use that...
		p->_scope = _scope.top();
		}
	// Check for symbol, and save the index.
	scopelist_type::iterator modscp(_scope.end());
	--modscp; --modscp; // Module scope.
	scopelist_type::value_type save, tmp = *modscp;
	p->_index = st_node_find(p->_name.c_str(), _scope, &tmp);
	if (p->_index._scope < 0)
		{
		if (p->_name[0] == '$')
			p->_index._value = vbs_error::SE_TFSUPPORT;
		vbs_err.set_data((vbs_error::value_type) p->_index._value, p->_lineno);
		vbs_err.out(p->_name);
		}

	// Make sure it's a task.
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type *node = symboltable.get(p->_index)->get_task();
	if (node == 0)
		{
		vbs_err.set_data(vbs_error::SE_NTASK, p->_lineno);
		vbs_err.out(p->_name);
		}
	// Tasks are not setup until the task is enabled.  This is a
	// task enable, so setup...
	save = *modscp;
	if (tmp != *modscp)
		*modscp = tmp; // Use instance scope if hierarchical name.
	node->setup(setup_st_node(_scope, p->_argument, _parent));
	if (*modscp != save)
		*modscp = save; // Restore original scope.

	// Now check argument list.
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	bool display_test = (p->_name == "$display");
	bool write_test = (p->_name == "$write");
	bool monitor_test = (p->_name == "$monitor");
	bool readmemb_test = (p->_name == "$readmemb");
	bool readmemh_test = (p->_name == "$readmemh");
	bool strobe_test = (p->_name == "$strobe");
	bool dumpon_test = (p->_name == "$dumpon");
	bool dumpoff_test = (p->_name == "$dumpoff");
	bool dumpall_test = (p->_name == "$dumpall");
	bool dumpvars_test = (p->_name == "$dumpvars");
	bool dumpfile_test = (p->_name == "$dumpfile");
	if (dumpvars_test) 
		{
		// Postpone setup to end of setup phase I.
		_postponedsetup->insert(_postponedsetup->end(), p);
		if (dumpinfo.dump_status() == false)
			dumpinfo.dump_format("vcd"); // Was not set, use default.

		// First argument is the level (expression), must setup!
		if (p->_argument != 0)
			{
			task_enable_stmt::arg_list::iterator itp(p->_argument->begin());
			(*itp)->setup(setup_expr(_scope, true));
			}

		return; // for now...
		}
	else if (dumpon_test || dumpoff_test || dumpall_test)
		{
		// No arguments.
		if (p->_argument != 0)
			{
			vbs_err.set_data(vbs_error::SE_NARGLST, p->_lineno);
			vbs_err.out(p->_name);
			}
		}
	else if (display_test || write_test || monitor_test || strobe_test ||
			 dumpfile_test)
		{
		// At least one argument.
		if (p->_argument == 0)
			{
			vbs_err.set_data(vbs_error::SE_NARGLST, p->_lineno);
			vbs_err.out(p->_name);
			}
		else if (p->_argument->size() < 1)
			{
			vbs_err.set_data(vbs_error::SE_NARGLST, p->_lineno);
			vbs_err.out(p->_name);
			}
		}
	else if (readmemb_test || readmemh_test)
		{
		// At least two arguments.
		if (p->_argument == 0)
			{
			vbs_err.set_data(vbs_error::SE_NARGLST, p->_lineno);
			vbs_err.out(p->_name);
			}
		else if (p->_argument->size() < 2)
			{
			vbs_err.set_data(vbs_error::SE_NARGLST, p->_lineno);
			vbs_err.out(p->_name);
			}
		}
	else
		{
		// User defined tasks.
		if (!check_arguments(node->_iovariable, p->_argument))
			{
			vbs_err.set_data(vbs_error::SE_NARGLST, p->_lineno);
			vbs_err.out(p->_name);
			}
		}

	// Everything is ok, so setup the argument list, if any.
	if (p->_argument == 0)
		return;
	task_enable_stmt::arg_list::iterator itp(p->_argument->begin());
	task_enable_stmt::arg_list::iterator stop(p->_argument->end());
	if (display_test || write_test || monitor_test || strobe_test ||
		readmemb_test || readmemh_test ||
		dumpfile_test)
		{
		// These tasks take a qouted string as first parameter.
		vbs_error::value_type err = vbs_error::SE_NONE;
		qouted_str *qs = (*itp)->get_qouted_str();
		if (qs == 0)
			{
			if (readmemb_test || readmemh_test || dumpfile_test)
				err = vbs_error::SE_INVARG;
			else
				err = vbs_error::SE_NOFMT;
			}
		else
			{
			const stmt_type::str_type &fmt(qs->get_str());
			if (fmt.size() == 0)
				{
				if (readmemb_test || readmemh_test || dumpfile_test)
					err = vbs_error::SE_INVARG;
				else
					err = vbs_error::SE_NOFMT;
				}
			else
				{
				if (dumpfile_test)
					{
					// FIXME: do this before second pass setup.
					dumpinfo.dump_file(fmt);
					}
				else if (readmemb_test || readmemh_test)
					{
					++itp;
					if ((*itp)->get_range_id() == 0)
						err = vbs_error::SE_INVARG;
					--itp; // So identifier is setup below.
					}
				else
					{
					// System tasks that take an output format.
					err = check_format(qs->get_str());
					}
				}
			}
		if (err != vbs_error::SE_NONE)
			{
			vbs_err.set_data(err, p->_lineno);
			vbs_err.out(p->_name);
			}
		++itp;
		}

	// If a $monitor/$strobe task enable, need to append an event to the
	// expressions in the arguments.
	event_cache_type *cache = 0;
	event_type *ev = 0;
	if (monitor_test)
		{
		cache = new event_cache_type(true, p);
		ev = new monitor_event<stmt_type>(cache, DC);
		p->_event = new counted_ptr<event_type>(ev);
		}
	else if (strobe_test)
		{
		cache = new event_cache_type(true, p);
		ev = new strobe_event<stmt_type>(cache, DC);
		p->_event = new counted_ptr<event_type>(ev);
		}
	for (; itp != stop; ++itp)
		(*itp)->setup(setup_expr(_scope));
	}

void
setup_stmt::operator()(assignment_stmt *p) const
	{
	handle_dec(p);
	if (p->_delayed_store != 0)
		{
		// Trigger only this statement for non-blocking assignment.
		if (p->_nonblocking)
			p->_delayed_store->setup(setup_dec(_scope, p, 0));
		else
			p->_delayed_store->setup(setup_dec(_scope, p, _parent));
		}
	if (p->_nonblocking)
		{
		event_cache_type *c = new event_cache_type(false, p);
		p->_event = new nonblock_event<stmt_type>(c, DC);
		}
	setup_lvalue::size_type size = p->_lval->setup(setup_lvalue(_scope));
	p->_rval->setup(setup_expr(_scope, false, _parent, size));
	}

void
setup_stmt::operator()(if_else_stmt *p) const
	{
	handle_dec(p);
	p->_expr->setup(setup_expr(_scope));
	// If statement is non-sequential-block and has delay, then create
	// a sequential-block wrapper statement to handle it.
	if (p->_true_stmt->_dec != 0 || p->_true_stmt->_ec != 0)
		{
		seq_block_stmt::stmt_list *lst = new seq_block_stmt::stmt_list;
		lst->push_back(p->_true_stmt);
		p->_true_stmt = new seq_block_stmt(lst);
		}
	p->_true_stmt->setup(setup_stmt(_scope, _parent));
	if (p->_false_stmt != 0)
		{
		if (p->_false_stmt->_dec != 0 || p->_false_stmt->_ec != 0)
			{
			seq_block_stmt::stmt_list *lst = new seq_block_stmt::stmt_list;
			lst->push_back(p->_false_stmt);
			p->_false_stmt = new seq_block_stmt(lst);
			}
		p->_false_stmt->setup(setup_stmt(_scope, _parent));
		}
	}

void
setup_stmt::operator()(case_stmt *p) const
	{
	handle_dec(p);
	p->_expr->setup(setup_expr(_scope));
	case_stmt::citem_list::iterator itp(p->_case_item->begin());
	case_stmt::citem_list::iterator stop(p->_case_item->end());
	for (; itp != stop; ++itp)
		{
		(*itp)->setup(setup_case_item(_scope, _parent));
		if ((*itp)->is_default() == true)
			{
			if (p->_def == 0)
				{
				case_stmt::citem_list::iterator delme(itp);
				p->_def = *itp;
				--itp; // Move iterator back one.
				p->_case_item->erase(delme);
				}
			else
				{
				vbs_err.set_data(vbs_error::SE_MULTDEFLT, p->_lineno);
				vbs_err.out("default:");
				}
			}
		}
	}

void
setup_stmt::operator()(loop_stmt *p) const
	{
	handle_dec(p);
	if (p->_preassign != 0)
		p->_preassign->setup(setup_stmt(_scope, _parent));
	if (p->_expr != 0)
		p->_expr->setup(setup_expr(_scope));
	if (p->_postassign != 0)
		p->_postassign->setup(setup_stmt(_scope, _parent));
	// If statement is non-sequential-block and has delay, then create
	// a sequential-block wrapper statement to handle it.
	if (p->_stmt->_dec != 0 || p->_stmt->_ec != 0)
		{
		seq_block_stmt::stmt_list *lst = new seq_block_stmt::stmt_list;
		lst->push_back(p->_stmt);
		p->_stmt = new seq_block_stmt(lst);
		}
	p->_stmt->setup(setup_stmt(_scope, _parent));
	}

setup_stmt::stmt_list *setup_stmt::_postponedsetup;
