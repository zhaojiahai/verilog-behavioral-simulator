// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_reg.cc

#include "expr/rangeid.h"
#include "moditm/d_reg.h"

reg_decl::reg_decl(decl_assign_list *i, range_type *r)
	{ _range = r; _id_list = i; }

reg_decl::~reg_decl()
	{ delete _range; delete _id_list; }

reg_decl::ostream_type &
reg_decl::display(ostream_type &s) const
	{
	s << "reg ";
	if (_range != 0)
		s << *_range << ' ';
	if (_id_list != 0)
		{
		decl_assign_list::iterator itp(_id_list->begin());
		decl_assign_list::iterator stop(_id_list->end());
		s << *(*itp)->_ident;
		if ((*itp)->_rval != 0)
			s << " = " << *(*itp)->_rval;
		for (++itp; itp != stop; ++itp)
			{
			s << ", " << *(*itp)->_ident;
			if ((*itp)->_rval != 0)
				s << " = " << *(*itp)->_rval;
			}
		}
	s << ';' << endl;
	return s;
	}

void
reg_decl::write(const write_type &wr) const
    { wr(this); }

void
reg_decl::setup(const setup_type &setup)
	{ setup(this); }
