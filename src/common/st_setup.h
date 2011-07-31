// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_setup.h
//
// Function object to setup the symbol table nodes.

#ifndef _ST_SETUP_H
#define _ST_SETUP_H

#include "common/dstack.h"
#include "common/st_node.h"
#include "expr/exprbase.h"
#include "stmt/stmtbase.h"
#include "misc/portconn.h"

struct setup_st_node : public st_node_setup
	{
	typedef st_node_base::scopelist_type scopelist_type;
	typedef stmt_base stmt_type;
	typedef expr_base expr_type;
	typedef basic_ptr<expr_type> expr_ptr;
	typedef st_node_base::list_type<expr_ptr> param_value_list;
	typedef param_value_list::iterator param_value_ptr;
	typedef st_node_base::list_type<expr_ptr> arg_list;
	typedef port_connection port_conn_type;
	typedef basic_ptr<port_conn_type> port_conn_ptr;
	typedef st_node_base::list_type<port_conn_ptr> port_conn_list;
	typedef st_module module_type;
	typedef st_net net_type;
	typedef st_function func_type;
	typedef st_task task_type;

	setup_st_node(scopelist_type &, arg_list *, stmt_type *);
	setup_st_node(scopelist_type &, port_conn_list * = 0,
		param_value_ptr * = 0, const param_value_ptr * = 0);

	void operator()(st_function *) const;
	void operator()(st_instantiation *) const;
	void operator()(st_module *) const;
	void operator()(st_net *) const;
	void operator()(st_task *) const;

	scopelist_type &_scope;
	arg_list *_caller_arg; // Setup function/task.
	stmt_type *_parent; // Parent statement for function/task.
	port_conn_list *_port_conn; // Setup instantiation.
	param_value_ptr *_first; // Setup instantiation.
	const param_value_ptr *_last; // Setup instantiation.
	};

#endif // _ST_SETUP_H
