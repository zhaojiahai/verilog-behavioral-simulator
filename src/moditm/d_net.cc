// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_net.cc

#include "moditm/d_net.h"

net_decl::net_decl(declaration_type t, decl_assign_list *i, range_type *r)
	: _type(t)
	{ _range = r; _id_list = i; }

net_decl::~net_decl()
	{ delete _range; delete _id_list; }

net_decl::ostream_type &
net_decl::display(ostream_type &s) const
	{
	switch (_type)
		{
		case WIRE: s << "wire "; break;
		case SUPPLY0: s << "supply0 "; break;
		case SUPPLY1: s << "supply1 "; break;
		default: s << "??? "; break;
		}
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
net_decl::write(const write_type &wr) const
    { wr(this); }

void
net_decl::setup(const setup_type &setup)
	{ setup(this); }
