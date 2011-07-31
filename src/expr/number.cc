// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// number.cc

#include <cstdlib>
#include "expr/number.h"

number::number(const char *n, number::base_type b, number::size_type s)
	: expr_base(true), bit_vector(n, b, s)
	{}

number::number(number::position_type m, number::position_type l)
	: expr_base(true), bit_vector(m, l)
	{}

number::number(const number &p)
	: common_base(p), expr_base(p), bit_vector(p)
	{}

number *
number::get_number()
	{ return this; }

number *
number::copy_constructor() const
	{ return new number(*this); }

number::nostream_type &
number::display(number::nostream_type &s) const
	{
	s << to_string(BASE2, 0);
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

void
get_base_size(const number::nstr_type &fmt,
		number::base_type &base, unsigned long &size)
	{
	size_t j = 0;
	number::nstr_type buf(fmt.length(), '\0');
	for (size_t i = 0; i < fmt.length(); ++i)
		{
		// There should be no errors in <fmt>, so we will not
		// do error checking...
		if (fmt[i] >= '0' && fmt[i] <= '9')
			buf[j++] = fmt[i];
		else if (fmt[i] == 'h')
			{ base = number::BASE16; break; }
		else if (fmt[i] == 'd')
			{ base = number::BASE10; break; }
		else if (fmt[i] == 'o')
			{ base = number::BASE8; break; }
		else if (fmt[i] == 'b')
			{ base = number::BASE2; break; }	
		// s for ascii, t/T for current time format.
		else if (fmt[i] == 's')
			{ base = number::BASESTR; break; }
		else if (fmt[i] == 't' || fmt[i] == 'T')
			{ if (j == 0) { buf[j++] = '2'; buf[j++] = '0'; } break; }
		}
	buf[j] = '\0';

	// Convert to format.
	size = atoi(buf.data());
	}
