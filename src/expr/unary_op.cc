// Verilog Behavioral Simulator
// Copyright (C) 1996-1997,2001,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// unary_op.cc

#include "expr/unary_op.h"

unary_op_expr::unary_op_expr(operator_type op, expr_type *e)
	: _operator(op)
	{ _expr = e; }

unary_op_expr::unary_op_expr(const unary_op_expr &p)
	: common_base(p), expr_base(p), _operator(p._operator)
	{ _expr = p._expr->copy_constructor(); }

unary_op_expr::~unary_op_expr()
	{ delete _expr; }

unary_op_expr *
unary_op_expr::copy_constructor() const
	{ return new unary_op_expr(*this); }

unary_op_expr::ostream_type &
unary_op_expr::display(ostream_type &s) const
	{
	const char *op = ".";
	switch (_operator)
		{
		case PLUS_EXPR: op = "+"; break;
		case MINUS_EXPR: op = "-"; break;
		case NOT_EXPR: op = "!"; break;
		case INVERT_EXPR: op = "~"; break;
		case AND_EXPR: op = "&"; break;
		case NAND_EXPR: op = "~&"; break;
		case OR_EXPR: op = "|"; break;
		case NOR_EXPR: op = "~|"; break;
		case XOR_EXPR: op = "^"; break;
		case XNOR_EXPR: op = "~^"; break;
		case NXOR_EXPR: op = "^~"; break;
		}
	return display_unary_expr(s, *_expr, op);
	}

void
unary_op_expr::write(const write_type &wr) const
	{ wr(this); }

bool
unary_op_expr::monitor(const monitor_type &mon) const
	{ return mon(this); }

const unary_op_expr::num_type &
unary_op_expr::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

unary_op_expr::size_type
unary_op_expr::setup(const setup_type &setup)
	{ return setup(this); }

void
unary_op_expr::trigger(const trigger_type &trigger)
	{ trigger(this); }
