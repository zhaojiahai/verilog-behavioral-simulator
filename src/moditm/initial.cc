// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// initial.cc

#include "moditm/initial.h"

initial::initial(stmt_type *s)
	{ _stmt = s; }

initial::~initial()
	{ delete _stmt; }

initial::ostream_type &
initial::display(ostream_type &s) const
	{
	s << "initial " << *_stmt;
	return s;
	}

void
initial::write(const write_type &wr) const
    { wr(this); }

void
initial::setup(const setup_type &setup)
	{ setup(this); }
