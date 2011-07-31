// Verilog Behavioral Simulator
// Copyright (C) 2003 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// xtime.cc

#include "stmt/stime.h"

sysfunc_stime::sysfunc_stime()
	: _name("$stime")
	{}

sysfunc_stime::ostream_type &
sysfunc_stime::display(ostream_type &s) const
	{ s << _name; return s; }

void
sysfunc_stime::setup(const setup_type &setup)
	{ setup(this); }

bool
sysfunc_stime::trigger(const trigger_type &trigger)
	{ return trigger(this); }
