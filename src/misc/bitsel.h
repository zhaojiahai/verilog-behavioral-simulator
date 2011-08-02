// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// bitsel.h

#ifndef _BITSEL_H
#define _BITSEL_H

#include "misc/selbase.h"
#include "expr/exprbase.h"

class bit_select : public select_base
	{
	friend struct monitor_select;
	friend struct setup_select;
	friend struct write_select;

public:
	typedef select_base select_type;
	typedef expr_base expr_type;

	bit_select(expr_type *);
	bit_select(const bit_select &);
	virtual ~bit_select();

	select_size_type get_data(position_type &, position_type &) const;
	bit_select *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	void setup(const setup_type &);

private:
	expr_type *_expr;
	};

#endif // _BITSEL_H
