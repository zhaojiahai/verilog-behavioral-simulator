// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// partsel.cc

#include "misc/partsel.h"

part_select::part_select(expr_type *l, expr_type *r)
	{
	_le = l;
	_re = r;
	}

part_select::part_select(const part_select &p)
	: common_base(p), select_base(p), _ln(p._ln), _rn(p._rn)
	{
	_le = p._le->copy_constructor();
	_re = p._re->copy_constructor();
	}

part_select::~part_select()
	{
	delete _le;
	delete _re;
	}

part_select::select_size_type
part_select::get_data(position_type &ms, position_type &ls) const
	{
	ms = _ln;
	ls = _rn;
	return select_type::SELECT_PART;
	}

part_select *
part_select::copy_constructor() const
	{ return new part_select(*this); }

part_select::ostream_type &
part_select::display(ostream_type &s) const
	{
	s << '[' << *_le << ':' << *_re << ']';
	return s;
	}

void
part_select::write(const write_type &wr) const
	{ wr(this); }

bool
part_select::monitor(const monitor_type &mon) const
	{ return mon(this); }

void
part_select::setup(const setup_type &s)
	{ s(this); }
