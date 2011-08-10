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

port_connection::port_connection(expr_type *e, const char *pn)
	{ _expr = e; _port_name = pn != 0 ? new str_type(pn) : 0; }

port_connection::~port_connection()
	{ delete _expr; delete _port_name; }

const port_connection::str_type *
port_connection::name()
	{return _port_name; }

port_connection::ostream_type &
port_connection::display(ostream_type &s) const
	{
	if (_port_name != 0)
		s << "." << *_port_name << "(";
	if (_expr != 0)
		s << *_expr;
	if (_port_name != 0)
		s << ")";
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
