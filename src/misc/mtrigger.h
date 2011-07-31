// Verilog Behavioral Simulator
// Copyright (C) 1997,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mtrigger.h

#ifndef _MTRIGGER_H
#define _MTRIGGER_H

#include "expr/exprbase.h"
#include "misc/lvalue.h"

struct trigger_lvalue : public lvalue_trigger
	{
	typedef select_base::SELECT_SIZE select_size_type;
	typedef select_base::position_type position_type;
	typedef expr_base expr_type;
	typedef expr_type::num_type num_type;
	typedef st_node_base node_type;
	typedef st_function func_type;
	typedef st_net net_type;

	trigger_lvalue(const num_type &n)
		: _num(n)
		{}

	void operator()(lvalue *) const;

	const num_type &_num;
	};

#endif // _MTRIGGER_H
