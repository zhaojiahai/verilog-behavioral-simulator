// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// strigger.h

#ifndef _STRIGGER_H
#define _STRIGGER_H

#include "expr/exprbase.h"
#include "stmt/stmtbase.h"
#include "stmt/case.h"

struct st_node_base;
class st_function;
class st_task;
class st_net;

struct trigger_case_item : public case_item_trigger
	{
	typedef expr_base expr_type;
	typedef expr_type::num_type num_type;
	typedef stmt_base stmt_type;
	typedef case_item::case_type case_type;

	trigger_case_item(case_type t, const num_type &b, bool &s, stmt_type *p)
		: _type(t), _status(s), _value(b)
		{ _parent = p; }

	bool operator()(case_item *) const;

	case_type _type;
	bool &_status;
	const num_type &_value;
	stmt_type *_parent;
	};

struct trigger_stmt : public stmt_trigger
	{
	typedef stmt_base stmt_type;
	typedef stmt_type::str_type str_type;
	typedef stmt_type::expr_type expr_type;
	typedef stmt_type::strstream_type strstream_type;
	typedef expr_type::num_type num_type;
	typedef task_enable_stmt task_caller_type;
	typedef st_function func_type;
	typedef st_task task_type;
	typedef st_net net_type;

	trigger_stmt(stmt_type *s)
		{ _parent = s; }

	void sys_task_output(task_caller_type *) const;
	void sys_task_readmem(task_caller_type *, int) const;
	int handle_dec(stmt_type *) const;
	bool operator()(null_stmt *) const;
	bool operator()(seq_block_stmt *) const;
	bool operator()(systask_finish *) const;
	bool operator()(systask_display *) const;
	bool operator()(systask_write *) const;
	bool operator()(systask_monitor *) const;
	bool operator()(systask_readmemb *) const;
	bool operator()(systask_readmemh *) const;
	bool operator()(systask_strobe *) const;
	bool operator()(systask_dumpon *) const;
	bool operator()(systask_dumpoff *) const;
	bool operator()(systask_dumpall *) const;
	bool operator()(systask_dumpvars *) const;
	bool operator()(systask_dumpfile *) const;
	bool operator()(sysfunc_time *) const;
	bool operator()(sysfunc_stime *) const;
	bool operator()(sysfunc_random *) const;
	bool operator()(task_enable_stmt *) const;
	bool operator()(assignment_stmt *) const;
	bool operator()(if_else_stmt *) const;
	bool operator()(case_stmt *) const;
	bool operator()(loop_stmt *) const;

	stmt_type *_parent;
	};

#endif // _STRIGGER_H
