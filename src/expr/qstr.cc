// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// qstr.cc

#include "expr/qstr.h"

qouted_str::qouted_str(const char *s)
	: expr_base(true), _str(s, strlen(s))
	{
	int len = 8 * (_str.size() - 2); // Don't include the qoutes.
	_value = num_type(len > 0 ? len - 1 : 0, 0, num_type::UNSIGNED, num_type::BASESTR);
	++s;
	for (int i = len - 8, j = len - 1; i >= 0; i -= 8, j -= 8, ++s)
		_value(j, i) = num_type(static_cast<num_type::decimal_type>(*s));
	}

qouted_str::qouted_str(const qouted_str &p)
	: common_base(p), expr_base(p), _str(p._str), _value(p._value)
	{
	// This must be a bug, the call to the copy constructor of
	// common_base should have done the job.
	_lineno = p._lineno;
	}

qouted_str::str_type &
qouted_str::get_str()
	{ return _str; }

const qouted_str::str_type &
qouted_str::get_str() const
	{ return _str; }

qouted_str *
qouted_str::get_qouted_str()
	{ return this; }

qouted_str *
qouted_str::copy_constructor() const
	{ return new qouted_str(*this); }

qouted_str::ostream_type &
qouted_str::display(ostream_type &s) const
	{
	s << _str;
	return s;
	}

void
qouted_str::write(const write_type &wr) const
	{ wr(this); }

bool
qouted_str::monitor(const monitor_type &mon) const
	{ return mon(this); }

const qouted_str::num_type &
qouted_str::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

qouted_str::size_type
qouted_str::setup(const setup_type &setup)
	{ return setup(this); }

void
qouted_str::trigger(const trigger_type &trigger)
	{ trigger(this); }
