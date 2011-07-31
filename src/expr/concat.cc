// Verilog Behavioral Simulator
// Copyright (C) 1997,2001-2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// concat.cc

#include "expr/concat.h"

concatenation::concatenation(expr_list *p, expr_type *r)
	{ _exp_list = p; _repeat_expr = r; }

concatenation::concatenation(const concatenation &p)
	: common_base(p), expr_base(p)
	{
	expr_list::iterator itp(p._exp_list->begin());
	expr_list::iterator stop(p._exp_list->end());
	_exp_list = new expr_list;
	for (; itp != stop; ++itp)
		_exp_list->push_back((*itp)->copy_constructor());
	if (p._repeat_expr != 0)
		_repeat_expr = p._repeat_expr->copy_constructor();
	else
		_repeat_expr = 0;
	}

concatenation::~concatenation()
	{
	delete _exp_list;
	delete _repeat_expr;
	}

concatenation *
concatenation::copy_constructor() const
	{ return new concatenation(*this); }

concatenation::ostream_type &
concatenation::display(concatenation::ostream_type &s) const
	{
	expr_list::iterator itp(_exp_list->begin());
	expr_list::iterator stop(_exp_list->end());

	if (_repeat_expr != 0)
		{
		s << "{ ";
		_repeat_expr->display(s);
		}

	s << "{ ";
	(*itp)->display(s);
	for (++itp; itp != stop; ++itp)
		{
		s << ", ";
		(*itp)->display(s);
		}
	s << " }";

	if (_repeat_expr != 0)
		s << " }";

	return s;
	}

void
concatenation::write(const write_type &wr) const
	{ wr(this); }

bool
concatenation::monitor(const monitor_type &mon) const
	{ return mon(this); }

const concatenation::num_type &
concatenation::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

concatenation::size_type
concatenation::setup(const setup_type &setup)
	{ return setup(this); }

void
concatenation::trigger(const trigger_type &trigger)
	{ trigger(this); }
