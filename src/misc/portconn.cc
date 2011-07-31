// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// portconn.cc

#include "misc/portconn.h"

port_connection::port_connection(expr_type *e)
	{ _expr = e; }

port_connection::~port_connection()
	{ delete _expr; }

port_connection::ostream_type &
port_connection::display(ostream_type &s) const
	{
	if (_expr != 0)
		s << *_expr;
	return s;
	}

void
port_connection::write(const write_type &wr) const
	{ wr(this); }

bool
port_connection::setup(const setup_type &setup)
	{ return setup(this); }

port_connection::ostream_type &
operator<<(port_connection::ostream_type &s, const port_connection &d)
	{ return d.display(s); }
