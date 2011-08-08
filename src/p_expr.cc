// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// p_expr.cc

#include <cstdlib> // atoi, etc.
#include <cstring> // strtok, etc.
#include "common/bvector.h"
#include "expr/funccall.h"
#include "expr/concat.h"
#include "expr/number.h"
#include "expr/qstr.h"
#include "expr/rangeid.h"
#include "expr/unary_op.h"
#include "expr/binary_op.h"
#include "expr/ternary_op.h"
#include "expr/mintypmax.h"
#include "parser.h"
#include "p_expr.h"
#include "vbs_yacc.h"

#include "common/debug.h"

p_expression_list
p_create_expression_list(p_expression_list lst, p_expression exp)
	{
	typedef expr_base::list_type< basic_ptr<expr_base> > expr_list;
	expr_list *ret;
	if (lst != 0)
		ret = (expr_list *) lst;
	else
		ret = new expr_list;
	if (exp != 0)
		{
		ret->push_back((expr_base *) exp);
		DEBUG_STATE(DEBUG_PARSER);
		DEBUG_OUTPUT("PARSER:  added expression to list.\n");
		}
	return ret;
	}

p_range_identifier_list
p_create_range_identifier_list
		(
		p_range_identifier_list idlst,
		p_range_identifier id
		)
	{
	typedef range_id::list_type< basic_ptr<range_id> > range_id_list;
	range_id_list *ret;
	if (idlst != 0)
		ret = (range_id_list *)idlst;
	else
		ret = new range_id_list;
	if (id != 0)
		{
		ret->push_back((range_id *) id);
		DEBUG_STATE(DEBUG_PARSER);
		DEBUG_OUTPUT("PARSER:  added range id to list.\n");
		}
	return ret;
	}

p_range_identifier
p_create_range_identifier(char *n, p_select r)
	{
	range_id *ret;
	ret = new range_id(n, (select_base *) r);
	ret->_lineno = cur_lineno;
	free(n);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created range id.\n");
	return ret;
	}

p_qouted_string
p_create_qouted_string(char *str)
	{
	qouted_str *ret;

	// Check for octal representations.
	int len = strlen(str);
	for (int i = 0; i < len; ++i)
		{
		if (str[i] == '\\' && (i+1) < len)
			{
			if (str[i+1] >= '0' && str[i+1] <= '9')
				{
				static char cs[] = {' ','o','c','t','a','l',
									' ','s','t','r','i','n','g',
									' ','c','o','n','s','t','a','n','t',
									' '};
				yyerror(cs);
				break;
				}
			}
		}

	ret = new qouted_str(str);
	ret->_lineno = cur_lineno;
	free(str);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created qouted string.\n");
	return ret;
	}

p_number
p_create_number(char *s)
	{
	bool negative = false;
	bit_vector::size_type len = 0;
	bit_vector::base_type type = bit_vector::BASE2;
	char *str, *siz, *num, base;
	number *ret;

	// Handle signed constant literals.
	str = strchr(s, '-');
	if (str != 0)
		{
		negative = true;
		++str;
		}
	else
		str = s;

	if (strchr(str, '\'') == 0)
		{
		base = 'd';
		num = str;
		}
	else
		{
		int idx = 1;
		if (str[0] == '\'')
			{
			if (*(str + 1) == 's')
				{
				negative = true;
				idx++;
				}
			len = 32; // Unsized constants must be at least 32bits.
			base = *(str + idx++);
			num = str + idx;
			}
		else
			{
			char *tmp = vbs_strdup(str);
			siz = strtok(tmp, "\'");
			if (*(str + strlen(siz) + 1) == 's')
				{
				negative = true;
				idx++;
				}
			len = negative ? 32 : atoi(siz);
			base = *(str + strlen(siz) + idx++);
			num = str + strlen(siz) + idx;
			free(tmp);
			}
		}
	switch(base)
		{
		case 'b': case 'B': type = bit_vector::BASE2; break;
		case 'o': case 'O': type = bit_vector::BASE8; break;
		case 'd': case 'D': type = bit_vector::BASE10; break;
		case 'h': case 'H': type = bit_vector::BASE16; break;
		}
	ret = new number(num, type, len, negative);
	ret->_lineno = cur_lineno;
	if (negative)
		{
		number tmp(*ret);
		unary_inv(tmp, *ret);
		binary_add(*ret, tmp, number("1", bit_vector::BASE10, 1, false));
		}
	free(s);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created number.\n");
	return ret;
	}

p_function_call
p_create_function_call(char *name, p_expression_list exp)
	{
	function_call *ret;
	ret = new function_call(name, (function_call::arg_list *) exp);
	ret->_lineno = cur_lineno;
	free(name);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created function call.\n");
	return ret;
	}

p_concatenation
p_create_concatenation(p_expression rpt, p_expression_list exp)
	{
	concatenation *ret;
	ret = new concatenation((concatenation::expr_list *) exp, (expr_base *)rpt);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	if (rpt == 0)
		DEBUG_OUTPUT("PARSER:  created concatenation.\n");
	else
		DEBUG_OUTPUT("PARSER:  created concatenation (with repeat).\n");
	return ret;
	}

p_unary_op_expr
p_create_unary_op_expr(int type, p_expression exp)
	{
	unary_op_expr *ret;
	unary_op_expr::operator_type op;
	switch (type)
		{
		case YYPLUS: op = unary_op_expr::PLUS_EXPR; break;
		case YYMINUS: op = unary_op_expr::MINUS_EXPR; break;
		case YYEXCLAM: op = unary_op_expr::NOT_EXPR; break;
		case YYTILDE: op = unary_op_expr::INVERT_EXPR; break;
		case YYAMPER: op = unary_op_expr::AND_EXPR; break;
		case YYTILDEAMPER: op = unary_op_expr::NAND_EXPR; break;
		case YYVERTBAR: op = unary_op_expr::OR_EXPR; break;
		case YYTILDEVERTBAR: op = unary_op_expr::NOR_EXPR; break;
		case YYCARET: op = unary_op_expr::XOR_EXPR; break;
		case YYTILDECARET: op = unary_op_expr::XNOR_EXPR; break;
		case YYCARETTILDE: op = unary_op_expr::NXOR_EXPR; break;
		default: return 0;
		}
	ret = new unary_op_expr(op, (expr_base *) exp);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created unary operator expression.\n");
	return ret;
	}

p_binary_op_expr
p_create_binary_op_expr(int type, p_expression l, p_expression r)
	{
	binary_op_expr *ret;
	binary_op_expr::operator_type op;
	switch (type)
		{
		case YYPLUS: op = binary_op_expr::ADD; break;
		case YYMINUS: op = binary_op_expr::SUB; break;
		case YYASTERIC: op = binary_op_expr::MULTIPLY; break;
		case YYSLASH: op = binary_op_expr::DIVIDE; break;
		case YYPERCENT: op = binary_op_expr::MODULO; break;
		case YYEQUALEQUAL: op = binary_op_expr::EQUAL_EQUAL; break;
		case YYEXCLAMEQUAL: op = binary_op_expr::NOT_EQUAL; break;
		case YYEQUALEQUALEQUAL: op = binary_op_expr::EQUAL_EQUAL_EQUAL; break;
		case YYEXCLAMEQUALEQUAL: op = binary_op_expr::NOT_EQUAL_EQUAL; break;
		case YYAMPERAMPER: op = binary_op_expr::AND_AND; break;
		case YYVERTBARVERTBAR: op = binary_op_expr::OR_OR; break;
		case YYLESS: op = binary_op_expr::LESS_THAN; break;
		case YYLESSEQUAL: op = binary_op_expr::LESS_EQUAL; break;
		case YYGREATER: op = binary_op_expr::GRT_THAN; break;
		case YYGREATEREQUAL: op = binary_op_expr::GRT_EQUAL; break;
		case YYAMPER: op = binary_op_expr::LOGIC_AND; break;
		case YYTILDEAMPER: op = binary_op_expr::LOGIC_NAND; break;
		case YYVERTBAR: op = binary_op_expr::LOGIC_OR; break;
		case YYTILDEVERTBAR: op = binary_op_expr::LOGIC_NOR; break;
		case YYCARET: op = binary_op_expr::LOGIC_XOR; break;
		case YYTILDECARET: op = binary_op_expr::LOGIC_XNOR; break;
		case YYCARETTILDE: op = binary_op_expr::LOGIC_NXOR; break;
		case YYLESSLESS: op = binary_op_expr::LEFT_SHIFT; break;
		case YYGREATERGREATER: op = binary_op_expr::RIGHT_SHIFT; break;
		default: return 0;
		}
	ret = new binary_op_expr(op, (expr_base *) l, (expr_base *) r);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created binary operator expression.\n");
	return ret;
	}

p_ternary_op_expr
p_create_ternary_op_expr(p_expression e, p_expression t, p_expression f)
	{
	ternary_op_expr *ret;
	ret = new ternary_op_expr((expr_base *) e,
		(expr_base *) t, (expr_base *) f);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created ternary operator expression.\n");
	return ret;
	}

p_mintypmax_expr
p_create_mintypmax_expr(p_expression min, p_expression typ, p_expression max)
	{
	mintypmax_expr *ret;
	ret = new mintypmax_expr((expr_base *) min,
		(expr_base *) typ, (expr_base *) max);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created mintypmax expression.\n");
	return ret;
	}
