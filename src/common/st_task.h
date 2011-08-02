// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_task.h
//
// Symbol table node to store a task definition.
//
// Note:  Task definitions are setup only if it is enabled in the
// module it was defined in. Thus, the tf declarations must be saved
// until setup is done.

#ifndef _ST_TASK_H
#define _ST_TASK_H

#include "common/ptr.h"
#include "common/st_node.h"
#include "stmt/stmtbase.h"
#include "moditm/tfbase.h"

class st_task : public st_node_base
	{
	friend struct setup_st_node;
	friend struct trigger_st_node;

	friend struct setup_stmt;
	friend struct setup_module_item;

public:
	typedef stmt_base stmt_type;
	typedef list_type<hash_type> io_list;
	typedef tfdecl_base decl_type;
	typedef basic_ptr<decl_type> decl_ptr;
	typedef list_type<decl_ptr> decl_list;

	st_task(const str_type &, stmt_type *, decl_list * = 0);
	virtual ~st_task();

	st_task *get_task();

	ostream_type &display(ostream_type &s) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	bool _delayed; // For delays in task enable.
	bool _done_setup;
	stmt_type *_stmt;
	io_list *_iovariable;
	decl_list *_declaration;
	};

#endif // _ST_TASK_H
