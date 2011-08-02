// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_func.h
//
// Symbol table node to store a function definition.
//
// Note:  Function definitions are setup only if it is enabled in the
// module it was defined in. Thus, the tf declarations must be saved
// until setup is done.

#ifndef _ST_FUNC_H
#define _ST_FUNC_H

#include "common/ptr.h"
#include "common/event.h"
#include "stmt/stmtbase.h"
#include "moditm/tfbase.h"
#include "common/st_node.h"

class st_function : public st_node_base
	{
	friend struct monitor_st_node;
	friend struct evaluate_st_node;
	friend struct setup_st_node;

	friend struct setup_expr;
	friend struct setup_stmt;
	friend struct trigger_stmt;
	friend struct setup_module_item;

public:
	typedef num_type::position_type position_type;
	typedef num_type::size_type size_type;
	typedef stmt_base stmt_type;
	typedef tfdecl_base decl_type;
	typedef basic_ptr<decl_type> decl_ptr;
	typedef list_type<decl_ptr> decl_list;
	typedef list_type<hash_type> io_list;
	typedef event_base<stmt_type> event_type;
	typedef counted_ptr<event_type> event_ptr;
	typedef list_type<event_ptr> monitor_list;

	st_function(const str_type &, stmt_type *, decl_list * = 0);
	virtual ~st_function();

	st_function *get_function();

	const size_type size() const
		{ return _storage.size(); }

	ostream_type &display(ostream_type &) const;
	bool monitor(const monitor_type &);
	const num_type &evaluate(const evaluate_type &) const;
	void setup(const setup_type &);

	// Utility for function trigger.
	void assignment(const num_type &, position_type, position_type,
					position_type, position_type);

private:
	bool _done_setup;
	num_type _storage;
	stmt_type *_stmt;
	io_list *_iovariable;
	decl_list *_declaration;

	// List of monitor statements for functions.
	monitor_list *_monitor;
	};

#endif // _ST_FUNC_H
