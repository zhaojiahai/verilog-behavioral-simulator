// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_io.cc

#include "expr/rangeid.h"
#include "moditm/d_io.h"

io_decl::io_decl(io_decl_type t, ident_list *i, range_type *r)
	: _type(t)
	{ _range = r; _id_list = i; }

io_decl::~io_decl()
	{ delete _range; delete _id_list; }

io_decl::ostream_type &
io_decl::display(ostream_type &s) const
	{
	switch (_type)
		{
		case INPUT: s << "input "; break;
		case OUTPUT: s << "output "; break;
		case INOUT: s << "inout "; break;
		default: s << "??? "; break;
		}

	if (_range != 0)
		s << *_range << ' ';
	if (_id_list != 0)
		{
		ident_list::iterator itp(_id_list->begin());
		ident_list::iterator stop(_id_list->end());
		s << *(*itp);
		for (++itp; itp != stop; ++itp)
			s << ", " << *(*itp);
		}
	s << ';' << endl;
	return s;
	}

void
io_decl::write(const write_type &wr) const
    { wr(this); }

void
io_decl::setup(const setup_type &setup)
	{ setup(this); }
