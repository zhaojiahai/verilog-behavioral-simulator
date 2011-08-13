// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// number.cc

#include <cstdlib>
#include "expr/number.h"

number::number(const char *n, signed_type st, number::base_type b, number::size_type s)
	: expr_base(true), bit_vector(n, st, b, s)
	{}

number::number(number::position_type m, number::position_type l, signed_type st)
	: expr_base(true), bit_vector(m, l, st)
	{}

number::number(const number &p)
	: common_base(p), expr_base(p), bit_vector(p)
	{}

number::~number()
	{
	}

number *
number::get_number()
	{ return this; }

number *
number::copy_constructor() const
	{ return new number(*this); }

number::nostream_type &
number::display(number::nostream_type &s) const
	{
	s << to_string();
	return s;
	}

void
number::write(const write_type &wr) const
	{ wr(this); }

bool
number::monitor(const monitor_type &mon) const
	{ return mon(this); }

const number::num_type &
number::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

number::size_type
number::setup(const setup_type &setup)
	{ return setup(this); }

void
number::trigger(const trigger_type &trigger)
	{ trigger(this); }
