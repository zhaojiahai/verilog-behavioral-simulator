// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_setup.cc

#include "common/error.h"
#include "common/sym_tab.h"
#include "common/st_mod.h"
#include "common/st_net.h"
#include "common/st_func.h"
#include "common/st_task.h"
#include "common/st_setup.h"
#include "stmt/ssetup.h"
#include "misc/msetup.h"
#include "moditm/tfsetup.h"
#include "moditm/misetup.h"

extern symbol_table symboltable;

// sim_start

setup_st_node::setup_st_node(scopelist_type &s, arg_list *a, stmt_type *p)
	: _scope(s)
	{
	_caller_arg = a;
	_parent = p;
	_port_conn = 0;
	_first = 0;
	_last = 0;
	}

setup_st_node::setup_st_node(scopelist_type &s, port_conn_list *p,
		param_value_ptr *f, const param_value_ptr *l)
	: _scope(s)
	{
	_caller_arg = 0;
	_parent = 0;
	_port_conn = p;
	_first = f;
	_last = l;
	}

void
setup_st_node::operator()(st_module *p) const
	{
	// Setup local scope.  Each new module has its own scope.
	int thisscope = p->instance_scope();
	scopelist_type localscope;

	localscope.push(0); // Global scope.
	localscope.push(thisscope); // Local scope.

	// Set the filename so when an error occurs, the error object will
	// print the correct file.
	vbs_err.push_filename(p->_filename);

	// Create ports first.  We have to create the symbol table nets before
	// any io declarations are done in the module items.
	if (p->_port_list != 0)
		{
		// Create the net type symbol table so checks can be made.
		module_type::port_list::iterator itp(p->_port_list->begin());
		module_type::port_list::iterator stop(p->_port_list->end());
		for (; itp != stop; ++itp)
			{
			net_type *net = new net_type((*itp)->name());
			net->scope(thisscope);
			net->_lineno = (*itp)->_lineno;
			(*itp)->index(symboltable.add(net));
			if ((*itp)->index()._scope < 0)
				{
				delete net;
				vbs_err.set_data((vbs_error::value_type) (*itp)->index()._value,
					(*itp)->_lineno);
				vbs_err.out((*itp)->name());
				}
			}
		}
	if (p->_module_item != 0)
		{
		module_type::module_item_list::iterator itp(p->_module_item->begin());
		module_type::module_item_list::iterator stop(p->_module_item->end());
		for (; itp != stop; ++itp)
			{
			(*itp)->setup(setup_module_item(localscope, 0, _first, _last));

			DEBUG_STATE(DEBUG_SYMBOL_TABLE);
			DEBUG_OUTPUT("DEBUG_SYMBOL_TABLE:  Between module items:\n");
			DEBUG_OUTPUT(symboltable);
			DEBUG_OUTPUT("\n\n");
			}
		}

	// Now check to make sure the port connections and io declarations match.
	if (p->_port_list != 0)
		{
		// Ports need port connections.  Pass the port connections to
		// setup the ports.
		if (_port_conn != 0)
			{
			if (_port_conn->size() != p->_port_list->size())
				{
				port_conn_list::iterator itp(_port_conn->begin());
				vbs_err.set_data(vbs_error::SE_NARGLST, (*itp)->_lineno);
				vbs_err.out(p->_name);
				}
			port_conn_list::iterator idx(_port_conn->begin());
			module_type::port_list::iterator pitp(p->_port_list->begin());
			module_type::port_list::iterator pstop(p->_port_list->end());
			for (; pitp != pstop; ++idx, ++pitp)
				{
				// Ports and port connections lists are the same length.
				// So no need worry about going pass the end.
				(*pitp)->setup(setup_port(localscope, (*idx).get()));
				}
			}
		}

	// Recursion may happen, so remove this level.
	vbs_err.pop_filename();
	}

void
setup_st_node::operator()(st_net *p) const
	{
	// Nothing to check.
	}

void
setup_st_node::operator()(st_function *p) const
	{
	// We do not need to setup the declaration and statement more than once.
	if (p->_done_setup != false)
		return;

	// Increment the scope for local data.
	int thisscope = symboltable.scope()++;
	scopelist_type localscope;

	// The scope for statements are:  global scope, parent scope, and
	// local scope.  Parent scope is needed for things like function
	// enables within the same module.
	localscope.push(0); // Global scope.
	localscope.push(*(--(--_scope.end()))); // Module scope.
	localscope.push(thisscope); // Local scope.

	// Must setup declarations first, because the statement might
	// need variables declared locally.
	if (p->_declaration != 0)
		{
		func_type::decl_list::iterator itp(p->_declaration->begin());
		func_type::decl_list::iterator stop(p->_declaration->end());
		func_type::io_list *iov = new func_type::io_list;
		for (; itp != stop; ++itp)
			(*itp)->setup(setup_tfdecl(localscope, iov));
		// We no longer need this object.  But don't delete it,
		// it was not a copy.
		p->_declaration = 0;
		if (iov->size() != 0)
			p->_iovariable = iov;
		else
			delete iov;
		}

	// The name of the function is the return value, so must setup
	// statement after entering the function name into the symbol
	// table.
	p->_stmt->setup(setup_stmt(localscope, _parent));

	// Update flag so we do setup only once.
	p->_done_setup = true;
	}

void
setup_st_node::operator()(st_task *p) const
	{
	// We do not need to setup the declaration and statement more than once.
	if (p->_done_setup != false)
		return;

	// Increment the scope for local data.
	int thisscope = symboltable.scope()++;
	scopelist_type localscope;

	// The scope for tasks are:  global scope, parent scope, and
	// local scope.  Parent scope is needed for things like task
	// enables within the same module.
	localscope.push(0); // Global scope.
	localscope.push(*(--(--_scope.end()))); // Module scope.
	localscope.push(thisscope); // Local scope.

	// Must setup declarations first, because the statement might
	// need variables declared locally.
	if (p->_declaration != 0)
		{
		task_type::decl_list::iterator itp(p->_declaration->begin());
		task_type::decl_list::iterator stop(p->_declaration->end());
		task_type::io_list *iov = new task_type::io_list;
		for (; itp != stop; ++itp)
			(*itp)->setup(setup_tfdecl(localscope, iov));
		// We no longer need this object.  But don't delete it,
		// it was not a copy.
		p->_declaration = 0;
		if (iov->size() != 0)
			p->_iovariable = iov;
		else
			delete iov;
		}

	// Setup statement.
	p->_stmt->setup(setup_stmt(localscope, _parent));

	// Update flag so we do setup only once.
	p->_done_setup = true;
	}

void
setup_st_node::operator()(st_instantiation *) const
	{
	// Nothing to do.
	}
