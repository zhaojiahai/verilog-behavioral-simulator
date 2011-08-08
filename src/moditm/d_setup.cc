// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_setup.cc

#include "common/error.h"
#include "common/event.h"
#include "common/sym_tab.h"
#include "common/st_net.h"
#include "expr/rangeid.h"
#include "expr/emon.h"
#include "expr/eeval.h"
#include "expr/esetup.h"
#include "misc/lvalue.h"
#include "misc/msetup.h"
#include "stmt/assign.h"
#include "stmt/ssetup.h"
#include "stmt/strigger.h"
#include "moditm/d_io.h"
#include "moditm/d_net.h"
#include "moditm/d_reg.h"
#include "moditm/d_int.h"
#include "moditm/d_param.h"
#include "moditm/d_setup.h"
#include "vbs.h"


inline void
decl_setup::io_setup(ident_list *il, range_type *r,
		iodirection_type iod, bool must_exist) const
	{
	// Setup a local scope stack for the local variable setup.
	scopelist_type localscope;
	localscope.push(_scope.top());

	// Now go through the list of ids to be declared.
	symbol_table &symboltable = vbs_engine::symboltable();
	ident_list::iterator itp(il->begin());
	ident_list::iterator stop(il->end());
	for (; itp != stop; ++itp)
		{
		hash_type hv(st_node_find((*itp)->name().c_str(), localscope));
		net_type *net = 0;
		if (hv._scope < 0)
			{
			if (must_exist)
				{
				// If the net doesn't exist, we can't change its direction
				vbs_err.set_data(vbs_error::SE_STDEF, (*itp)->_lineno);
				vbs_err.out((*itp)->name());
				}
			else
				{
				// Does not exist, create one.
				net = new net_type((*itp)->name());
				net->scope(localscope.top());
				net->_lineno = (*itp)->_lineno;
				hv = symboltable.add(net);
				if (hv._scope < 0)
					{
					delete net;
					vbs_err.set_data((vbs_error::value_type) hv._value,
						(*itp)->_lineno);
					vbs_err.out((*itp)->name());
					}
				}
			}
		else
			{
			net = symboltable.get(hv)->get_net();
			if (net == 0)
				{
				vbs_err.set_data(vbs_error::SE_TYPE, (*itp)->_lineno);
				vbs_err.out((*itp)->name());
				}
			}

		// Check the net object for validity.
		if (net->_iodirection != net_type::UNDEF)
			{
			vbs_err.set_data(vbs_error::SE_IODECL, (*itp)->_lineno);
			vbs_err.out((*itp)->name());
			}
		else
			net->_iodirection = iod;

		// Create storage if not already done.
		if (net->_storage == 0)
			{
			range_type::position_type ms = 0, ls = 0;
			if (r != 0)
				{
				r->setup(setup_select(_scope));
				r->get_data(ms, ls);
				}
			net->_storage = new num_type(ms, ls);
			net->_result = new num_type(ms, ls);
			}

		// Save the hash value index for later testing.
		if (_iovariable != 0)
			_iovariable->push_back(hv);

		// Update line number.
		net->_lineno = (*itp)->_lineno;
		}
	}

void
decl_setup::operator()(io_decl *p, bool must_exist) const
	{
	switch (p->_type)
		{
		case io_decl::INPUT:
			io_setup(p->_id_list, p->_range, net_type::IN, must_exist);
			break;
		case io_decl::OUTPUT:
			io_setup(p->_id_list, p->_range, net_type::OUT, must_exist);
			break;
		case io_decl::INOUT:
			io_setup(p->_id_list, p->_range, net_type::INOUT, must_exist);
			break;
		default:
			break;
		}
	}

inline void
decl_setup::net_setup(ident_ptr &id, range_type *r, type t) const
	{
	// Find the symbol table node, if it exists.
	symbol_table &symboltable = vbs_engine::symboltable();
	scopelist_type localscope;
	localscope.push(_scope.top());
	hash_type hv(st_node_find(id->name().c_str(), localscope));
	net_type *net = 0;
	if (hv._scope < 0)
		{
		// Does not exist, create one.
		net = new net_type(id->name(), t);
		net->scope(_scope.top());
		net->_lineno = id->_lineno;
		hv = symboltable.add(net);
		if (hv._scope < 0)
			{
			delete net;
			vbs_err.set_data((vbs_error::value_type) hv._value, id->_lineno);
			vbs_err.out(id->name());
			}
		}
	else
		{
		net = symboltable.get(hv)->get_net();
		if (net == 0)
			{
			vbs_err.set_data(vbs_error::SE_TYPE, id->_lineno);
			vbs_err.out(id->name());
			}

		// Make sure this is the only declaration of this net.
		if (net->_type != net_type::PORT)
			{
			// Second time declaring this net, check it.
			vbs_error::value_type err = vbs_error::SE_NONE;
			switch (t)
				{
				case net_type::WIRE:
					err = vbs_error::SE_WIREDECL;
					break;
				case net_type::REGISTER:
					err = vbs_error::SE_REGDECL;
					break;
				case net_type::INTEGER:
					err = vbs_error::SE_INTDECL;
					break;
				case net_type::MEMORY:
					err = vbs_error::SE_MEMDECL;
					break;
				case net_type::PARAMETER:
					err = vbs_error::SE_PARAMDECL;
					break;
				default:
					break;
				}

			if (err != vbs_error::SE_NONE)
				{
				vbs_err.set_data(err, id->_lineno);
				vbs_err.out(id->name());
				}
			}
		else
			{
			// Save new type.
			net->_type = t;
			}
		}

	// Validate storage space, create it if not already done.
	if (net->_storage != 0)
		{
		// Storage already defined by IO declaration.
		range_type::position_type s_ms, s_ls, ms = 0, ls = 0;

		// The two bit selects must match.
		if (r != 0)
			{
			r->setup(setup_select(_scope));
			r->get_data(ms, ls);
			if (ms < 0 || ms > logic::cache_size
			 || ls < 0 || ls > logic::cache_size)
				{
				vbs_err.set_data(vbs_error::SE_SUPPORT, id->_lineno);
				vbs_err.out(id->name());
				}
			}
		else if (t == net_type::INTEGER)
			{
			ms = 31;
			ls = 0;
			}

		if (net->_storage->msb_lower())
			{
			s_ms = net->_storage->start_pos();
			s_ls = net->_storage->stop_pos();
			}
		else
			{
			s_ls = net->_storage->start_pos();
			s_ms = net->_storage->stop_pos();
			}

		if (ms != s_ms || ls != s_ls)
			{
			if (t == net_type::INTEGER)
				{
				// Direction declared first but the index order was different.
				delete net->_storage;
				delete net->_result;
				net->_storage = new num_type(ms, ls, true);
				net->_result = new num_type(ms, ls, true);
				}
			else
				{
				vbs_err.set_data(vbs_error::SE_IDXORDER, id->_lineno);
				vbs_err.out(id->name());
				}
			}
		}
	else
		{
		// Get the width of the net.
		range_type::position_type ms = 0, ls = 0;
		if (r != 0)
			{
			r->setup(setup_select(_scope));
			r->get_data(ms, ls);
			if (ms < 0 || ms > logic::cache_size
			 || ls < 0 || ls > logic::cache_size)
				{
				vbs_err.set_data(vbs_error::SE_SUPPORT, id->_lineno);
				vbs_err.out(id->name());
				}
			}
		else if (t == net_type::INTEGER || t == net_type::PARAMETER)
			{
			ms = 31;
			ls = 0;
			}
		// Check if this is a memory declaration.
		if (id->select() != 0)
			{
			net_type::position_type mem_ms = 0, mem_ls = 0;
			id->select()->setup(setup_select(_scope));
			id->get_select(mem_ms, mem_ls);
			if (mem_ms > mem_ls)
				{
				net->_upper_mem_idx = mem_ms;
				net->_lower_mem_idx = mem_ls;
				}
			else
				{
				net->_upper_mem_idx = mem_ls;
				net->_lower_mem_idx = mem_ms;
				}
			net->_storage = new num_type[net->_upper_mem_idx+1];
			for (int i = 0; i <= net->_upper_mem_idx; ++i)
				net->_storage[i] = num_type(ms, ls);
			net->_result = new num_type(ms, ls);
			net->_type = net_type::MEMORY;
			}
		else
			{
			if (t == net_type::WIRE)
				net->_storage = new num_type(ms, ls, false, Z);
			else if (t == net_type::INTEGER)
				net->_storage = new num_type(ms, ls, true);
			else
				net->_storage = new num_type(ms, ls);
			net->_result = new num_type(ms, ls);
			}
		}

	// Update line number.
	net->_lineno = id->_lineno;
	}

void
decl_setup::operator()(net_decl *p) const
	{
	// Do we support this declaration type?
	if (p->_type != net_decl::WIRE)
		{
		vbs_err.set_data(vbs_error::SE_SUPPORT, p->_lineno);
		vbs_err.out("net type");
		}

	// Go through each variable, and set it up.
	symbol_table &symboltable = vbs_engine::symboltable();
	event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
	net_decl::decl_assign_list::const_iterator itp(p->_id_list->begin());
	net_decl::decl_assign_list::const_iterator stop(p->_id_list->end());
	event_cache_type *cache = 0;
	event_type *ev = 0;
	lvalue *lval = 0;
	expr_type *expr = 0;
	assignment_stmt *st = 0;
	for (; itp != stop; ++itp)
		{
		// Add identifier to symbol table.
		net_setup((*itp)->_ident, p->_range, net_type::WIRE);

		// Check for new assign style declaration.
		if ((*itp)->_rval != 0)
			{
			// Create new assignment statement.
			lvalue::expr_list *elst = new lvalue::expr_list;
			lvalue::ident_type *id = (*itp)->_ident->copy_constructor();
			elst->push_back(id);
			lval = new lvalue(elst);
			expr = (*itp)->_rval->copy_constructor();
			st = new assignment_stmt(lval, expr, false);
			st->setup(setup_stmt(_scope, st));

			// Avoid memory leak by keeping track of allocated statement.
			_stmtlist->push_back(basic_ptr<stmt_base>(st));

			// Treat assignment as continuous assignment.
			cache = new event_cache_type(true, st);
			ev = new change_event<stmt_type>(cache, DC);
			counted_ptr< event_base<stmt_type> > e(ev);
			expr->monitor(monitor_expr(&e));

			// Set initial value.
			net_type *net = (symboltable.get(id->index()))->get_net();
			if (net != 0)
				{
				const num_type &num(expr->evaluate(evaluate_expr()));
				net->assignment(num, -1, -1, -1, -1);
				}

			// Schedule assignment at least once.
			eventqueue.add_event(ev);
			}
		}
	}

void
decl_setup::reset()
	{
	delete _stmtlist;
	_stmtlist = new stmt_list;
	}

decl_setup::stmt_list *decl_setup::_stmtlist;

void
decl_setup::operator()(reg_decl *p) const
	{
	// Go through each variable, and set it up.
	symbol_table &symboltable = vbs_engine::symboltable();
	reg_decl::decl_assign_list::iterator itp(p->_id_list->begin());
	reg_decl::decl_assign_list::iterator stop(p->_id_list->end());
	for (; itp != stop; ++itp)
		{
		net_setup((*itp)->_ident, p->_range, net_type::REGISTER);

		// Setup expression.
		if ((*itp)->_rval != 0)
			{
			// Assign value into symbol table.
			net_type *net;
			(*itp)->_ident->setup(setup_expr(_scope));
			(*itp)->_rval->setup(setup_expr(_scope, true));
			net = (symboltable.get((*itp)->_ident->index()))->get_net();
			if (net != 0)
				{
				const num_type &num((*itp)->_rval->evaluate(evaluate_expr()));
				net->assignment(num, -1, -1, -1, -1);
				}
			}
		}
	}

void
decl_setup::operator()(int_decl *p) const
	{
	// Go through each variable, and set it up.
	symbol_table &symboltable = vbs_engine::symboltable();
	int_decl::decl_assign_list::iterator itp(p->_id_list->begin());
	int_decl::decl_assign_list::iterator stop(p->_id_list->end());
	for (; itp != stop; ++itp)
		{
		net_setup((*itp)->_ident, 0, net_type::INTEGER);

		// Setup expression.
		if ((*itp)->_rval != 0)
			{
			// Assign value into symbol table.
			net_type *net;
			(*itp)->_ident->setup(setup_expr(_scope));
			(*itp)->_rval->setup(setup_expr(_scope, true));
			net = (symboltable.get((*itp)->_ident->index()))->get_net();
			if (net != 0)
				{
				const num_type &num((*itp)->_rval->evaluate(evaluate_expr()));
				net->assignment(num, -1, -1, -1, -1);
				}
			}
		}
	}

void
decl_setup::operator()(param_decl *p) const
	{
	// Go through each variable, and set it up.
	symbol_table &symboltable = vbs_engine::symboltable();
	param_decl::decl_assign_list::const_iterator itp(p->_id_list->begin());
	param_decl::decl_assign_list::const_iterator stop(p->_id_list->end());
	for (; itp != stop; ++itp)
		{
		// Add identifier to symbol table.
		net_setup((*itp)->_ident, p->_range, net_type::PARAMETER);

		// Setup expression.
		if ((*itp)->_rval != 0)
			{
			// Assign value into symbol table.
			net_type *net;
			(*itp)->_ident->setup(setup_expr(_scope));
			(*itp)->_rval->setup(setup_expr(_scope, true));
			net = (symboltable.get((*itp)->_ident->index()))->get_net();
			if (_first != 0 && *_first != *_last)
				{
				if (net != 0)
					{
					const num_type &num((*(*_first))->evaluate(evaluate_expr()));
					net->assignment(num, -1, -1, -1, -1);
					}
				++(*_first);
				}
			else
				{
				// Use provided default.
				if (net != 0)
					{
					const num_type &num((*itp)->_rval->evaluate(evaluate_expr()));
					net->assignment(num, -1, -1, -1, -1);
					}
				}
			}
		}
	}
