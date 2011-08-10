/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2002,2011 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * p_moditm.h
 *
 * Parser functions to create module item objects.
 */

#ifndef _P_MODITM_H
#define _P_MODITM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "p_types.h"


	/*
	 * Module instantiation.
	 */


p_port_connection
p_create_port_connection(p_expression, const char *);

p_port_connection_list
p_create_port_connection_list(p_port_connection_list, p_port_connection);

p_module_instance
p_create_module_instance(char *, p_port_connection_list);

p_module_instance_list
p_create_module_instance_list(p_module_instance_list, p_module_instance);

p_tfdecl_list
p_create_tfdecl_list(p_tfdecl_list, p_tfdecl);

p_module_item_list
p_create_module_item_list(p_module_item_list, p_module_item);


	/*
	 * Module items.
	 */


p_param_declaration
p_create_param_declaration(p_decl_assign_list, p_part_select);

p_io_declaration
p_create_io_declaration(int, p_range_identifier_list, p_part_select);

p_reg_declaration
p_create_reg_declaration(p_decl_assign_list, p_part_select);

p_int_declaration
p_create_int_declaration(p_decl_assign_list idlst);

p_net_declaration
p_create_net_declaration(int, p_decl_assign_list, p_part_select);

p_decl_assign_list
p_create_decl_assign_list(p_decl_assign_list, p_decl_assign);

p_decl_assign
p_create_decl_assign(p_lvalue, p_expression);

p_function_declaration
p_create_function_declaration
	(
	char *,
	p_statement,
	p_tfdecl_list,
	p_part_select
	);

p_task_declaration
p_create_task_declaration
	(
	char *,
	p_statement,
	p_tfdecl_list
	);

p_initial_construct
p_create_initial_construct(p_statement);

p_always_construct
p_create_always_construct(p_statement);

p_module_instantiation
p_create_module_instantiation(
	char *,
	p_module_instance_list,
	p_expression_list
	);

p_continuous_assignment
p_create_continuous_assignment
	(
	p_blocking_assignment_list,
	p_delay_or_event_control
	);

p_blocking_assignment_list
p_create_blocking_assignment_list
	(
	p_blocking_assignment_list,
	p_blocking_assignment);


#ifdef __cplusplus
}
#endif

#endif /* _P_MODITM_H */
