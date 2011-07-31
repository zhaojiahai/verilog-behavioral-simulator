// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2003 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// ifelse.cc

#include "expr/exprbase.h"
#include "stmt/ifelse.h"

if_else_stmt::if_else_stmt(expr_type *e, stmt_type *t, stmt_type *f)
	{
	_expr = e;
	_true_stmt = t;
	_false_stmt = f;
	}

if_else_stmt::if_else_stmt(const if_else_stmt &p)
	: common_base(p), stmt_base(p)
	{
	_expr = p._expr->copy_constructor();
	_true_stmt = p._true_stmt->copy_constructor();
	if (p._false_stmt != 0)
		_false_stmt = p._false_stmt->copy_constructor();
	else
		_false_stmt = 0;
	}

if_else_stmt::~if_else_stmt()
	{
	delete _expr;
	delete _true_stmt;
	if (_false_stmt != 0)
		delete _false_stmt;
	}

if_else_stmt *
if_else_stmt::copy_constructor() const
	{ return new if_else_stmt(*this); }

if_else_stmt::ostream_type &
if_else_stmt::display(ostream_type &s) const
	{
	stmt_base::display(s); // Delay or event control...
	s << "if (" << *_expr << ")" << endl;
	s << *_true_stmt;
	if (_false_stmt != 0)
		{
		s << "else" << endl;
		s << *_false_stmt;
		}
	return s;
	}

void
if_else_stmt::write(const write_type &wr) const
	{ wr(this); }

void
if_else_stmt::setup(const setup_type &setup)
	{ setup(this); }

bool
if_else_stmt::trigger(const trigger_type &trigger)
	{ return trigger(this); }
