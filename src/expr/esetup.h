// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// esetup.h

#ifndef _ESETUP_H
#define _ESETUP_H

#include "common/dstack.h"
#include "expr/exprbase.h"
#include "expr/funccall.h"

struct stmt_base;
class st_function;
class st_net;

struct setup_expr : public expr_setup
	{
	typedef Stack<int> scopelist_type;
	typedef expr_base expr_type;
	typedef expr_type::num_type num_type;
	typedef stmt_base stmt_type;
	typedef function_call func_caller_type;
	typedef func_caller_type::arg_list arg_list;
	typedef st_function func_type;
	typedef st_net net_type;

	setup_expr(scopelist_type &s, bool cc = false, stmt_type *p = 0, size_type size = 0)
		: _check_const(cc), _result_size(size), _scope(s)
		{ _parent = p; }

	size_type operator()(function_call *) const;
	size_type operator()(concatenation *) const;
	size_type operator()(number *) const;
	size_type operator()(qouted_str *) const;
	size_type operator()(range_id *) const;
	size_type operator()(unary_op_expr *) const;
	size_type operator()(binary_op_expr *) const;
	size_type operator()(ternary_op_expr *) const;
	size_type operator()(mintypmax_expr *) const;

	bool _check_const;
	size_type _result_size;
	scopelist_type &_scope;
	stmt_type *_parent;
	};

#endif // _ESETUP_H
