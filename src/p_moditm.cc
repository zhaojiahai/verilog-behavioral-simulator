// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// p_moditm.cc

#include "stmt/stmtbase.h"
#include "moditm/initial.h"
#include "moditm/always.h"
#include "moditm/func.h"
#include "moditm/task.h"
#include "moditm/modinst.h"
#include "moditm/cassign.h"
#include "moditm/d_io.h"
#include "moditm/d_net.h"
#include "moditm/d_reg.h"
#include "moditm/d_int.h"
#include "moditm/d_param.h"
#include "parser.h"
#include "p_moditm.h"
#include "vbs_yacc.h"

#include "common/debug.h"

// Module item objects.

p_module_item_list
p_create_module_item_list(p_module_item_list milst, p_module_item mi)
	{
	typedef module_item_base::list_type< basic_ptr<module_item_base> >
		module_item_list;
	module_item_list *ret;
	if (milst != 0)
		ret = (module_item_list *)milst;
	else
		ret = new module_item_list;
	if (mi != 0)
		{
		// Put task/function declaration in front so it is setup first.
		// This is needed because the task/function name must be in the
		// symbol table before this task/function can be enabled.
		// This changes the order of the declaration, so the displaying
		// of a module will not show the order in the source file.  FIXME!
		module_item_base *tmp = (module_item_base *)mi;
		if (tmp->get_function() || tmp->get_task())
			ret->push_front(tmp);
		else
			ret->push_back(tmp);
		DEBUG_STATE(DEBUG_PARSER);
		DEBUG_OUTPUT("PARSER:  added module item to list.\n");
		}
	return ret;
	}

p_param_declaration
p_create_param_declaration(p_decl_assign_list idlst, p_part_select r)
	{
	param_decl *ret;
	ret = new param_decl((param_decl::decl_assign_list *)idlst, (part_select *)r);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created parameter declaration.\n");
	return ret;
	}

p_io_declaration
p_create_io_declaration(int t, p_range_identifier_list idlst, p_part_select r)
	{
	io_decl *ret;
	switch (t)
		{
		case YYINPUT:
			ret = new io_decl
				(
				io_decl::INPUT,
				(io_decl::ident_list *) idlst,
				(part_select *) r
				);
			break;
		case YYOUTPUT:
			ret = new io_decl
				(
				io_decl::OUTPUT,
				(io_decl::ident_list *) idlst,
				(part_select *) r
				);
			break;
		case YYINOUT:
			ret = new io_decl
				(
				io_decl::INOUT,
				(io_decl::ident_list *) idlst,
				(part_select *) r
				);
			break;
		default:
			return 0;
		}
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created io declaration.\n");
	return ret;
	}

p_reg_declaration
p_create_reg_declaration(p_decl_assign_list idlst, p_part_select r)
	{
	reg_decl *ret;
	ret = new reg_decl((reg_decl::decl_assign_list *) idlst, (part_select *) r);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created register declaration.\n");
	return ret;
	}

p_int_declaration
p_create_int_declaration(p_decl_assign_list idlst)
	{
	int_decl *ret;
	ret = new int_decl((int_decl::decl_assign_list *) idlst);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created integer declaration.\n");
	return ret;
	}

p_net_declaration
p_create_net_declaration(int t, p_decl_assign_list idlst, p_part_select r)
	{
	net_decl *ret;
	net_decl::declaration_type type = net_decl::NONE;
	switch (t)
		{
		case YYWIRE:
			type = net_decl::WIRE;
			break;
		case YYTRI:
			type = net_decl::TRI;
			break;
		case YYTRI1:
			type = net_decl::TRI1;
			break;
		case YYSUPPLY0:
			type = net_decl::SUPPLY0;
			break;
		case YYWAND:
			type = net_decl::WAND;
			break;
		case YYTRIAND:
			type = net_decl::TRIAND;
			break;
		case YYTRI0:
			type = net_decl::TRI0;
			break;
		case YYSUPPLY1:
			type = net_decl::SUPPLY1;
			break;
		case YYWOR:
			type = net_decl::WOR;
			break;
		case YYTRIOR:
			type = net_decl::TRIOR;
			break;
		default:
			type = net_decl::NONE;
			break;
		}
	ret = new net_decl(type,
				(net_decl::decl_assign_list *)idlst, (part_select *)r);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created net declaration.\n");
	return ret;
	}

p_decl_assign_list
p_create_decl_assign_list(p_decl_assign_list ral, p_decl_assign ra)
	{
	tfdecl_base::decl_assign_list *ret;
	if (ral != 0)
		ret = (tfdecl_base::decl_assign_list *) ral;
	else
		ret = new tfdecl_base::decl_assign_list;
	ret->push_back((tfdecl_base::decl_assign_type *) ra);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  added decl identifer (assignment) to list.\n");
	return ret;
	}

p_decl_assign
p_create_decl_assign(p_range_identifier id, p_expression exp)
	{
	tfdecl_base::decl_assign_type *ret;
	ret = new tfdecl_base::decl_assign_type;
	ret->_ident = (tfdecl_base::ident_type *) id;
	ret->_rval = (tfdecl_base::expr_type *) exp;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created decl identifier (assignment).\n");
	return ret;
	}

p_tfdecl_list
p_create_tfdecl_list(p_tfdecl_list tflst, p_tfdecl tf)
	{
	typedef tfdecl_base::list_type< basic_ptr<tfdecl_base> > tfdecl_list;
	tfdecl_list *ret;
	if (tflst != 0)
		ret = (tfdecl_list *)tflst;
	else
		ret = new tfdecl_list;
	if (tf != 0)
		{
		ret->push_back((tfdecl_base *)tf);
		DEBUG_STATE(DEBUG_PARSER);
		DEBUG_OUTPUT("PARSER:  added tf declaration to list.\n");
		}
	return ret;
	}

p_function_declaration
p_create_function_declaration
		(
		char *name,
		p_statement st,
		p_tfdecl_list tflist,
		p_part_select r
		)
	{
	function *ret;
	ret = new function(name, (stmt_base *) st, (function::decl_list *) tflist,
		(part_select *) r);
	ret->_lineno = cur_lineno;
	free(name);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created function definition.\n");
	return ret;
	}

p_task_declaration
p_create_task_declaration
		(
		char *name,
		p_statement st,
		p_tfdecl_list tflist)
	{
	task *ret;
	ret = new task(name, (stmt_base *) st, (task::decl_list *) tflist);
	ret->_lineno = cur_lineno;
	free(name);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created task definition.\n");
	return ret;
	}

p_initial_construct
p_create_initial_construct(p_statement st)
	{
	initial *ret;
	ret = new initial((stmt_base *)st);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created initial statement.\n");
	return ret;
	}

p_always_construct
p_create_always_construct(p_statement st)
	{
	always *ret;
	ret = new always((stmt_base *)st);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created always statement.\n");
	return ret;
	}

p_port_connection
p_create_port_connection(p_expression exp, const char *pn)
	{
	port_connection *ret;
	ret = new port_connection((expr_base *) exp, pn);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created port connection.\n");
	return ret;
	}

p_port_connection_list
p_create_port_connection_list(p_port_connection_list lst, p_port_connection pc)
	{
	typedef port_connection::list_type< basic_ptr<port_connection> >
		port_conn_list;
	port_conn_list *ret;
	if (lst != 0)
		ret = (port_conn_list *) lst;
	else
		ret = new port_conn_list;
	ret->push_back((port_connection *)pc);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created port connection list.\n");
	return ret;
	}

p_module_instance
p_create_module_instance(char *name, p_port_connection_list lst)
	{
	module_instance *ret;
	ret = new module_instance(name, (module_instance::port_conn_list *)lst);
	ret->_lineno = cur_lineno;
	free(name);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created module instance.\n");
	return ret;
	}

p_module_instance_list
p_create_module_instance_list(p_module_instance_list lst, p_module_instance mi)
	{
	typedef module_instance::list_type< basic_ptr<module_instance> >
		module_list;
	module_list *ret;
	if (lst != 0)
		ret = (module_list *) lst;
	else
		ret = new module_list;
	ret->push_back((module_instance *)mi);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created module instance list.\n");
	return ret;
	}

p_module_instantiation
p_create_module_instantiation(char *name, p_module_instance_list milst,
		p_expression_list elst)
	{
	module_instantiation *ret;
	ret = new module_instantiation(name,
		(module_instantiation::mod_inst_list *) milst,
		(module_instantiation::param_value_list *) elst);
	ret->_lineno = cur_lineno;
	free(name);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created module instantiation.\n");
	return ret;
	}

p_blocking_assignment_list
p_create_blocking_assignment_list
		(
		p_blocking_assignment_list lst,
		p_blocking_assignment as
		)
	{
	typedef assignment_stmt::list_type< basic_ptr<assignment_stmt> >
		assign_stmt_list;
	assign_stmt_list *ret;
	if (lst != 0)
		ret = (assign_stmt_list *) lst;
	else
		ret = new assign_stmt_list;
	ret->push_back((assignment_stmt *)as);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created assignment list.\n");
	return ret;
	}

p_continuous_assignment
p_create_continuous_assignment
		(
		p_blocking_assignment_list lst,
		p_delay_or_event_control d
		)
	{
	cont_assign *ret;
	ret = new cont_assign((cont_assign::assign_stmt_list *)lst, (dec_base *)d);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created continuous assignment.\n");
	return ret;
	}
