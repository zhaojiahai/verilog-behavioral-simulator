// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// write.cc

#include "stmt/write.h"

systask_write::systask_write()
	: _name("$write")
	{}

systask_write::ostream_type &
systask_write::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_write::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_write::trigger(const trigger_type &trigger)
	{ return trigger(this); }
