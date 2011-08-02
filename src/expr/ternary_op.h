// Verilog Behavioral Simulator
// Copyright (C) 1997,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// ternary_op.h

#ifndef _TERNARY_OP_H
#define _TERNARY_OP_H

#include "expr/exprbase.h"

class ternary_op_expr : public expr_base
	{
	friend struct monitor_expr;
	friend struct evaluate_expr;
	friend struct setup_expr;
	friend struct trigger_expr;
	friend struct write_expr;

public:
	typedef expr_base expr_type;

	ternary_op_expr(expr_type *, expr_type *, expr_type *);
	ternary_op_expr(const ternary_op_expr &);
	virtual ~ternary_op_expr();

	ternary_op_expr *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	const num_type &evaluate(const evaluate_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);

private:
	expr_type *_expr;
	expr_type *_true_expr;
	expr_type *_false_expr;
	};

#endif // _TERNARY_OP_H
