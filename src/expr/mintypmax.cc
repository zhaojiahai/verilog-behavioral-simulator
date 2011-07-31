// Verilog Behavioral Simulator
// Copyright (C) 1997,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mintypmax.cc

#include "expr/mintypmax.h"

mintypmax_expr::delay_type mintypmax_expr::_delay = mintypmax_expr::TYPDELAY;

void
mintypmax_expr::min_delay()
	{ _delay = MINDELAY; }

void
mintypmax_expr::typ_delay()
	{ _delay = TYPDELAY; }

void
mintypmax_expr::max_delay()
	{ _delay = MAXDELAY; }

mintypmax_expr::mintypmax_expr(expr_type *min, expr_type *typ, expr_type *max)
	{
	_min_expr = (min != 0) ? min : 0;
	_typ_expr = (typ != 0) ? typ : 0;
	_max_expr = (max != 0) ? max : 0;
	}

mintypmax_expr::mintypmax_expr(const mintypmax_expr &p)
	: common_base(p), expr_base(p)
	{
	_min_expr = (p._min_expr != 0) ? p._min_expr->copy_constructor() : 0;
	_typ_expr = (p._typ_expr != 0) ? p._typ_expr->copy_constructor() : 0;
	_max_expr = (p._max_expr != 0) ? p._max_expr->copy_constructor() : 0;
	}

mintypmax_expr::~mintypmax_expr()
	{
	if (_min_expr != 0)
		delete _min_expr;
	if (_typ_expr != 0)
		delete _typ_expr;
	if (_max_expr != 0)
		delete _max_expr;
	}

mintypmax_expr *
mintypmax_expr::copy_constructor() const
	{ return new mintypmax_expr(*this); }

mintypmax_expr::ostream_type &
mintypmax_expr::display(ostream_type &s) const
	{
	s << '(';
	s << *_min_expr;
	if (_typ_expr != 0)
		{
		s << " : ";
		s << *_typ_expr;
		s << " : ";
		if (_max_expr != 0)
			s << *_max_expr;
		}
	s << ')';
	return s;
	}

void
mintypmax_expr::write(const write_type &wr) const
	{ wr(this); }

bool
mintypmax_expr::monitor(const monitor_type &mon) const
	{ return mon(this); }

const mintypmax_expr::num_type &
mintypmax_expr::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

mintypmax_expr::size_type
mintypmax_expr::setup(const setup_type &setup)
	{ return setup(this); }

void
mintypmax_expr::trigger(const trigger_type &trigger)
	{ trigger(this); }
