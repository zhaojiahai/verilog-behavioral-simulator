// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// number.h

#ifndef _NUMBER_H
#define _NUMBER_H

#include "expr/exprbase.h"

class number : public expr_base, public expr_base::num_type
	{
	friend struct evaluate_expr;
	friend struct setup_expr;
	friend struct trigger_expr;
	friend struct write_expr;

public:
	typedef num_type::size_type size_type;
	typedef num_type::position_type position_type;
	typedef num_type::signed_type signed_type;
	typedef num_type::base_type base_type;
	typedef expr_base::str_type nstr_type;
	typedef expr_base::ostream_type nostream_type;

	number(const char *, signed_type, base_type, size_type);
	number(position_type, position_type, signed_type);
	number(const number &);
	virtual ~number();

	number *get_number();

	number *copy_constructor() const;
	nostream_type &display(nostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	const num_type &evaluate(const evaluate_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);
	};

// Comparison with integer constants.

inline bool
operator==(number &l, unsigned long val)
	{ return bit_vector(l) == bit_vector(val); }

inline bool
operator==(unsigned long val, number &r)
	{ return bit_vector(val) == bit_vector(r); }

inline bool
operator<(number &l, unsigned long val)
	{ return bit_vector(l) < bit_vector(val); }

inline bool
operator<(unsigned long val, number &r)
	{ return bit_vector(val) < bit_vector(r); }

inline number
operator<<(number &l, number &r)
	{ return (l << r); }

inline number
operator>>(number &l, number &r)
	{ return (l >> r); }

#endif // _NUMBER_H
