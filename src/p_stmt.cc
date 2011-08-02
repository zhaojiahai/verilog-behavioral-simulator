// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// p_stmt.cc

#include "common/error.h"
#include "expr/exprbase.h"
#include "stmt/seqblk.h"
#include "stmt/taskenbl.h"
#include "stmt/assign.h"
#include "stmt/ifelse.h"
#include "stmt/case.h"
#include "stmt/loopstmt.h"
#include "misc/delaynum.h"
#include "parser.h"
#include "p_expr.h"
#include "p_stmt.h"
#include "p_misc.h"
#include "vbs_yacc.h"

#include "common/debug.h"

p_case_item_list
p_create_case_item_list(p_case_item_list cilst, p_case_item ci)
	{
	typedef case_item::list_type< basic_ptr<case_item> > case_item_list;
	case_item_list *ret;
	if (cilst != 0)
		ret = (case_item_list *)cilst;
	else
		ret = new case_item_list;
	ret->push_back((case_item *)ci);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  added case item to list.\n");
	return ret;
	}

p_case_item
p_create_case_item(p_expression_list elst, p_statement st)
	{
	case_item *ret;
	ret = new case_item((stmt_base *)st, (case_item::expr_list *)elst);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created case item.\n");
	return ret;
	}

p_statement_list
p_create_statement_list(p_statement_list slst, p_statement st)
	{
	typedef stmt_base::list_type< basic_ptr<stmt_base> > stmt_list;
	stmt_list *ret;
	if (slst != 0)
		ret = (stmt_list *)slst;
	else
		ret = new stmt_list;
	if (st != 0)
		{
		ret->push_back((stmt_base *) st);
		DEBUG_STATE(DEBUG_PARSER);
		DEBUG_OUTPUT("PARSER:  added statement to list.\n");
		}
	return ret;
	}

p_statement
p_create_null_statement(void)
	{
	stmt_base *ret;
	ret = new null_stmt;
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created null statement.\n");
	return ret;
	}

p_seq_block
p_create_seq_block(p_statement_list slst)
	{
	seq_block_stmt *ret;
	ret = new seq_block_stmt((seq_block_stmt::stmt_list *)slst);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created sequential block.\n");
	return ret;
	}

p_task_enable
p_create_task_enable(char *name, p_expression_list explst)
	{
	task_enable_stmt *ret;
	ret = new task_enable_stmt(name, (task_enable_stmt::arg_list *) explst);
	ret->_lineno = cur_lineno;
	free(name);
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created task enable statement.\n");
	return ret;
	}

p_blocking_assignment
p_create_blocking_assignment
		(
		p_lvalue lv,
		p_expression exp,
		p_delay_or_event_control dec
		)
	{
	assignment_stmt *ret;
	ret = new assignment_stmt((lvalue *)lv, (expr_base *)exp, false,
							  (dec_base *)dec);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created blocking assignment statement.\n");
	return ret;
	}

p_non_blocking_assignment
p_create_non_blocking_assignment
		(
		p_lvalue lv,
		p_expression exp,
		p_delay_or_event_control dec
		)
	{
	assignment_stmt *ret;
	if (dec == 0)
		dec = new delay_num(0UL); /* Non-blocking assignment. */
	ret = new assignment_stmt((lvalue *)lv, (expr_base *)exp, true,
							  (dec_base *)dec);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created non-blocking assignment statement.\n");
	return ret;
	}

p_if_else_statement
p_create_if_else_statement(p_expression exp, p_statement t, p_statement f)
	{
	if_else_stmt *ret;
	ret = new if_else_stmt((expr_base *)exp, (stmt_base *)t, (stmt_base *)f);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created if/else statement.\n");
	return ret;
	}

p_case_statement
p_create_case_statement(int t, p_expression exp, p_case_item_list cilst)
	{
	case_stmt *ret;
	switch (t)
		{
		case YYCASEX:
			ret = new case_stmt(case_item::CASE_X, (expr_base *)exp,
				(case_stmt::citem_list *)cilst);
			break;
		case YYCASEZ:
			ret = new case_stmt(case_item::CASE_Z, (expr_base *)exp,
				(case_stmt::citem_list *)cilst);
			break;
		default:
			ret = new case_stmt(case_item::CASE_C, (expr_base *)exp,
				(case_stmt::citem_list *)cilst);
			break;
		}
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created case statement.\n");
	return ret;
	}

p_loop_statement
p_create_loop_statement
		(
		int t,
		p_statement st,
		p_expression exp,
		p_blocking_assignment pre,
		p_blocking_assignment post
		)
	{
	loop_stmt *ret;
	loop_stmt::loop_type type;
	switch (t)
		{
		case YYFOREVER: type = loop_stmt::FOREVER; break;
		case YYREPEAT: type = loop_stmt::REPEAT; break;
		case YYWHILE: type = loop_stmt::WHILE; break;
		case YYFOR: type = loop_stmt::FOR; break;
		default: return 0;
		}

	ret = new loop_stmt(type, (stmt_base *)st,
		(expr_base *)exp, (assignment_stmt *)pre, (assignment_stmt *)post);
	ret->_lineno = cur_lineno;
	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  created loop statement.\n");
	return ret;
	}

p_statement
p_create_include_delay_or_event_control
		(
		p_statement st,
		p_delay_or_event_control dec
		)
	{
	stmt_base *ret;
	dec_base *ec;
	bool ec_is_event, err = false;

	ret = (stmt_base *)st;
	ec = (dec_base *)dec;
	ec_is_event = ec->is_event_control();
	if (ret->_dec == 0)
		{
		if (ret->_ec == 0)
			{
			if (ec_is_event == true)
				ret->_ec = ec;
			else
				ret->_dec = ec;
			}
		else
			err = true;
		}
	else
		{
		bool dec_is_event = ret->_dec->is_event_control();

		if (ret->_ec == 0)
			{
			if (ec_is_event == true && dec_is_event == false)
				ret->_ec = ec;
			else
				err = true;
			}
		else
			err = true;
		}

	if (err != false)
		{
		static char cs[] = {'d','e','l','a','y',' ',
							'o','r',' ',
							'e','v','e','n','t',' ',
							'c','o','n','t','r','o','l'};
		yyerror(cs);
		}

	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("PARSER:  added delay or event control to statement.\n");
	return ret;
	}
