// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// etrigger.h

#ifndef _ETRIGGER_H
#define _ETRIGGER_H

#include "expr/exprbase.h"

struct trigger_expr : public expr_trigger
	{
	typedef expr_base expr_type;
	typedef expr_type::str_type str_type;
	typedef expr_type::num_type num_type;

	trigger_expr(str_type &s)
		: _format(s)
		{}

	void operator()(function_call *) const;
	void operator()(concatenation *) const;
	void operator()(number *) const;
	void operator()(qouted_str *) const;
	void operator()(range_id *) const;
	void operator()(unary_op_expr *) const;
	void operator()(binary_op_expr *) const;
	void operator()(ternary_op_expr *) const;
	void operator()(mintypmax_expr *) const;

	str_type &_format;
	};

#endif // _ETRIGGER_H
