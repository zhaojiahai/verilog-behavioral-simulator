// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// misetup.cc

#include "common/error.h"
#include "common/event.h"
#include "common/sym_tab.h"
#include "common/scp_tab.h"
#include "common/time_whl.h"
#include "common/st_func.h"
#include "common/st_inst.h"
#include "common/st_task.h"
#include "common/st_mod.h"
#include "common/st_setup.h"
#include "expr/emon.h"
#include "expr/esetup.h"
#include "stmt/seqblk.h"
#include "stmt/ssetup.h"
#include "misc/partsel.h" // Function return value is a range.
#include "misc/msetup.h"
#include "moditm/initial.h"
#include "moditm/always.h"
#include "moditm/cassign.h"
#include "moditm/func.h"
#include "moditm/task.h"
#include "moditm/modinst.h"
#include "moditm/misetup.h"
#include "vbs.h"


struct setup_module_instance : public module_instance_setup
	{
	typedef Stack<int> scopelist_type;
	typedef st_node_base node_type;
	typedef node_type::hash_type hash_type;
	typedef module_instance::port_conn_list port_conn_list;
	typedef st_instantiation instantiation_type;
	typedef st_module module_type;
	typedef module_instantiation::param_value_list::iterator param_value_ptr;

	setup_module_instance(scopelist_type &s, module_type *m,
						  param_value_ptr *f, const param_value_ptr *l)
		: _scope(s)
		{
		_first = f;
		_last = l;
		_stmod = m;
		}

	void operator()(module_instance *) const;

	scopelist_type &_scope;
	param_value_ptr *_first;
	const param_value_ptr *_last;
	module_type *_stmod;
	};

void
setup_module_instance::operator()(module_instance *p) const
	{
	// Create a module instance symbol table object.
	symbol_table &symboltable = vbs_engine::symboltable();
	instantiation_type *inst = 0;
	hash_type hv(symboltable.find(p->_name, _scope, true));
	if (hv._scope < 0)
		{
		inst = new instantiation_type(p->_name);
		inst->scope(_scope.top());
		inst->_lineno = p->_lineno;
		hv = symboltable.add(inst, true);
		if (hv._scope < 0)
			{
			delete inst;
			vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
			vbs_err.out(p->_name);
			}
		}
	else
		{
		vbs_err.set_data(vbs_error::SE_STDUP, p->_lineno);
		vbs_err.out(p->_name);
		}

	// Setup the port connections.
	if (p->_port_conn != 0)
		{
		unsigned int empty;
		port_conn_list::iterator itp(p->_port_conn->begin());
		port_conn_list::iterator stop(p->_port_conn->end());
		for (empty = 0; itp != stop; ++itp)
			{
			if ((*itp)->setup(setup_port_conn(_scope)) == false)
				++empty;
			}
		if (empty == p->_port_conn->size())
			{
			// Entirely blank list, discard.
			delete p->_port_conn;
			p->_port_conn = 0;
			}
		}

	// Setup the module definition being used.
	scope_table &scopetable = vbs_engine::scopetable();
	scopetable.push_scope(inst->name());
	scopetable.push_level();
	// Moved here (from st_setup.cc) because inst->instance_scope must
	// be set before setting up the module.
	int thisscope = symboltable.scope()++;
	inst->level(scopetable.level());
	inst->instance_scope(thisscope);
	_stmod->instance_scope(thisscope);
	_stmod->setup(setup_st_node(_scope, p->_port_conn, _first, _last));
	scopetable.insert(thisscope, scopetable.scope_string());
	scopetable.pop_level();
	scopetable.pop_scope();
	}

// Utility function that requires access to setup_module_instance.

void
setup_toplevel(st_module *stmod, Stack<int> &scope)
	{
	module_instance *inst = new module_instance(stmod->name().c_str(), 0);
	inst->setup(setup_module_instance(scope, stmod, 0, 0));
	delete inst;
	}


// Module item setup function objects.


void
setup_module_item::operator()(initial *p) const
	{
	// Initial statement now require to be a sequential block.
	initial::stmt_type *st, *cpy = p->_stmt->copy_constructor();
	if (p->_stmt->get_seq_block() != 0)
		st = cpy;
	else
		{
		// Force an initial statement to be a sequential block.
		seq_block_stmt::stmt_list *lst = new seq_block_stmt::stmt_list;
		lst->push_back(cpy);
		st = new seq_block_stmt(lst);
		}
	st->setup(setup_stmt(_scope, st));
	time_wheel<stmt_base> &timewheel = vbs_engine::timewheel();
	timewheel.add_event(0, st);
	}

void
setup_module_item::operator()(always *p) const
	{
	// Always statement now require to be a sequential block.
	always::stmt_type *st, *cpy = p->_stmt->copy_constructor();
	if (p->_stmt->get_seq_block() != 0)
		st = cpy;
	else
		{
		// Force an initial statement to be a sequential block.
		seq_block_stmt::stmt_list *lst = new seq_block_stmt::stmt_list;
		lst->push_back(cpy);
		st = new seq_block_stmt(lst);
		st->_ec = cpy->_ec;
		cpy->_ec = 0;
		}
	st->_always = true;
	st->setup(setup_stmt(_scope, st));

	// If no event control, must schedule for execution.
	if (st->_ec == 0)
		{
		time_wheel<stmt_base> &timewheel = vbs_engine::timewheel();
		timewheel.add_event(0, st);
		}
	}

void
setup_module_item::operator()(cont_assign *p) const
	{
	// A continuous assignment has many similarities with other
	// Verilog constructs.  One of them is the 'always' procedural
	// block.  Since a change in any one of the variables in the
	// right-hand-side of the assignment will cause an event to occur.
	// So we must set it up correctly.
	event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
	cont_assign::assign_stmt_list::iterator itp(p->_assign->begin());
	cont_assign::assign_stmt_list::iterator stop(p->_assign->end());
	event_cache_type *cache = 0;
	event_type *ev = 0;
	cont_assign::assign_stmt *st = 0;
	for (; itp != stop; ++itp)
		{
		st = (*itp)->copy_constructor();
		// This delay or event control should only be a delay literal,
		// i.e. delay_num.  No need setup.
		if (st->_ec != 0)
			{
			vbs_err.set_data(vbs_error::SE_SUPPORT, p->_dec->_lineno);
			vbs_err.out("event control in continuous assignment");
			}
		if (p->_dec != 0)
			{
			if (p->_dec->is_event_control())
				{
				vbs_err.set_data(vbs_error::SE_SUPPORT, p->_dec->_lineno);
				vbs_err.out("event control in continuous assignment");
				}
			st->_dec = p->_dec->copy_constructor();
			}
		st->setup(setup_stmt(_scope, st));

		cache = new event_cache_type(true, st);
		ev = new change_event<stmt_type>(cache, DC);
		counted_ptr< event_base<stmt_type> > e(ev);
		st->_rval->monitor(monitor_expr(&e));

		// Schedule assignment at least once.
		eventqueue.add_event(ev);
		}
	}

void
setup_module_item::operator()(function *p) const
	{
	// Setup function definitions mean saving it in the symbol table.
	symbol_table &symboltable = vbs_engine::symboltable();
	hash_type hv(st_node_find(p->_name.c_str(), _scope));
	func_type *func;
	if (hv._scope < 0)
		{
		// Does not exist, create a new symbol table node.
		// Make a copy of the statement, because each new function
		// definition could have a different scope if the module it
		// is in is instantiated more than once.
		function::stmt_type *st = p->_stmt->copy_constructor();
		func = new func_type(p->_name, st, p->_decl);
		function::range_type::position_type ms = 0, ls = 0;
		if (p->_retsize != 0)
			{
			p->_retsize->setup(setup_select(_scope));
			p->_retsize->get_data(ms, ls);
			}
		func->_storage = num_type(ms, ls);
		func->scope(_scope.top());
		func->_lineno = p->_lineno;
		hv = symboltable.add(func);
		if (hv._scope < 0)
			{
			delete func;
			vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
			vbs_err.out(p->_name);
			}
		}
	else
		{
		vbs_err.set_data(vbs_error::SE_STDUP, p->_lineno);
		vbs_err.out(p->_name);
		}
	}

void
setup_module_item::operator()(task *p) const
	{
	// Setup task definitions mean saving it in the symbol table.
	symbol_table &symboltable = vbs_engine::symboltable();
	hash_type hv(st_node_find(p->_name.c_str(), _scope));
	task_type *t;
	if (hv._scope < 0)
		{
		// Does not exist, create a new symbol table node.
		// Make a copy of the statement, because each new function
		// definition could have a different scope if the module it
		// is in is instantiated more than once.
		task::stmt_type *st = p->_stmt->copy_constructor();
		t = new task_type(p->_name, st, p->_decl);
		t->scope(_scope.top());
		t->_lineno = p->_lineno;
		hv = symboltable.add(t);
		if (hv._scope < 0)
			{
			delete t;
			vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
			vbs_err.out(p->_name);
			}
		}
	else
		{
		vbs_err.set_data(vbs_error::SE_STDUP, p->_lineno);
		vbs_err.out(p->_name);
		}
	}

void
setup_module_item::operator()(module_instantiation *p) const
	{
	static Stack<hash_type> recurse;
	symbol_table &symboltable = vbs_engine::symboltable();

	// Find the module definition for setting up the instances.
	hash_type hv(st_node_find(p->_name.c_str(), _scope));
	if (hv._scope < 0)
		{
		vbs_err.set_data((vbs_error::value_type) hv._value, p->_lineno);
		vbs_err.out(p->_name);
		}
	module_type *mod = symboltable.get(hv)->get_module();
	if (mod == 0)
		{
		vbs_err.set_data(vbs_error::SE_TYPE, p->_lineno);
		vbs_err.out(p->_name);
		}

	// Check if we are recursing.
	Stack<hash_type>::iterator idx(recurse.begin());
	Stack<hash_type>::iterator bottom(recurse.end());
	for (; idx != bottom; ++idx)
		{
		if (*idx == hv)
			{
			vbs_err.set_data(vbs_error::SE_RECURSE, p->_lineno);
			vbs_err.out(p->_name);
			}
		}
	recurse.push(hv);

	// Setup each instance of the instantiation.
	module_instantiation::mod_inst_list::iterator itp(p->_instance->begin());
	module_instantiation::mod_inst_list::iterator stop(p->_instance->end());
	if (p->_param_value != 0)
		{
		module_instantiation::param_value_list::iterator
			first(p->_param_value->begin());
		module_instantiation::param_value_list::iterator
			last(p->_param_value->end());
		for (; first != last; ++first)
			(*first)->setup(setup_expr(_scope));

		first = p->_param_value->begin();
		last = p->_param_value->end();
		for (; itp != stop; ++itp)
			(*itp)->setup(setup_module_instance(_scope, mod, &first, &last));
		}
	else
		{
		for (; itp != stop; ++itp)
			(*itp)->setup(setup_module_instance(_scope, mod, 0, 0));
		}

	// End recursion.
	recurse.pop();
	}


// Front ends to make virtual calls work.


void
setup_module_item::operator()(io_decl *p) const
	{ decl_setup::operator()(p); }

void
setup_module_item::operator()(net_decl *p) const
	{ decl_setup::operator()(p); }

void
setup_module_item::operator()(reg_decl *p) const
	{ decl_setup::operator()(p); }

void
setup_module_item::operator()(int_decl *p) const
	{ decl_setup::operator()(p); }

void
setup_module_item::operator()(param_decl *p) const
	{ decl_setup::operator()(p); }
