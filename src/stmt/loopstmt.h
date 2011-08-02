// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002-2003,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// loopstmt.h

#ifndef _LOOPSTMT_H
#define _LOOPSTMT_H

#include "expr/exprbase.h"
#include "stmt/stmtbase.h"

class loop_stmt : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;
	friend struct write_stmt;

public:
	typedef expr_base expr_type;
	typedef stmt_base stmt_type;
	typedef assignment_stmt assign_type;

	enum loop_type
		{
		FOREVER,
		REPEAT,
		WHILE,
		FOR
		};

	loop_stmt(loop_type, stmt_type *,
		expr_type * = 0, assign_type * = 0, assign_type * = 0);
	loop_stmt(const loop_stmt &);
	virtual ~loop_stmt();

	bool delayed() const;
	loop_stmt *get_loop();
	loop_stmt *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	bool _delayed; // For delays in loops.
	unsigned long _repeat_count; // For triggering statement.
	loop_type _type;
	expr_type *_expr;
	stmt_type *_stmt;
	assign_type *_preassign;
	assign_type *_postassign;
	};

#endif // _LOOPSTMT_H
