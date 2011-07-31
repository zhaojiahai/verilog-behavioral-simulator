// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// always.cc

#include "moditm/always.h"

always::always(stmt_type *s)
	{ _stmt = s; }

always::~always()
	{ delete _stmt; }

always::ostream_type &
always::display(ostream_type &s) const
	{
	s << "always " << *_stmt;
	return s;
	}

void
always::write(const write_type &wr) const
	{ wr(this); }

void
always::setup(const setup_type &setup)
	{ setup(this); }
