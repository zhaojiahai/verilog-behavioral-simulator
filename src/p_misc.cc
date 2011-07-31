// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// p_misc.cc

#include "expr/number.h" // delay num.
#include "expr/rangeid.h" // event id.
#include "misc/module.h"
#include "misc/port.h"
#include "misc/delaynum.h"
#include "misc/delayid.h"
#include "misc/evntexpr.h"
#include "misc/bitsel.h"
#include "misc/partsel.h"
#include "misc/lvalue.h"
#include "moditm/mibase.h"
#include "parser.h"
#include "p_misc.h"

#include "common/debug.h"


	/*
	 * Module declaration.
	 */


p_module
p_create_module(char *name, p_module_item_list milst, p_port_list plst)
	{
	module *ret;

	// If the port list is empty, do not bother keeping it.
	module::port_list *lst = (module::port_list *)plst;
	if (lst != 0 && lst->size() == 0)
		{
		delete lst;
		lst = 0;
		}

	ret = new module(name,
		(module::module_item_list *)milst, lst);
	ret->_filename = cur_filename;
	ret->_lineno = cur_lineno;
	free(name);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created a module.\n");
	return ret;
	}

p_port_list
p_create_port_list(p_port_list plst, p_port p)
	{
	typedef port::list_type< basic_ptr<port> > port_list;
	port_list *ret;
	if (plst != 0)
		ret = (port_list *)plst;
	else
		ret = new port_list;
	if (p != 0)
		{
		ret->push_back((port *)p);
		DEBUG_STATE(DEBUG_PARSER);
		DEBUG_OUTPUT("PARSER:  added port to list.\n");
		}
	return ret;
	}

p_port
p_create_port(char *name)
	{
	port *ret;
	ret = new port(name);
	ret->_lineno = cur_lineno;
	free(name);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created port.\n");
	return ret;
	}


	/*
	 * Delay or event control.
	 */


p_delay_or_event_control
p_create_delay_number(p_number num)
	{
	delay_num *ret;
	bit_vector *amt = (number *)num;
	ret = new delay_num(*amt);
	ret->_lineno = cur_lineno;
	delete amt;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created delay by const number.\n");
	return ret;
	}

p_delay_or_event_control
p_create_delay_identifier(p_expression exp)
	{
	delay_id *ret;
	ret = new delay_id((expr_base *)exp);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created delay by expression.\n");
	return ret;
	}

p_delay_or_event_control
p_create_event_identifier(char *id)
	{
	event_expr *evntexpr;
	range_id *rid = new range_id(id, 0);
	evntexpr = (event_expr *)p_create_change_event_expression(rid);
	ored_event_expr::event_expr_list *lst = (ored_event_expr::event_expr_list *)
		p_create_event_expression_list(0, evntexpr);
	ored_event_expr *ret = (ored_event_expr *)p_create_or_event_expression(lst);
	free(id);
	return ret;
	}

p_delay_or_event_control
p_create_or_event_expression(p_event_expression_list lst)
	{
	ored_event_expr *ret;
	ret = new ored_event_expr((ored_event_expr::event_expr_list *)lst);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created ored event expression.\n");
	return ret;
	}

p_event_expression
p_create_change_event_expression(p_expression exp)
	{
	event_expr *ret;
	ret = new event_expr((expr_base *)exp, TRIG_CHNG);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created event expression (change).\n");
	return ret;
	}

p_event_expression
p_create_posedge_event_expression(p_expression exp)
	{
	event_expr *ret;
	ret = new event_expr((expr_base *)exp, TRIG_POSEDGE);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created event expression (posedge).\n");
	return ret;
	}

p_event_expression
p_create_negedge_event_expression(p_expression exp)
	{
	event_expr *ret;
	ret = new event_expr((expr_base *)exp, TRIG_NEGEDGE);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created event expression (negedge).\n");
	return ret;
	}

p_event_expression_list
p_create_event_expression_list
		(
		p_event_expression_list lst,
		p_event_expression exp
		)
	{
	typedef event_expr::list_type< basic_ptr<event_expr> > event_expr_list;
	event_expr_list *ret;
	if (lst != 0)
		ret = (event_expr_list *)lst;
	else
		ret = new event_expr_list;
	ret->push_back((event_expr *)exp);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  added event expression to list.\n");
	return ret;
	}


	/*
	 * Others.
	 */


p_bit_select
p_create_bit_select(p_expression exp)
	{
	bit_select *ret;
	ret = new bit_select((expr_base *) exp);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created bit select.\n");
	return ret;
	}

p_part_select
p_create_part_select(p_expression l, p_expression r)
	{
	part_select *ret;
	ret = new part_select((expr_base *) l, (expr_base *) r);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created part select.\n");
	return ret;
	}

p_lvalue
p_create_lvalue(p_expression_list elst)
	{
	lvalue *ret;
	ret = new lvalue((lvalue::expr_list *) elst);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created lvalue.\n");
	return ret;
	}
