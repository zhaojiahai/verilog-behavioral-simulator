// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002,2011 Lay Hoon Tho, Jimen Ching
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
#include "vbs.h"


void
get_base_size(const trigger_expr::str_type &fmt,
		trigger_expr::num_type::base_type &base, unsigned long &size)
	{
	size_t j = 0;
	number::nstr_type buf(fmt.length(), '\0');
	for (size_t i = 0; i < fmt.length(); ++i)
		{
		// There should be no errors in <fmt>, so we will not
		// do error checking...
		if (fmt[i] >= '0' && fmt[i] <= '9')
			buf[j++] = fmt[i];
		else if (fmt[i] == 'h')
			{ base = number::BASE16; break; }
		else if (fmt[i] == 'd')
			{ base = number::BASE10; break; }
		else if (fmt[i] == 'o')
			{ base = number::BASE8; break; }
		else if (fmt[i] == 'b')
			{ base = number::BASE2; break; }
		// s for ascii, t/T for current time format.
		else if (fmt[i] == 's')
			{ base = number::BASESTR; break; }
		else if (fmt[i] == 't' || fmt[i] == 'T')
			{ if (j == 0) { buf[j++] = '2'; buf[j++] = '0'; } break; }
		}
	buf[j] = '\0';

	// Convert to format.
	if (j > 0)
		size = atoi(buf.data());
	}

void
display_output(const trigger_expr::str_type &s, const trigger_expr::num_type &n)
	{
	trigger_expr::num_type::base_type base = trigger_expr::num_type::BASE10;
	trigger_expr::num_type::size_type size = static_cast<unsigned long>(-1);
	get_base_size(s, base, size);
	cout << n.to_string(base, size);
	}

void
trigger_expr::operator()(function_call *p) const
	{
	// <node> can not be NULL.  Setup should have checked.
	symbol_table &symboltable = vbs_engine::symboltable();
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
	symbol_table &symboltable = vbs_engine::symboltable();
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
