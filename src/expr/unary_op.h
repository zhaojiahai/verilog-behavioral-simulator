// Verilog Behavioral Simulator
// Copyright (C) 1996-1997,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// unary_op.h

#ifndef _UNARY_OP_H
#define _UNARY_OP_H

#include "expr/exprbase.h"

class unary_op_expr : public expr_base
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
		PLUS_EXPR,
		MINUS_EXPR,
		NOT_EXPR,
		INVERT_EXPR,
		AND_EXPR,
		NAND_EXPR,
		OR_EXPR,
		NOR_EXPR,
		XOR_EXPR,
		XNOR_EXPR,
		NXOR_EXPR
		};

	unary_op_expr(operator_type, expr_type *);
	unary_op_expr(const unary_op_expr &);
	~unary_op_expr();

	unary_op_expr *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	const num_type &evaluate(const evaluate_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);

private:
	operator_type _operator;
	expr_type *_expr;
	};

#endif // _UNARY_OP_H
