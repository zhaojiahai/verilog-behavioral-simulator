// Verilog Behavioral Simulator
// Copyright (C) 1996-1997 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// monitor.cc

#include "stmt/monitor.h"

systask_monitor::systask_monitor()
	: _name("$monitor")
	{}

systask_monitor::ostream_type &
systask_monitor::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_monitor::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_monitor::trigger(const trigger_type &trigger)
	{ return trigger(this); }
