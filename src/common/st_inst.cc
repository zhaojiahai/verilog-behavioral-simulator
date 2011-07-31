// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_inst.cc

#include "common/st_inst.h"

st_instantiation::st_instantiation(const str_type &n)
	: st_node_base(n, "instantiation")
	{ }

int
st_instantiation::level() const
	{ return _level; }

void
st_instantiation::level(int l)
	{ _level = l; }

st_instantiation *
st_instantiation::get_instantiation()
	{ return this; }

st_instantiation::ostream_type &
st_instantiation::display(ostream_type &s) const
	{
	s << "('" << _name << "'," << type_name() << ',' << _scope << ')';
	return s;
	}

void
st_instantiation::setup(const setup_type &setup)
	{ setup(this); }
