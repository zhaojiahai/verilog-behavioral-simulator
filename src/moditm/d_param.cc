// Verilog Behavioral Simulator
// Copyright (C) 1997,2001,2003 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_param.cc

#include "moditm/d_param.h"

param_decl::param_decl(decl_assign_list *p, range_type *r)
	{ _range = r; _id_list = p; }

param_decl::~param_decl()
	{ delete _range; delete _id_list; }

param_decl::ostream_type &
param_decl::display(ostream_type &s) const
	{
	s << "parameter ";
	if (_range != 0)
		s << *_range << ' ';
	if (_id_list != 0)
		{
		decl_assign_list::iterator itp(_id_list->begin());
		decl_assign_list::iterator stop(_id_list->end());
		s << *(*itp)->_ident << " = " << *(*itp)->_rval;
		for (++itp; itp != stop; ++itp)
			s << ", " << *(*itp)->_ident << " = " << *(*itp)->_rval;
		}
	s << ';' << endl;
	return s;
	}

void
param_decl::write(const write_type &wr) const
    { wr(this); }

void
param_decl::setup(const setup_type &setup)
	{ setup(this); }
