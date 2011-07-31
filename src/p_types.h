/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * p_types.h
 *
 * This file contains all the typedef's used by the p_*.h header files.
 * The reason why they are here is because the objects became complex
 * enough to be interdependent on each other.  Thus, we need to typedef
 * all the types in one location.
 */

#ifndef _P_TYPES_H
#define _P_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Types used in expression objects. */

typedef void *p_select;
typedef void *p_bit_select;
typedef void *p_part_select;

typedef void *p_expression;
typedef void *p_expression_list;
typedef void *p_constant_expression;
typedef void *p_range_identifier;
typedef void *p_range_identifier_list;
typedef void *p_qouted_string;
typedef void *p_number;

typedef void *p_add;
typedef void *p_subtract;

typedef void *p_logand;
typedef void *p_logor;
typedef void *p_logxor;
typedef void *p_left_shift;
typedef void *p_right_shift;

typedef void *p_equal_equal;
typedef void *p_not_equal;
typedef void *p_less_than;
typedef void *p_less_equal;
typedef void *p_greater_than;
typedef void *p_greater_equal;

typedef void *p_or_or;
typedef void *p_and_and;

typedef void *p_function_call;
typedef void *p_concatenation;
typedef void *p_mintypmax_expr;

typedef void *p_unary_op_expr;
typedef void *p_binary_op_expr;
typedef void *p_ternary_op_expr;


/* Types used in statement objects. */

typedef void *p_lvalue;
typedef void *p_case_item;
typedef void *p_case_item_list;

typedef void *p_statement;
typedef void *p_statement_list;
typedef void *p_seq_block;
typedef void *p_task_enable;
typedef void *p_blocking_assignment;
typedef void *p_non_blocking_assignment;
typedef void *p_if_else_statement;
typedef void *p_case_statement;
typedef void *p_loop_statement;

typedef void *p_initial_construct;
typedef void *p_always_construct;


/* Types used in module item objects. */

typedef void *p_module_item_list;
typedef void *p_module_item;
typedef void *p_tfdecl_list;
typedef void *p_tfdecl;
typedef void *p_param_declaration;
typedef void *p_io_declaration;
typedef void *p_net_declaration;
typedef void *p_reg_declaration;
typedef void *p_int_declaration;
typedef void *p_function_declaration;
typedef void *p_task_declaration;
typedef void *p_module_instantiation;
typedef void *p_continuous_assignment;

typedef void *p_decl_assign_list;
typedef void *p_decl_assign;


/* Types used with module instantiation. */

typedef void *p_port_connection;
typedef void *p_port_connection_list;
typedef void *p_module_instance;
typedef void *p_module_instance_list;
typedef void *p_blocking_assignment_list;


/* Types used in misc. objects. */

typedef void *p_module;
typedef void *p_port_list;
typedef void *p_port;

typedef void *p_event_expression;
typedef void *p_event_expression_list;
typedef void *p_delay_or_event_control;


#ifdef __cplusplus
}
#endif

#endif /* _P_TYPES_H */
