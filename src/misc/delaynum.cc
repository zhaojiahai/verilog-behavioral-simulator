// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// delaynum.cc

#include "common/bvector.h"
#include "misc/delaynum.h"

delay_num::delay_num(const num_type &num)
	: bit_vector(num)
	{ _event = 0; }

delay_num::delay_num(const delay_num &p)
	: common_base(p), dec_base(p), bit_vector(p)
	{
	// We should not be making copies of delay_num after setup.
	// Thus _event should be NULL in p anyway.
	_event = 0;
	}

delay_num *
delay_num::copy_constructor() const
	{ return new delay_num(*this); }

delay_num::ostream_type &
delay_num::display(ostream_type &s) const
	{
	s << '#' << to_string(num_type::BASE10) << ' ';
	return s;
	}

void
delay_num::write(const write_type &wr) const
	{ wr(this); }

void
delay_num::setup(const setup_type &setup)
	{ setup(this); }

int
delay_num::trigger(const trigger_type &trigger)
	{ return trigger(this); }
