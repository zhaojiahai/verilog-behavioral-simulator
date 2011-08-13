// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// bitsel.cc

#include "misc/bitsel.h"
#include "expr/eeval.h"

bit_select::bit_select(expr_type *e)
	{ _expr = e; }

bit_select::bit_select(const bit_select &p)
	: common_base(p), select_base(p)
	{ _expr = p._expr->copy_constructor(); }

bit_select::~bit_select()
	{
	delete _expr;
	}

bit_select::select_size_type
bit_select::get_data(position_type &ms, position_type &ls) const
	{
	bool fail;
	ms = ls = _expr->evaluate(evaluate_expr()).to_signed_int(&fail);
	if (fail)
		ms = -1;
	return select_type::SELECT_BIT;
	}

bit_select *
bit_select::copy_constructor() const
	{ return new bit_select(*this); }

bit_select::ostream_type &
bit_select::display(ostream_type &s) const
	{
	s << '[' << *_expr << ']';
	return s;
	}

void
bit_select::write(const write_type &wr) const
	{ wr(this); }

bool
bit_select::monitor(const monitor_type &mon) const
	{ return mon(this); }

void
bit_select::setup(const setup_type &setup)
	{ setup(this); }
