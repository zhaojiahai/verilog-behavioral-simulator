// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// msetup.cc

#include "common/hash.h"
#include "common/error.h"
#include "common/sym_tab.h"
#include "common/st_mod.h"
#include "common/st_net.h"
#include "common/st_func.h"
#include "expr/number.h"
#include "expr/rangeid.h"
#include "expr/eeval.h"
#include "expr/esetup.h"
#include "misc/bitsel.h"
#include "misc/partsel.h"
#include "misc/msetup.h"

// For port setup.
#include "common/event.h"
#include "expr/emon.h"
#include "stmt/stmtbase.h"
#include "stmt/assign.h"

#include "vbs.h"

// sim_parsed_module

void
setup_module::operator()(module *p) const
	{
	// Setting up a module means storing the port list and module items.
	// This is all we do because we are not really in setup mode yet.
	_stmod->_port_list = p->_port_list;
	_stmod->_module_item = p->_module_item;
	// Save the filename from where this module was read.
	_stmod->_filename = p->_filename;
	// Steal the lists.
	p->_port_list = 0;
	p->_module_item = 0;
	}


void
setup_port::operator()(port *p) const
	{
	// If port has a port connection, setup the connection...
	symbol_table &symboltable = vbs_engine::symboltable();
	if (_port_conn->_expr != 0)
		{
		ident_type *id;
		net_type *net = symboltable.get(p->_index)->get_net();

		// For output type ports, update the index with the hash
		// value of the port connection so we can indicate a change
		// to the parent module.
		if (net->_iodirection == net_type::OUT
		 || net->_iodirection == net_type::INOUT)
			{
 			id = _port_conn->_expr->get_range_id();
			if (id != 0)
				{
				// Port index points to the index of the port connected to.
				net->_port_index = new net_type::hash_type(id->index());
				id->get_select(net->_port_ms, net->_port_ls);
				}
			else
				{
				vbs_err.set_data(vbs_error::SE_SUPPORT, p->_lineno);
				vbs_err.out("non-identifier in port assignment");
				}
			}

		// For input type ports, setup events to monitor changes in
		// the port connections.  <net> should not be null, setup of
		// module instance should have created it.
		if (net->_iodirection == net_type::IN
		 || net->_iodirection == net_type::INOUT)
			{
			// Create an assignment statement for event.
			lvalue::expr_list *elst = new lvalue::expr_list;
			id = new range_id(p->_name.c_str());
			elst->push_back(id);
			lvalue *lval = new lvalue(elst);
			lval->setup(setup_lvalue(_scope));
			// Do not setup this expression.  It will modify the scope.
			// The expression in _port_conn should already be setup
			// and have the correct index.
			expr_base *exp = _port_conn->_expr->copy_constructor();
			stmt_base *st = new assignment_stmt(lval, exp, false);
			_stmtlist->push_back(basic_ptr<stmt_base>(st));

			// Create event for monitoring expressions.
			event_base<stmt_base>::event_container *cache =
				new event_base<stmt_base>::event_container(true, st);
			event_base<stmt_base> *ev =
				new change_event<stmt_base>(cache, number(0, 0));
			counted_ptr< event_base<stmt_base> > e(ev);
			exp->monitor(monitor_expr(&e));
			}
		}
	}

void
setup_port::reset()
	{
	delete _stmtlist;
	_stmtlist = new std::list<basic_ptr<stmt_base> >;
	}

std::list<basic_ptr<stmt_base> > *setup_port::_stmtlist;


bool
setup_port_conn::operator()(port_connection *p) const
	{
	/* Port connections can be empty. */
	if (p->_expr != 0)
		{
		p->_expr->setup(setup_expr(_scope));
		return true;
		}
	return false;
	}


void
setup_select::operator()(bit_select *p) const
	{ p->_expr->setup(setup_expr(_scope)); }

void
setup_select::operator()(part_select *p) const
	{
	p->_le->setup(setup_expr(_scope, true));
	p->_re->setup(setup_expr(_scope, true));
	// Must be constant, so optimize.
	p->_ln = static_cast<unsigned long>(p->_le->evaluate(evaluate_expr()));
	p->_rn = static_cast<unsigned long>(p->_re->evaluate(evaluate_expr()));
	if (static_cast<signed long>(p->_ln) < 0)
		{
		part_select::strstream_type buf;
		buf << *p;
		vbs_err.set_data(vbs_error::SE_SUPPORT, p->_lineno);
		vbs_err.out(buf);
		}
	if (static_cast<signed long>(p->_rn) < 0)
		{
		part_select::strstream_type buf;
		buf << *p;
		vbs_err.set_data(vbs_error::SE_SUPPORT, p->_lineno);
		vbs_err.out(buf);
		}
	}


// Lvalue for blocking assignment.

setup_lvalue::size_type
setup_lvalue::operator()(lvalue *l) const
	{
	symbol_table &symboltable = vbs_engine::symboltable();
	lvalue::expr_list::reverse_iterator itp(l->_exp_list->rbegin());
	lvalue::expr_list::reverse_iterator stop(l->_exp_list->rend());
	size_type node_size = 0, size = 0, prev_size = 0;
	node_type *node;

	for (; itp != stop; ++itp)
		{
		// Setup of range_id for index of variable.
		(*itp)->setup(setup_expr(_scope));

		// Lvalues are wires, registers, and function names.
		lvalue::ident_type *rid = (*itp)->get_range_id();
		if (rid == 0)
			{
			strstream_type buf;
			buf << *l;
			vbs_err.set_data(vbs_error::SE_NLVAL, l->_lineno);
			vbs_err.out(buf);
			}
		else
			{
			lvalue::position_type ms, ls;
			lvalue::select_size_type sel_size;
			sel_size = rid->get_select(ms, ls);
			if (sel_size == lvalue::select_type::SELECT_PART)
				node_size = ((ms >= ls ) ? (ms - ls) : (ls - ms)) + 1;
			else
				{
				// Setup for range_id should have tested for valid index.
				// So no need check here.
				node = symboltable.get(rid->index());
				st_net *net = node->get_net();
				if (net != 0)
					{
					if (sel_size == lvalue::select_type::SELECT_BIT
					 && net->depth() == 0)
						node_size = 1;
					else
						node_size = net->size();
					}
				else
					{
					st_function *func = node->get_function();
					if (func != 0)
						{
						if (sel_size == lvalue::select_type::SELECT_BIT)
							node_size = 1;
						else
							node_size = func->size();
						}
					else
						{
						strstream_type buf;
						buf << *l << '<' << node->type_name() << '>';
						vbs_err.set_data(vbs_error::SE_NLVAL, l->_lineno);
						vbs_err.out(buf);
						}
					}
				}
			}
		size += node_size;

		if (l->_exp_list->size() < 2)
			{
			// If only a single element, no need break up the rvalue.
			l->_elmt_list.push_front(lvalue::element_type(-1, -1));
			}
		else
			{
			l->_elmt_list.push_front(lvalue::element_type(size-1, prev_size));
			prev_size = size;
			}
		}
	return size;
	}
