// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// cassign.cc

#include "moditm/cassign.h"

cont_assign::cont_assign(assign_stmt_list *a, dec_type *d)
	{ _dec = d; _assign = a; }

cont_assign::~cont_assign()
	{ delete _dec; delete _assign; }

cont_assign::ostream_type &
cont_assign::display(ostream_type &s) const
	{
	s << "assign ";
	if (_dec != 0)
		s << *_dec << ' ';
	assign_stmt_list::iterator itp(_assign->begin());
	assign_stmt_list::iterator stop(_assign->end());
	s << *(*itp);
	for (++itp; itp != stop; ++itp)
		s << ',' << endl << *(*itp);
	return s;
	}

void
cont_assign::write(const write_type &wr) const
    { wr(this); }

void
cont_assign::setup(const setup_type &setup)
	{ setup(this); }
