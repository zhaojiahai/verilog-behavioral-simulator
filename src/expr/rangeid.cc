// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// rangeid.cc

#include "expr/rangeid.h"

range_id::range_id(const char *n, select_type *r)
	: _index(), _name(n)
	{ _select = r; }

range_id::range_id(const range_id &p)
	: common_base(p), expr_base(p), _index(p._index), _name(p._name)
	{
	if (p._select != 0)
		_select = p._select->copy_constructor();
	else
		_select = 0;
	}

range_id::~range_id()
	{ delete _select; }

range_id *
range_id::get_range_id()
	{ return this; }

range_id *
range_id::copy_constructor() const
	{ return new range_id(*this); }

range_id::ostream_type &
range_id::display(ostream_type &s) const
	{
	s << _name;
	if (_select != 0)
		s << *_select;
	return s;
	}

void
range_id::write(const write_type &wr) const
	{ wr(this); }

bool
range_id::monitor(const monitor_type &mon) const
	{ return mon(this); }

const range_id::num_type &
range_id::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

range_id::size_type
range_id::setup(const setup_type &setup)
	{ return setup(this); }

void
range_id::trigger(const trigger_type &trigger)
	{ trigger(this); }
