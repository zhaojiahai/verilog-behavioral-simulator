/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * p_misc.h
 *
 * Parser functions to create misc. objects.
 */

#ifndef _P_MISC_H
#define _P_MISC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "p_types.h"


	/*
	 * Module declaration.
	 */


p_module
p_create_module(char *, p_module_item_list, p_port_list);

p_port_list
p_create_port_list(p_port_list, p_port);

p_port
p_create_port(char *);


	/*
	 * Delay or event control.
	 */


p_delay_or_event_control
p_create_delay_number(p_number);

p_delay_or_event_control
p_create_delay_identifier(p_expression);

p_delay_or_event_control
p_create_event_identifier(char *);

p_delay_or_event_control
p_create_or_event_expression(p_event_expression_list);

p_event_expression
p_create_change_event_expression(p_expression);

p_event_expression
p_create_posedge_event_expression(p_expression);

p_event_expression
p_create_negedge_event_expression(p_expression);

p_event_expression_list
p_create_event_expression_list(p_event_expression_list, p_event_expression);


	/*
	 * Others.
	 */


p_constant_expression
p_create_constant_expression(p_expression);

p_bit_select
p_create_bit_select(p_expression);

p_part_select
p_create_part_select(p_expression, p_expression);

p_lvalue
p_create_lvalue(p_expression_list);


#ifdef __cplusplus
}
#endif

#endif /* _P_MISC_H */
