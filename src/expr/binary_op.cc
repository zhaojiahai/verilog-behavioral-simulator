// Verilog Behavioral Simulator
// Copyright (C) 1996-1997,2001,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// binary_op.cc

#include "expr/binary_op.h"

binary_op_expr::binary_op_expr(operator_type op, expr_type *l, expr_type *r)
	: _operator(op)
	{
	_left = l;
	_right = r;
	}

binary_op_expr::binary_op_expr(const binary_op_expr &p)
	: common_base(p), expr_base(p), _operator(p._operator)
	{
	_left = p._left->copy_constructor();
	_right = p._right->copy_constructor();
	}

binary_op_expr::~binary_op_expr()
	{
	delete _left;
	delete _right;
	}

binary_op_expr *
binary_op_expr::copy_constructor() const
	{ return new binary_op_expr(*this); }

binary_op_expr::ostream_type &
binary_op_expr::display(ostream_type &s) const
	{
	const char *op = ".";
	switch (_operator)
		{
		case ADD: op = "+"; break;
		case SUB: op = "-"; break;
		case MULTIPLY: op = "*"; break;
		case DIVIDE: op = "/"; break;
		case MODULO: op = "%"; break;
		case POWER: op = "**"; break;
		case EQUAL_EQUAL: op = "=="; break;
		case NOT_EQUAL: op = "!="; break;
		case EQUAL_EQUAL_EQUAL: op = "==="; break;
		case NOT_EQUAL_EQUAL: op = "!=="; break;
		case AND_AND: op = "&&"; break;
		case OR_OR: op = "||"; break;
		case LESS_THAN: op = "<"; break;
		case LESS_EQUAL: op = "<="; break;
		case GRT_THAN: op = ">"; break;
		case GRT_EQUAL: op = ">="; break;
		case LOGIC_AND: op = "&"; break;
		case LOGIC_NAND: op = "~&"; break;
		case LOGIC_OR: op = "|"; break;
		case LOGIC_NOR: op = "~|"; break;
		case LOGIC_XOR: op = "^"; break;
		case LOGIC_XNOR: op = "~^"; break;
		case LOGIC_NXOR: op = "^~"; break;
		case LEFT_SHIFT: op = "<<"; break;
		case RIGHT_SHIFT: op = ">>"; break;
		}
	return display_binary_expr(s, *_left, *_right, op);
	}

void
binary_op_expr::write(const write_type &wr) const
	{ wr(this); }

bool
binary_op_expr::monitor(const monitor_type &mon) const
	{ return mon(this); }

const binary_op_expr::num_type &
binary_op_expr::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

binary_op_expr::size_type
binary_op_expr::setup(const setup_type &setup)
	{ return setup(this); }

void
binary_op_expr::trigger(const trigger_type &trigger)
	{ trigger(this); }
