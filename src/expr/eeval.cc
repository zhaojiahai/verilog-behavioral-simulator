// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// eeval.cc

#include "common/sym_tab.h"
#include "common/st_net.h"
#include "common/st_func.h"
#include "common/st_eval.h"
#include "expr/eeval.h"
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


const evaluate_expr::num_type &
evaluate_expr::operator()(const function_call *p) const
	{
	symbol_table &symboltable = vbs_engine::symboltable();
	st_function *func = symboltable.get(p->_index)->get_function();
	*p->_result = func->evaluate(evaluate_st_node(p));
	return *p->_result;
	}

const evaluate_expr::num_type &
evaluate_expr::operator()(const concatenation *p) const
	{
	concatenation::size_type i, repeat_size;
	concatenation::num_type::position_type ls, ms, tmp;
	concatenation::element_list::const_iterator eitp;

	// Repeat count of zero will always result in zero.
	if (p->_repeat_count == 0)
		{
		*p->_result = LO;
		return *p->_result;
		}

	// Evaluate each sub-expression and merge them.
	concatenation::expr_list::iterator itp, stop(p->_exp_list->end());
	repeat_size = p->_result->size() / p->_repeat_count;
	for (i = 1; i <= p->_repeat_count; ++i)
		{
		itp = p->_exp_list->begin();
		eitp = p->_elmt_list.begin();
		tmp = (p->_repeat_count - i) * repeat_size;
		for (; itp != stop; ++itp, ++eitp)
			{
			ls = (*eitp).ls + tmp;
			ms = (*eitp).ms + tmp;
			(*p->_result)(ms, ls) = (*itp)->evaluate(evaluate_expr());
			}
		}
	return *p->_result;
	}

const evaluate_expr::num_type &
evaluate_expr::operator()(const number *p) const
	{
	*p->_result = *p;
	return *p->_result;
	}

const evaluate_expr::num_type &
evaluate_expr::operator()(const qouted_str *p) const
	{
	*p->_result = p->_value;
	return *p->_result;
	}

const evaluate_expr::num_type &
evaluate_expr::operator()(const range_id *p) const
	{
	symbol_table &symboltable = vbs_engine::symboltable();
	st_node_base *node = symboltable.get(p->_index);
	st_net *net = node->get_net();
	if (net == 0)
		{
		// We are the identifier of a function return value.  So just
		// return a number of the correct size.
		*p->_result = DC;
		return *p->_result;
		}
	// Setup should have checked range.
	range_id::position_type ms, ls;
	if (p->get_select(ms, ls) != range_id::select_type::SELECT_NONE)
		{
		if (ms != -1)
			*p->_result = net->evaluate(evaluate_st_node(ms, ls));
		else
			*p->_result = DC;
		}
	else
		*p->_result = net->evaluate(evaluate_st_node());
	return *p->_result;
	}

const evaluate_expr::num_type &
evaluate_expr::operator()(const unary_op_expr *p) const
	{
	switch (p->_operator)
		{
		case unary_op_expr::PLUS_EXPR:
			*p->_result = p->_expr->evaluate(evaluate_expr()); break;
		case unary_op_expr::MINUS_EXPR:
			unary_neg(*p->_result, p->_expr->evaluate(evaluate_expr())); break;
		case unary_op_expr::NOT_EXPR:
			*p->_result = !(p->_expr->evaluate(evaluate_expr())); break;
		case unary_op_expr::INVERT_EXPR:
			unary_inv(*p->_result, p->_expr->evaluate(evaluate_expr())); break;
		case unary_op_expr::AND_EXPR:
			*p->_result = reduce_and(p->_expr->evaluate(evaluate_expr())); break;
		case unary_op_expr::NAND_EXPR:
			*p->_result = reduce_nand(p->_expr->evaluate(evaluate_expr())); break;
		case unary_op_expr::OR_EXPR:
			*p->_result = reduce_or(p->_expr->evaluate(evaluate_expr())); break;
		case unary_op_expr::NOR_EXPR:
			*p->_result = reduce_nor(p->_expr->evaluate(evaluate_expr())); break;
		case unary_op_expr::XOR_EXPR:
			*p->_result = reduce_xor(p->_expr->evaluate(evaluate_expr())); break;
		case unary_op_expr::XNOR_EXPR:
			*p->_result = reduce_xnor(p->_expr->evaluate(evaluate_expr())); break;
		case unary_op_expr::NXOR_EXPR:
			*p->_result = reduce_xnor(p->_expr->evaluate(evaluate_expr())); break;
		}
	return *p->_result;
	}

const evaluate_expr::num_type &
evaluate_expr::operator()(const binary_op_expr *p) const
	{
	// We already calculated result for power operator.
	if (p->_operator != binary_op_expr::POWER)
		{
		const num_type &left(p->_left->evaluate(evaluate_expr()));
		const num_type &right(p->_right->evaluate(evaluate_expr()));
		switch (p->_operator)
			{
			case binary_op_expr::ADD:
				binary_add(*p->_result, left, right); break;
			case binary_op_expr::SUB:
				binary_sub(*p->_result, left, right); break;
			case binary_op_expr::MULTIPLY:
				binary_mul(*p->_result, left, right); break;
			case binary_op_expr::DIVIDE:
				binary_div(*p->_result, left, right); break;
			case binary_op_expr::MODULO:
				binary_mod(*p->_result, left, right); break;
			case binary_op_expr::POWER: break; // Handled above.
			case binary_op_expr::EQUAL_EQUAL:
				*p->_result = (left == right); break;
			case binary_op_expr::NOT_EQUAL:
				*p->_result = (left != right); break;
			case binary_op_expr::EQUAL_EQUAL_EQUAL:
				*p->_result = compare_c(left, right); break;
			case binary_op_expr::NOT_EQUAL_EQUAL:
				*p->_result = !compare_c(left, right); break;
			case binary_op_expr::AND_AND:
				*p->_result = (left && right); break;
			case binary_op_expr::OR_OR:
				*p->_result = (left || right); break;
			case binary_op_expr::LESS_THAN:
				*p->_result = (left < right); break;
			case binary_op_expr::LESS_EQUAL:
				*p->_result = (left <= right); break;
			case binary_op_expr::GRT_THAN:
				*p->_result = (left > right); break;
			case binary_op_expr::GRT_EQUAL:
				*p->_result = (left >= right); break;
			case binary_op_expr::LOGIC_AND:
				logic_and(*p->_result, left, right); break;
			case binary_op_expr::LOGIC_NAND:
				logic_nand(*p->_result, left, right); break;
			case binary_op_expr::LOGIC_OR:
				logic_or(*p->_result, left, right); break;
			case binary_op_expr::LOGIC_NOR:
				logic_nor(*p->_result, left, right); break;
			case binary_op_expr::LOGIC_XOR:
				logic_xor(*p->_result, left, right); break;
			case binary_op_expr::LOGIC_XNOR:
				logic_xnor(*p->_result, left, right); break;
			case binary_op_expr::LOGIC_NXOR:
				logic_xnor(*p->_result, left, right); break;
			case binary_op_expr::LEFT_SHIFT:
				binary_lshf(*p->_result, left, right); break;
			case binary_op_expr::RIGHT_SHIFT:
				binary_rshf(*p->_result, left, right); break;
			}
		}
	return *p->_result;
	}

const evaluate_expr::num_type &
evaluate_expr::operator()(const ternary_op_expr *p) const
	{
	const num_type &expr(p->_expr->evaluate(evaluate_expr()));
	const num_type zero(LO);
	num_type::logic_type non_zero = (expr > zero);
	switch (static_cast<int>(non_zero))
		{
		case 0:
			*p->_result = p->_false_expr->evaluate(evaluate_expr());
			break;
		case 1:
			*p->_result = p->_true_expr->evaluate(evaluate_expr());
			break;
		default:
			const num_type &t(p->_true_expr->evaluate(evaluate_expr()));
			const num_type &f(p->_false_expr->evaluate(evaluate_expr()));
			ternary_cmp(*p->_result, t, f);
			break;
		}
	p->_result->set_tristate();
	return *p->_result;
	}

const evaluate_expr::num_type &
evaluate_expr::operator()(const mintypmax_expr *p) const
	{
	switch (mintypmax_expr::_delay)
		{
		case mintypmax_expr::MINDELAY:
			if (p->_min_expr != 0)
				*p->_result = p->_min_expr->evaluate(evaluate_expr());
			break;
		case mintypmax_expr::TYPDELAY:
			if (p->_typ_expr != 0)
				*p->_result = p->_typ_expr->evaluate(evaluate_expr());
			break;
		case mintypmax_expr::MAXDELAY:
			if (p->_max_expr != 0)
				*p->_result = p->_max_expr->evaluate(evaluate_expr());
			break;
		default:
			// Non mintypmax expression.
			*p->_result = p->_min_expr->evaluate(evaluate_expr());
			break;
		}
	return *p->_result;
	}
