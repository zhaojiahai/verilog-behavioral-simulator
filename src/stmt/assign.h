// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// nbassign.h

#ifndef _ASSIGN_H
#define _ASSIGN_H

#include "common/event.h"
#include "expr/exprbase.h"
#include "stmt/stmtbase.h"
#include "misc/lvalue.h"
#include "misc/decbase.h"

class assignment_stmt : public stmt_base
	{
	friend struct setup_module_item;
	friend struct setup_stmt;
	friend struct trigger_stmt;
	friend struct write_stmt;

public:
	typedef lvalue lvalue_type;
	typedef lvalue_type::position_type position_type;
	typedef expr_base expr_type;
	typedef expr_type::num_type num_type;
	typedef stmt_base stmt_type;
	typedef dec_base dec_type;

	assignment_stmt(lvalue_type *, expr_type *, bool nb, dec_type * = 0);
	assignment_stmt(const assignment_stmt &);
	virtual ~assignment_stmt();

	assignment_stmt *get_assign();
	assignment_stmt *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	lvalue_type *_lval;
	expr_type *_rval;
	dec_type *_delayed_store;

	// Non-blocking assignment needs an event.
	typedef event_base<stmt_type> event_type;
	bool _nonblocking;
	num_type *_value;
	event_type *_event;
	};

#endif // _ASSIGN_H
