// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// xtime.cc

#include "stmt/xtime.h"

sysfunc_time::sysfunc_time()
	: _name("$time")
	{}

sysfunc_time::ostream_type &
sysfunc_time::display(ostream_type &s) const
	{ s << _name; return s; }

void
sysfunc_time::setup(const setup_type &setup)
	{ setup(this); }

bool
sysfunc_time::trigger(const trigger_type &trigger)
	{ return trigger(this); }
