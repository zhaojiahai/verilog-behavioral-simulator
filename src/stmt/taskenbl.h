// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// taskenbl.h

#ifndef _TASKENBL_H
#define _TASKENBL_H

#include "common/ptr.h"
#include "common/hash.h"
#include "expr/exprbase.h"
#include "stmt/stmtbase.h"

class task_enable_stmt : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;
	friend struct write_stmt;

	friend struct trigger_st_node;

public:
	typedef hash_value hash_type;
	typedef expr_base expr_type;
	typedef basic_ptr<expr_type> arg_type;
	typedef list_type<arg_type> arg_list;

	task_enable_stmt(const char *, arg_list * = 0);
	task_enable_stmt(const task_enable_stmt &);
	virtual ~task_enable_stmt();

	bool delayed() const;
	task_enable_stmt *get_task_enable();
	task_enable_stmt *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	str_type _name; // Name of system task w/ '$'
	int _scope; // Scope of the block that contains this statement.
	bool _delayed; // For delays in task enable.
	bool _change_enabled; // Task is enabled via change event.
	hash_type _index; // Index of task in symbol table.
	arg_list *_argument; // String constant for format
                          // and list of arguments
	};

#endif // _TASKENBL_H
