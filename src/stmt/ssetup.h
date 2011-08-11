// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// ssetup.h

#ifndef _SSETUP_H
#define _SSETUP_H

#include "common/dstack.h"
#include "common/event.h"
#include "stmt/stmtbase.h"
#include "stmt/case.h"

struct st_node_base;
class task_enable_stmt;
class st_function;
class st_task;
class st_net;

struct setup_stmt : public stmt_setup
	{
	typedef Stack<int> scopelist_type;
	typedef stmt_base stmt_type;
	typedef stmt_type::strstream_type strstream_type;
	typedef task_enable_stmt task_enable_type;
	typedef event_base<stmt_type> event_type;
	typedef counted_ptr<event_type> event_ptr;
	typedef event_type::event_container event_cache_type;
	typedef st_function func_type;
	typedef st_task task_type;
	typedef st_net net_type;

	setup_stmt(scopelist_type &s, stmt_type *st, bool ilp = false)
		: _inloop(ilp), _scope(s)
		{ _parent = st; }
	virtual ~setup_stmt()
		{}

	// Setup pass setup utilities.
	static void reset();
	static void second_pass();

	void handle_dec(stmt_type *) const;
	void operator()(null_stmt *) const;
	void operator()(seq_block_stmt *) const;
	void operator()(systask_finish *) const;
	void operator()(systask_display *) const;
	void operator()(systask_write *) const;
	void operator()(systask_monitor *) const;
	void operator()(systask_readmemb *) const;
	void operator()(systask_readmemh *) const;
	void operator()(systask_strobe *) const;
	void operator()(systask_dumpon *) const;
	void operator()(systask_dumpoff *) const;
	void operator()(systask_dumpall *) const;
	void operator()(systask_dumpvars *) const;
	void operator()(systask_dumpfile *) const;
	void operator()(sysfunc_time *) const;
	void operator()(sysfunc_stime *) const;
	void operator()(sysfunc_random *) const;
	void operator()(task_enable_stmt *) const;
	void operator()(assignment_stmt *) const;
	void operator()(if_else_stmt *) const;
	void operator()(case_stmt *) const;
	void operator()(loop_stmt *) const;

	bool _inloop;
	scopelist_type &_scope;
	stmt_type *_parent;

	// VCD setup must be done after other setup.  Keep a list
	// of $dumpvars tasks to process later.
	typedef stmt_base::list_type<stmt_type *> stmt_list;
	static stmt_list *_postponedsetup;
	};

#endif // _SSETUP_H
