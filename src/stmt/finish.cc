// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// finish.cc

#include "stmt/finish.h"

systask_finish::systask_finish()
	: _name("$finish")
	{}

systask_finish::ostream_type &
systask_finish::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_finish::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_finish::trigger(const trigger_type &trigger)
	{ return trigger(this); }
