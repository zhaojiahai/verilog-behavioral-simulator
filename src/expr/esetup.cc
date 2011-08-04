// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// esetup.cc

#include "common/error.h"
#include "common/sym_tab.h"
#include "common/st_func.h"
#include "common/st_net.h"
#include "common/st_inst.h"
#include "common/st_setup.h"
#include "common/st_util.h"
#include "expr/esetup.h"
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
#include "misc/msetup.h"
#include "vbs.h"


setup_expr::size_type
setup_expr::operator()(function_call *p) const
	{
	if (_check_const)
		{
		vbs_err.set_data(vbs_error::SE_TYPE, p->_lineno);
		vbs_err.out(p->_name);
		}

	// First find the index.
	scopelist_type::iterator modscp(_scope.end());
	--modscp; --modscp; // Module scope.
	scopelist_type::value_type save, tmp = *modscp;
	p->_index = st_node_find(p->_name.c_str(), _scope, &tmp);
	if (p->_index._scope < 0)
		{
		if (p->_name[0] == '$')
			p->_index._value = vbs_error::SE_TFSUPPORT;
		vbs_err.set_data((vbs_error::value_type) p->_index._value, p->_lineno);
		vbs_err.out(p->_name);
		}

	// Make sure it's a function.
	symbol_table &symboltable = vbs_engine::symboltable();
	func_type *node = symboltable.get(p->_index)->get_function();
	if (node == 0)
		{
		vbs_err.set_data(vbs_error::SE_NFUNCTION, p->_lineno);
		vbs_err.out(p->_name);
		}
	// Functions are not setup until the function is called.  This is a
	// function call, so setup...
	save = *modscp;
	if (tmp != *modscp)
		*modscp = tmp; // Use instance scope if hierarchical name.
	node->setup(setup_st_node(_scope, p->_argument, _parent));
	if (*modscp != save)
		*modscp = save; // Restore original scope.

	// Now check argument list.
	if (p->_name[0] == '$')
		{
		// Arguments are optional to system functions.
		}
	else
		{
		// User defined functions must have at least one argument.
		vbs_error::value_type er;
		if (p->_argument == 0)
			er = vbs_error::SE_NARGLST;
		else if (!check_arguments(node->_iovariable, p->_argument))
			er = vbs_error::SE_NARGLST;
		else
			er = vbs_error::SE_NONE;
		if (er != vbs_error::SE_NONE)
			{
			vbs_err.set_data(er, p->_lineno);
			vbs_err.out(p->_name);
			}
		}
	// Setup the argument expressions (seed to identifiers).
	if (p->_argument != 0)
		{
		arg_list::iterator itp(p->_argument->begin());
		arg_list::iterator stop(p->_argument->end());
		for (; itp != stop; ++itp)
			(*itp)->setup(setup_expr(_scope));
		}

	p->_result = new num_type(node->size() - 1, 0);
	return p->_result->size();
	}

setup_expr::size_type
setup_expr::operator()(concatenation *p) const
	{
	concatenation::expr_list::reverse_iterator itp(p->_exp_list->rbegin());
	concatenation::expr_list::reverse_iterator stop(p->_exp_list->rend());
	concatenation::size_type size = 0;
	concatenation::size_type prev_size = 0;
	concatenation::size_type repeat_count = 1;

	// Repeat expression must be constant, save time by evaluating it now.
	if (p->_repeat_expr != 0)
		{
		p->_repeat_expr->setup(setup_expr(_scope, true));
		repeat_count = p->_repeat_expr->evaluate(evaluate_expr());
		}

	for (; itp != stop; ++itp)
		{
		size += (*itp)->setup(setup_expr(_scope, _check_const));
		p->_elmt_list.push_front(concatenation::element_type(size-1, prev_size));
		prev_size = size;
		}
	p->_repeat_count = repeat_count;
	size *= p->_repeat_count;
	p->_result = new num_type(size > 0 ? size - 1 : 0, 0);
	return p->_result->size();
	}

setup_expr::size_type
setup_expr::operator()(number *p) const
	{
	// When a number is used in a range select, it is setup multiple
	// times.  So check if we have already done setup.
	if (p->_result == 0)
		p->_result = new num_type(p->size() - 1, 0);
	return p->_result->size();
	}

setup_expr::size_type
setup_expr::operator()(qouted_str *p) const
	{
	p->_result = new num_type(p->_value.size() - 1, 0);
	return p->_result->size();
	}

setup_expr::size_type
setup_expr::operator()(range_id *p) const
	{
	p->_index = st_node_find(p->_name.c_str(), _scope);
// FIXME: must be some kind of extension used by a contributor (delete later).
#if 0
	{
#include "moditm/d_int.h"
	// This should be treated as a int with value calculated during
	// trigger, denoting the scope of the string.
	typedef range_id::list_type< basic_ptr<range_id> > ident_ptr_list;
	int_decl *i;
	ident_ptr_list *idlst;
	idlst = new ident_ptr_list;
	if (idlst != 0)
		idlst->push_back(p);
	i = new int_decl((ident_ptr_list *) idlst);
	i->_lineno = p->_lineno;
	i->setup(mitf_setup());
	return;
	}
#endif
	if (p->_index._scope < 0)
		{
		vbs_err.set_data((vbs_error::value_type) p->_index._value, p->_lineno);
		vbs_err.out(p->_name);
		}

	symbol_table &symboltable = vbs_engine::symboltable();
	net_type *net = symboltable.get(p->_index)->get_net();
	// In the case of an lvalue in an assignment, a function name could
	// also be an lvalue.
	func_type *func = symboltable.get(p->_index)->get_function();

	// Setup select type before obtaining the range values.
	if (p->_select != 0)
		p->_select->setup(setup_select(_scope));
	range_id::position_type ms, ls;
	range_id::select_type::SELECT_SIZE t = p->get_select(ms, ls);

	// The range depends on the type of identifier.
	vbs_error::value_type er = vbs_error::SE_NONE;
	size_type rs = 0;
	if (net != 0)
		{
		rs = net->size();
		switch (net->_type)
			{
			case net_type::PARAMETER:
#if 0
				// Parameters can not have selectors.
				if (t != range_id::select_type::SELECT_NONE)
					er = vbs_error::SE_TYPE;
#endif
				break;
			case net_type::INTEGER:
				// Integers are not constant.
				if (_check_const)
					er = vbs_error::SE_TYPE;
				break;
			case net_type::MEMORY:
				if (_check_const)
					er = vbs_error::SE_TYPE;
				else if (t == range_id::select_type::SELECT_PART)
					{
					// ms and ls should equal
					er = vbs_error::SE_TYPE;
					}
				break;
			default:
				if (_check_const)
					er = vbs_error::SE_TYPE;
				else
					{
					if (t != range_id::select_type::SELECT_NONE && ms != -1)
						{
						// Recalculate size with indices.
						rs = (*net->_storage)(ms,ls).size();
						if (rs == 0)
							er = vbs_error::SE_IDXORDER;
						}
					}
				break;
			}
		}
	else if (func != 0)
		{
		rs = func->size();
		if (_check_const)
			er = vbs_error::SE_TYPE;
		else
			{
			if (t != range_id::select_type::SELECT_NONE && ms != -1)
				{
				rs = func->_storage(ms, ls).size();
				if (rs == 0)
					er = vbs_error::SE_IDXORDER;
				}
			}
		}
	else
		er = vbs_error::SE_TYPE;

	// Display error if any.
	if (er != vbs_error::SE_NONE)
		{
		vbs_error::strstream_type buf;
		buf << *p;
		vbs_err.set_data(er, p->_lineno);
		vbs_err.out(buf);
		}

	p->_result = new num_type(rs - 1, 0);
	return p->_result->size();
	}

setup_expr::size_type
setup_expr::operator()(unary_op_expr *p) const
	{
	size_type s = p->_expr->setup(setup_expr(_scope, _check_const, _parent, _result_size));
	size_type size = 0, rs = 0;
	switch (p->_operator)
		{
		case unary_op_expr::PLUS_EXPR:
		case unary_op_expr::MINUS_EXPR:
			rs = s;
			size = rs < _result_size ? _result_size : rs;
			break;
		case unary_op_expr::NOT_EXPR:
			size = rs = 1;
			break;
		case unary_op_expr::INVERT_EXPR:
			size = rs = s;
			break;
		case unary_op_expr::AND_EXPR:
		case unary_op_expr::NAND_EXPR:
		case unary_op_expr::OR_EXPR:
		case unary_op_expr::NOR_EXPR:
		case unary_op_expr::XOR_EXPR:
		case unary_op_expr::XNOR_EXPR:
		case unary_op_expr::NXOR_EXPR:
			size = rs = 1;
			break;
		}
	p->_result = new num_type(size - 1, 0);
	return p->_result->size();
	}

setup_expr::size_type
setup_expr::operator()(binary_op_expr *p) const
	{
	size_type ls = p->_left->setup(setup_expr(_scope, _check_const, _parent, _result_size));
	size_type rs = p->_right->setup(setup_expr(_scope, _check_const, _parent, _result_size));
	size_type size = 0, s = 0;
	switch (p->_operator)
		{
		case binary_op_expr::ADD:
		case binary_op_expr::SUB:
			s = ls > rs ? ls : rs;
			size = s < _result_size ? _result_size : s;
			break;
		case binary_op_expr::MULTIPLY:
			s = ls + rs;
			size = s < _result_size ? _result_size : s;
			break;
		case binary_op_expr::DIVIDE:
		case binary_op_expr::MODULO:
			s = ls > rs ? ls : rs;
			size = s < _result_size ? _result_size : s;
			break;
		case binary_op_expr::EQUAL_EQUAL:
		case binary_op_expr::NOT_EQUAL:
		case binary_op_expr::EQUAL_EQUAL_EQUAL:
		case binary_op_expr::NOT_EQUAL_EQUAL:
		case binary_op_expr::AND_AND:
		case binary_op_expr::OR_OR:
		case binary_op_expr::LESS_THAN:
		case binary_op_expr::LESS_EQUAL:
		case binary_op_expr::GRT_THAN:
		case binary_op_expr::GRT_EQUAL:
			size = s = 1;
			break;
		case binary_op_expr::LOGIC_AND:
		case binary_op_expr::LOGIC_NAND:
		case binary_op_expr::LOGIC_OR:
		case binary_op_expr::LOGIC_NOR:
		case binary_op_expr::LOGIC_XOR:
		case binary_op_expr::LOGIC_XNOR:
		case binary_op_expr::LOGIC_NXOR:
			s = ls > rs ? ls : rs;
			size = s < _result_size ? _result_size : s;
			break;
		case binary_op_expr::LEFT_SHIFT:
		case binary_op_expr::RIGHT_SHIFT:
			s = ls;
			size = s < _result_size ? _result_size : s;
			break;
		}
	p->_result = new num_type(size - 1, 0);
	return p->_result->size();
	}

setup_expr::size_type
setup_expr::operator()(ternary_op_expr *p) const
	{
	p->_expr->setup(setup_expr(_scope, _check_const, _parent, _result_size));
	size_type ts = p->_true_expr->setup(setup_expr(_scope, _check_const, _parent, _result_size));
	size_type fs = p->_false_expr->setup(setup_expr(_scope, _check_const, _parent, _result_size));
	size_type rs = ts > fs ? ts : fs;
	size_type size = rs < _result_size ? _result_size : rs;
	p->_result = new num_type(size - 1, 0);
	return p->_result->size();
	}

setup_expr::size_type
setup_expr::operator()(mintypmax_expr *p) const
	{
	size_type rs = 0;
	switch (mintypmax_expr::_delay)
		{
		case mintypmax_expr::MINDELAY:
			if (p->_min_expr != 0)
				rs = p->_min_expr->setup(setup_expr(_scope, _check_const));
			break;
		case mintypmax_expr::TYPDELAY:
			if (p->_typ_expr != 0)
				rs = p->_typ_expr->setup(setup_expr(_scope, _check_const));
			break;
		case mintypmax_expr::MAXDELAY:
			if (p->_max_expr != 0)
				rs = p->_max_expr->setup(setup_expr(_scope, _check_const));
			break;
		default:
			// Non mintypmax expression.
			rs = p->_min_expr->setup(setup_expr(_scope, _check_const));
			break;
		}
	p->_result = new num_type(rs - 1, 0);
	return p->_result->size();
	}
