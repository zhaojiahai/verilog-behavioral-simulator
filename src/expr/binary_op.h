// Verilog Behavioral Simulator
// Copyright (C) 1996-1997,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// binary_op.h

#ifndef _BINARY_OP_H
#define _BINARY_OP_H

#include "expr/exprbase.h"

class binary_op_expr : public expr_base
	{
	friend struct monitor_expr;
	friend struct evaluate_expr;
	friend struct setup_expr;
	friend struct trigger_expr;
	friend struct write_expr;

public:
	typedef expr_base expr_type;

	enum operator_type
		{
		ADD,
		SUB,
		MULTIPLY,
		DIVIDE,
		MODULO,
		EQUAL_EQUAL,
		NOT_EQUAL,
		EQUAL_EQUAL_EQUAL,
		NOT_EQUAL_EQUAL,
		AND_AND,
		OR_OR,
		LESS_THAN,
		LESS_EQUAL,
		GRT_THAN,
		GRT_EQUAL,
		LOGIC_AND,
		LOGIC_NAND,
		LOGIC_OR,
		LOGIC_NOR,
		LOGIC_XOR,
		LOGIC_XNOR,
		LOGIC_NXOR,
		LEFT_SHIFT,
		RIGHT_SHIFT
		};

	binary_op_expr(operator_type, expr_type *, expr_type *);
	binary_op_expr(const binary_op_expr &);
	~binary_op_expr();

	binary_op_expr *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	const num_type &evaluate(const evaluate_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);

private:
	operator_type _operator;
	expr_type *_left;
	expr_type *_right;
	};

#endif // _BINARY_OP_H
