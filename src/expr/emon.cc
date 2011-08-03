// Verilog Behavioral Simulator
// Copyright (C) 1996-1997,2001,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// emon.cc

#include "common/sym_tab.h"
#include "common/st_net.h"
#include "common/st_func.h"
#include "common/st_mon.h"
#include "expr/funccall.h"
#include "expr/concat.h"
#include "expr/number.h"
#include "expr/qstr.h"
#include "expr/rangeid.h"
#include "expr/unary_op.h"
#include "expr/binary_op.h"
#include "expr/ternary_op.h"
#include "expr/mintypmax.h"
#include "expr/eeval.h"
#include "expr/emon.h"
#include "misc/mmon.h"
#include "vbs.h"


bool
monitor_expr::operator()(const function_call *p) const
	{
	bool success = true;
	symbol_table &symboltable = vbs_engine::symboltable();
	st_function *func = symboltable.get(p->_index)->get_function();
	if (func != 0)
		{
		if (p->_argument != 0)
			{
			function_call::arg_list::iterator itp(p->_argument->begin());
			function_call::arg_list::iterator stop(p->_argument->end());
			for (; itp != stop; ++itp)
				{
				success = (*itp)->monitor(monitor_expr(_event));
				if (success == false)
					break;
				}
			}
		if (success == true)
			success = func->monitor(monitor_st_node(_event));
		}
	else
		success = false;
	return success;
	}

bool
monitor_expr::operator()(const concatenation *p) const
	{
	concatenation::expr_list::iterator itp(p->_exp_list->begin());
	concatenation::expr_list::iterator stop(p->_exp_list->end());
	for (; itp != stop; ++itp)
		{
		if ((*itp)->monitor(monitor_expr(_event)) == false)
			return false;
		}
	return true;
	}

bool
monitor_expr::operator()(const number *) const
	{
	// Create static list of event pointers because the _event passed in
	// is from the stack.  We need a valid pointer until the event is
	// executed.  This is kind of a waste of memory, but what can we do?
	// FIXME!
	static event_ptr_list ev_list;

	if (!_via_select)
		{
		if (_event != 0 && !_event->get()->is_queued())
			{
			// Something is monitoring a constant, so put an event on the
			// event queue and trigger it at the end of time unit 0.
			event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
			ev_list.push_back(*_event);
			eventqueue.add_event(_event->get());
			}
		}
	return true;
	}

bool
monitor_expr::operator()(const qouted_str *) const
	{
	// Create static list of event pointers because the _event passed in
	// is from the stack.  We need a valid pointer until the event is
	// executed.  This is kind of a waste of memory, but what can we do?
	// FIXME!
	static event_ptr_list ev_list;

	if (!_via_select)
		{
		if (_event != 0 && !_event->get()->is_queued())
			{
			// Something is monitoring a constant, so put an event on the
			// event queue and trigger it at the end of time unit 0.
			event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
			ev_list.push_back(*_event);
			eventqueue.add_event(_event->get());
			}
		}
	return true;
	}

bool
monitor_expr::operator()(const range_id *p) const
	{
	bool success;
	symbol_table &symboltable = vbs_engine::symboltable();
	st_net *node = symboltable.get(p->_index)->get_net();
	if (node != 0)
		{
		success = node->monitor(monitor_st_node(_event));
		if (success && p->_select != 0)
			success = p->_select->monitor(monitor_select(_event));
		}
	else
		success = false;

	return success;
	}

bool
monitor_expr::operator()(const unary_op_expr *p) const
	{ return p->_expr->monitor(monitor_expr(_event)); }

bool
monitor_expr::operator()(const binary_op_expr *p) const
	{
	return (p->_left->monitor(monitor_expr(_event))
	     && p->_right->monitor(monitor_expr(_event)));
	}

bool
monitor_expr::operator()(const ternary_op_expr *p) const
	{
	return (p->_expr->monitor(monitor_expr(_event))
		&& p->_true_expr->monitor(monitor_expr(_event))
		&& p->_false_expr->monitor(monitor_expr(_event)));
	}

bool
monitor_expr::operator()(const mintypmax_expr *p) const
	{
	switch (mintypmax_expr::_delay)
		{
		case mintypmax_expr::MINDELAY:
			if (p->_min_expr != 0)
				return p->_min_expr->monitor(monitor_expr(_event));
			break;
		case mintypmax_expr::TYPDELAY:
			if (p->_typ_expr != 0)
				return p->_typ_expr->monitor(monitor_expr(_event));
			break;
		case mintypmax_expr::MAXDELAY:
			if (p->_max_expr != 0)
				return p->_max_expr->monitor(monitor_expr(_event));
			break;
		default:
			break;
		}
	// Non mintypmax expression.
	return p->_min_expr->monitor(monitor_expr(_event));
	}
