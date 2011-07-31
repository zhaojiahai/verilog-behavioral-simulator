// Verilog Behavioral Simulator
// Copyright (C) 1997 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// strobe.cc

#include "stmt/strobe.h"

systask_strobe::systask_strobe()
	: _name("$strobe")
	{}

systask_strobe::ostream_type &
systask_strobe::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_strobe::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_strobe::trigger(const trigger_type &trigger)
	{ return trigger(this); }
