// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// delayid.cc

#include "misc/delayid.h"

delay_id::delay_id(expr_type *e)
	{
	_expr = e;
	_event = 0;
	}

delay_id::delay_id(const delay_id &p)
	: common_base(p), dec_base(p)
	{
	// We should not be making copies of delay_id after setup.
	// Thus _event should be NULL in p anyway.
	_expr = p._expr->copy_constructor();
	_event = 0;
	}

delay_id::~delay_id()
	{
	delete _event;
	}

delay_id *
delay_id::copy_constructor() const
	{ return new delay_id(*this); }

delay_id::ostream_type &
delay_id::display(ostream_type &s) const
	{
	s << "#(";
	_expr->display(s);
	s << ") ";
	return s;
	}

void
delay_id::write(const write_type &wr) const
	{ wr(this); }

void
delay_id::setup(const setup_type &setup)
	{ setup(this); }

int
delay_id::trigger(const trigger_type &trigger)
	{ return trigger(this); }
