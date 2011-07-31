// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// etrigger.cc

#include "common/sym_tab.h"
#include "common/st_net.h"
#include "common/st_func.h"
#include "common/st_eval.h"
#include "expr/eeval.h"
#include "expr/etrigger.h"
#include "expr/funccall.h"
#include "expr/concat.h"
#include "expr/number.h"
#include "expr/qstr.h"
#include "expr/rangeid.h"
#include "expr/unary_op.h"
#include "expr/binary_op.h"
#include "expr/ternary_op.h"
#include "expr/mintypmax.h"

extern symbol_table symboltable;

void
display_output(const trigger_expr::str_type &s, const trigger_expr::num_type &n)
	{
	trigger_expr::num_type::base_type base = trigger_expr::num_type::BASE10;
	trigger_expr::num_type::size_type size = 0;
	get_base_size(s, base, size);
	cout << n.to_string(base, size);
	}

void
trigger_expr::operator()(function_call *p) const
	{
	// <node> can not be NULL.  Setup should have checked.
	st_function *node = symboltable.get(p->_index)->get_function();
	*p->_result = node->evaluate(evaluate_st_node(p));
	display_output(_format, *p->_result);
	}

void
trigger_expr::operator()(concatenation *p) const
	{ display_output(_format, p->evaluate(evaluate_expr())); }

void
trigger_expr::operator()(number *n) const
	{ display_output(_format, *n); }

void
trigger_expr::operator()(qouted_str *p) const
	{
	// Display the string without the qoutes.
	if (_format != "s" && _format != "S")
		display_output(_format, p->_value);
	else
		{
		size_t len = p->get_str().size() - 1;
		const str_type &s(p->get_str());
		for (size_t i = 1; i < len; ++i)
			cout << s[i];
		}
	}

void
trigger_expr::operator()(range_id *p) const
	{
	// Grab the data from the symbol table.
	st_net *node = symboltable.get(p->_index)->get_net();
	// Print out the content.  Setup should have checked the range.
	range_id::position_type ms, ls;
	if (p->get_select(ms, ls) != range_id::select_type::SELECT_NONE)
		*p->_result = node->evaluate(evaluate_st_node(ms, ls));
	else
		*p->_result = node->evaluate(evaluate_st_node());
	display_output(_format, *p->_result);
	}

void
trigger_expr::operator()(unary_op_expr *p) const
	{ display_output(_format, p->evaluate(evaluate_expr())); }

void
trigger_expr::operator()(binary_op_expr *p) const
	{ display_output(_format, p->evaluate(evaluate_expr())); }

void
trigger_expr::operator()(ternary_op_expr *p) const
	{ display_output(_format, p->evaluate(evaluate_expr())); }

void
trigger_expr::operator()(mintypmax_expr *p) const
	{ display_output(_format, p->evaluate(evaluate_expr())); }
