// Verilog Behavioral Simulator
// Copyright (C) 1996-1997,2001,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// emon.h

#ifndef _EMON_H
#define _EMON_H

#include "common/ptr.h"
#include "common/event.h"
#include "expr/exprbase.h"
#include "stmt/stmtbase.h"

struct monitor_expr : public expr_monitor
	{
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;
	typedef counted_ptr<event_type> event_ptr;
	typedef stmt_type::list_type<event_ptr> event_ptr_list;

	monitor_expr(event_ptr *e, bool b = false)
		: _via_select(b)
		{ _event = e; }

	static void reset();

	bool operator()(const function_call *) const;
	bool operator()(const concatenation *) const;
	bool operator()(const number *) const;
	bool operator()(const qouted_str *) const;
	bool operator()(const range_id *) const;
	bool operator()(const unary_op_expr *) const;
	bool operator()(const binary_op_expr *) const;
	bool operator()(const ternary_op_expr *) const;
	bool operator()(const mintypmax_expr *) const;

	bool _via_select;
	event_ptr *_event;

	// Used by number/qouted_str to cache event from stack.
	static event_ptr_list *_eventlist;
	};

#endif // _EMON_H
