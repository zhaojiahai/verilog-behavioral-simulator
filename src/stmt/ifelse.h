// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// ifelse.h

#ifndef _IFELSE_H
#define _IFELSE_H

#include "expr/exprbase.h"
#include "stmt/stmtbase.h"

class if_else_stmt : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;
	friend struct write_stmt;

public:
	typedef expr_base expr_type;
	typedef stmt_base stmt_type;

	if_else_stmt(expr_type *, stmt_type *, stmt_type * = 0);
	if_else_stmt(const if_else_stmt &);
	virtual ~if_else_stmt();

	if_else_stmt *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	expr_type *_expr;
	stmt_type *_true_stmt;
	stmt_type *_false_stmt;
	};

#endif // _IFELSE_H
