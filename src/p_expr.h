/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * p_expr.h
 *
 * Parser functions to create expression objects.
 */

#ifndef _P_EXPR_H
#define _P_EXPR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "p_types.h"


	/*
	 * Expressions.
	 */


p_expression_list
p_create_expression_list(p_expression_list, p_expression);

p_range_identifier_list
p_create_range_identifier_list(p_range_identifier_list, p_range_identifier);

p_range_identifier
p_create_range_identifier(char *, p_select);

p_range_identifier
p_extract_range_identifier(p_concatenation);

p_qouted_string
p_create_qouted_string(char *);

p_number
p_create_number(char *);

p_function_call
p_create_function_call(char *, p_expression_list);

p_concatenation
p_create_concatenation(p_expression, p_expression_list);

p_unary_op_expr
p_create_unary_op_expr(int, p_expression);

p_binary_op_expr
p_create_binary_op_expr(int, p_expression, p_expression);

p_ternary_op_expr
p_create_ternary_op_expr(p_expression, p_expression, p_expression);

p_mintypmax_expr
p_create_mintypmax_expr(p_expression, p_expression, p_expression);

#ifdef __cplusplus
}
#endif

#endif /* _P_EXPR_H */
