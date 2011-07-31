// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002-2003 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// loopstmt.cc

#include "expr/exprbase.h"
#include "stmt/assign.h"
#include "stmt/loopstmt.h"

loop_stmt::loop_stmt(loop_type t, stmt_type *s,
		expr_type *e, assign_type *pre, assign_type *post)
	: _delayed(false), _type(t)
	{
	_stmt = s;
	_expr = e;
	_preassign = pre;
	_postassign = post;
	}

loop_stmt::loop_stmt(const loop_stmt &p)
	: common_base(p), stmt_base(p), _delayed(p._delayed), _type(p._type)
	{
	_stmt = p._stmt->copy_constructor();
	_expr = 0;
	if (p._expr != 0)
		_expr = p._expr->copy_constructor();
	_preassign = _postassign = 0;
	if (p._preassign != 0)
		_preassign = p._preassign->copy_constructor();
	if (p._postassign != 0)
		_postassign = p._postassign->copy_constructor();
	}

loop_stmt::~loop_stmt()
	{
	delete _stmt;
	if (_expr != 0)
		delete _expr;
	if (_preassign != 0)
		delete _preassign;
	if (_postassign != 0)
		delete _postassign;
	}

bool
loop_stmt::delayed() const
	{ return _delayed; }

loop_stmt *
loop_stmt::get_loop()
	{ return this; }

loop_stmt *
loop_stmt::copy_constructor() const
	{ return new loop_stmt(*this); }

loop_stmt::ostream_type &
loop_stmt::display(ostream_type &s) const
	{
	stmt_base::display(s); // Delay or event control...
	switch (_type)
		{
		case FOREVER:
			s << "forever" << endl;
			break;
		case REPEAT:
			s << "repeat (" << *_expr << ')' << endl;
			break;
		case WHILE:
			s << "while (" << *_expr << ')' << endl;
			break;
		case FOR:
			s << "for (" << *_preassign;
			s << *_expr << "; " << endl;
			s << *_postassign << ")" << endl;
			break;
		default:
			return s;
		}
	s << *_stmt;
	return s;
	}

void
loop_stmt::write(const write_type &wr) const
	{ wr(this); }

void
loop_stmt::setup(const setup_type &setup)
	{ setup(this); }

bool
loop_stmt::trigger(const trigger_type &trigger)
	{ return trigger(this); }
