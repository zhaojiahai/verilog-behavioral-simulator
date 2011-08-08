// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// nbassign.cc

#include "stmt/assign.h"

assignment_stmt::assignment_stmt(lvalue_type *l, expr_type *e, bool nb,
								 dec_type *d)
	{
	_value = 0;
	_lval = l;
	_rval = e;
	_nonblocking = nb;
	_delayed_store = d;
	}

assignment_stmt::assignment_stmt(const assignment_stmt &p)
	: common_base(p), stmt_base(p)
	{
	_value = 0;
	_lval = new lvalue(*p._lval);
	_rval = p._rval->copy_constructor();
	_nonblocking = p._nonblocking;
	if (p._delayed_store != 0)
		_delayed_store = p._delayed_store->copy_constructor();
	else
		_delayed_store = 0;
	}

assignment_stmt::~assignment_stmt()
	{
	delete _value;
	delete _lval;
	delete _rval;
	if (_delayed_store != 0)
		delete _delayed_store;
	}

assignment_stmt *
assignment_stmt::get_assign()
	{ return this; }

assignment_stmt *
assignment_stmt::copy_constructor() const
	{ return new assignment_stmt(*this); }

assignment_stmt::ostream_type &
assignment_stmt::display(ostream_type &s) const
	{
	stmt_base::display(s); // Delay or event control...
	s << *_lval;
	if (_nonblocking)
		s << " <= ";
	else
		s << " = ";
	if (_delayed_store != 0)
		s << *(_delayed_store);
	s << *_rval << ';' << endl;
	return s;
	}

void
assignment_stmt::write(const write_type &wr) const
	{ wr(this); }

void
assignment_stmt::setup(const setup_type &setup)
	{ setup(this); }

bool
assignment_stmt::trigger(const trigger_type &trigger)
	{ return trigger(this); }
