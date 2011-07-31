/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * p_stmt.h
 *
 * Parser functions to create statement objects.
 */

#ifndef _P_STMTS_H
#define _P_STMTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "p_types.h"

p_case_item
p_create_case_item(p_expression_list, p_statement);

p_case_item_list
p_create_case_item_list(p_expression, p_case_item_list);

p_statement_list
p_create_statement_list(p_statement_list, p_statement);

p_statement
p_create_null_statement(void);

p_seq_block
p_create_seq_block(p_statement_list);

p_task_enable
p_create_task_enable(char *, p_expression_list);

p_blocking_assignment
p_create_blocking_assignment(p_lvalue, p_expression, p_delay_or_event_control);

p_non_blocking_assignment
p_create_non_blocking_assignment(p_lvalue, p_expression, p_delay_or_event_control);

p_if_else_statement
p_create_if_else_statement(p_expression, p_statement, p_statement);

p_case_statement
p_create_case_statement(int, p_expression, p_case_item_list);

p_loop_statement
p_create_loop_statement
	(
	int,
	p_statement,
	p_expression,
	p_blocking_assignment,
	p_blocking_assignment
	);

p_statement
p_create_include_delay_or_event_control(p_statement, p_delay_or_event_control);

#ifdef __cplusplus
}
#endif

#endif /* _P_STMTS_H */
