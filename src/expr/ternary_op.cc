// Verilog Behavioral Simulator
// Copyright (C) 1997,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// ternary_op.cc

#include "expr/ternary_op.h"

ternary_op_expr::ternary_op_expr(expr_type *e, expr_type *t, expr_type *f)
	{
	_expr = e;
	_true_expr = t;
	_false_expr = f;
	}

ternary_op_expr::ternary_op_expr(const ternary_op_expr &p)
	: common_base(p), expr_base(p)
	{
	_expr = p._expr->copy_constructor();
	_true_expr = p._true_expr->copy_constructor();
	_false_expr = p._false_expr->copy_constructor();
	}

ternary_op_expr::~ternary_op_expr()
	{
	delete _expr;
	delete _true_expr;
	delete _false_expr;
	}

ternary_op_expr *
ternary_op_expr::copy_constructor() const
	{ return new ternary_op_expr(*this); }

ternary_op_expr::ostream_type &
ternary_op_expr::display(ostream_type &s) const
	{
	s << *_expr;
	s << " ? ";
	s << *_true_expr;
	s << " : ";
	s << *_false_expr;
	return s;
	}

void
ternary_op_expr::write(const write_type &wr) const
	{ wr(this); }

bool
ternary_op_expr::monitor(const monitor_type &mon) const
	{ return mon(this); }

const ternary_op_expr::num_type &
ternary_op_expr::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

ternary_op_expr::size_type
ternary_op_expr::setup(const setup_type &setup)
	{ return setup(this); }

void
ternary_op_expr::trigger(const trigger_type &trigger)
	{ trigger(this); }
