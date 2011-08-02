// Verilog Behavioral Simulator
// Copyright (C) 2000 George Varughese
//
// With modifications from:
// Jimen Ching <jimen.ching@gmail.com>
// 
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dumptsk.cc

#include "stmt/dumptsk.h"

systask_dumpon::systask_dumpon()
	: _name("$dumpon")
	{}

systask_dumpon::ostream_type &
systask_dumpon::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_dumpon::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_dumpon::trigger(const trigger_type &trigger)
	{ return trigger(this); }


// dumpoff

systask_dumpoff::systask_dumpoff()
	: _name("$dumpoff")
	{}

systask_dumpoff::ostream_type &
systask_dumpoff::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_dumpoff::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_dumpoff::trigger(const trigger_type &trigger)
	{ return trigger(this); }


// dumpall

systask_dumpall::systask_dumpall()
	: _name("$dumpall")
	{}

systask_dumpall::ostream_type &
systask_dumpall::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_dumpall::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_dumpall::trigger(const trigger_type &trigger)
	{ return trigger(this); }


// dumpvars

systask_dumpvars::systask_dumpvars()
	: _name("$dumpvars")
	{}

systask_dumpvars::ostream_type &
systask_dumpvars::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_dumpvars::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_dumpvars::trigger(const trigger_type &trigger)
	{ return trigger(this); }


// dumpfile

systask_dumpfile::systask_dumpfile()
	: _name("$dumpfile")
	{}

systask_dumpfile::ostream_type &
systask_dumpfile::display(ostream_type &s) const
	{ s << _name; return s; }

void
systask_dumpfile::setup(const setup_type &setup)
	{ setup(this); }

bool
systask_dumpfile::trigger(const trigger_type &trigger)
	{ return trigger(this); }
