// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// eeval.h

#ifndef _EEVAL_H
#define _EEVAL_H

#include "expr/exprbase.h"

struct evaluate_expr : public expr_evaluate
	{
	const num_type &operator()(const function_call *) const;
	const num_type &operator()(const concatenation *) const;
	const num_type &operator()(const number *) const;
	const num_type &operator()(const qouted_str *) const;
	const num_type &operator()(const range_id *) const;
	const num_type &operator()(const unary_op_expr *) const;
	const num_type &operator()(const binary_op_expr *) const;
	const num_type &operator()(const ternary_op_expr *) const;
	const num_type &operator()(const mintypmax_expr *) const;
	};

#endif // _EEVAL_H
