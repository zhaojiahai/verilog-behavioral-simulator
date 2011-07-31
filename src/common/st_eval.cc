// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_eval.cc

#include "common/st_func.h"
#include "common/st_net.h"
#include "common/st_util.h"
#include "common/st_eval.h"
#include "expr/funccall.h"
#include "stmt/stmtbase.h"
#include "stmt/strigger.h"

const evaluate_st_node::num_type &
evaluate_st_node::operator()(const st_function *p) const
	{
	// Initialize local variables.
	if (p->_iovariable != 0)
		entry_iovars(*(p->_iovariable), *(_caller->_argument));
	// Trigger the statement in the function.  Functions usually
	// take arguments.  But since a function caller is an expression,
	// we can't pass in the caller when triggering the statement.
	p->_stmt->trigger(trigger_stmt(p->_stmt));
	// Set output variables.
	if (p->_iovariable != 0)
		exit_iovars(*(p->_iovariable), *(_caller->_argument));
	return p->_storage;
	}

const evaluate_st_node::num_type &
evaluate_st_node::operator()(const st_net *p) const
	{
	st_net_evaluate eval(*p);
	return eval(p->_port_ms, p->_port_ls, _ms, _ls, false);
	}
