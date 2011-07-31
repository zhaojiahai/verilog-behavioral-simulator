// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// port.cc

#include "misc/port.h"

port::port(const char *n)
	: _name(n)
	{}

port::ostream_type &
port::display(ostream_type &s) const
	{ s << _name; return s; }

port *
port::read(const read_type &rd)
	{ return rd(); }

void
port::write(const write_type &wr) const
	{ wr(this); }

void
port::setup(const setup_type &setup)
	{ setup(this); }

port::ostream_type &
operator<<(port::ostream_type &s, const port &p)
	{ return p.display(s); }
