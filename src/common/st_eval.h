// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_eval.h
//
// Function object to evaluate a symbol table node.  The only nodes
// that can be evaluated right now are net and function nodes.

#ifndef _ST_EVAL_H
#define _ST_EVAL_H

#include "common/st_node.h"

class function_call;

struct evaluate_st_node : public st_node_evaluate
	{
	typedef st_node_evaluate::num_type num_type;
	typedef num_type::position_type position_type;
	typedef function_call func_caller_type;

	evaluate_st_node()
		: _ms(-1), _ls(-1)
		{ _caller = 0; }
	evaluate_st_node(const func_caller_type *f)
		: _ms(-1), _ls(-1)
		{ _caller = f; }
	evaluate_st_node(const position_type m, const position_type l)
		: _ms(m), _ls(l)
		{ _caller = 0; }

	const num_type &operator()(const st_net *) const;
	const num_type &operator()(const st_function *) const;

	position_type _ms;
	position_type _ls;
	const func_caller_type *_caller;
	};

#endif // _ST_EVAL_H
