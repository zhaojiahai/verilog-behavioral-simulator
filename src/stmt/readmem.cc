// Verilog Behavioral Simulator
// Copyright (C) 2001 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// readmem.cc

#include "stmt/readmem.h"

systask_readmemb::systask_readmemb()
	: _name("$readmemb")
	{}

systask_readmemb::ostream_type &
systask_readmemb::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_readmemb::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_readmemb::trigger(const trigger_type &trigger)
	{ return trigger(this); }

systask_readmemh::systask_readmemh()
	: _name("$readmemh")
	{}

systask_readmemh::ostream_type &
systask_readmemh::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_readmemh::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_readmemh::trigger(const trigger_type &trigger)
	{ return trigger(this); }
