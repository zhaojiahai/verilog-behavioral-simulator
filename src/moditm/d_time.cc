// Verilog Behavioral Simulator
// Copyright (C) 2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_time.cc

#include "moditm/d_time.h"

time_decl::time_decl(decl_assign_list *i)
	{ _id_list = i; }

time_decl::~time_decl()
	{ delete _id_list; }

time_decl::ostream_type &
time_decl::display(ostream_type &s) const
	{
	s << "integer ";
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
time_decl::write(const write_type &wr) const
    { wr(this); }

void
time_decl::setup(const setup_type &setup)
	{ setup(this); }
