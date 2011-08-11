// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// misetup.h

#ifndef _MISETUP_H
#define _MISETUP_H

#include "common/event.h"
#include "stmt/stmtbase.h"
#include "moditm/mitfbase.h"
#include "moditm/d_setup.h"

class st_module;
class st_function;
class st_task;

struct setup_module_item : public mitf_setup, public decl_setup
	{
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;
	typedef event_type::event_container event_cache_type;
	typedef stmt_type::list_type<hash_type> hash_list;
	typedef st_module module_type;
	typedef st_function func_type;
	typedef st_task task_type;

	setup_module_item(scopelist_type &s, hash_list *i = 0,
					  param_value_ptr *f = 0, const param_value_ptr *l = 0)
		: decl_setup(s, i, f, l)
		{}

	static void reset();

	void operator()(initial *) const;
	void operator()(always *) const;
	void operator()(cont_assign *) const;
	void operator()(function *) const;
	void operator()(task *) const;
	void operator()(module_instantiation *) const;
	// Front end for the declaration setup functions.
	void operator()(io_decl *) const;
	void operator()(net_decl *) const;
	void operator()(reg_decl *) const;
	void operator()(int_decl *) const;
	void operator()(time_decl *) const;
	void operator()(param_decl *) const;

	// Keep track of statements allocated for initial/always/continuous
	// statements with one statement.
	typedef stmt_type::list_type< basic_ptr<stmt_type> > stmt_list;
	static stmt_list *_stmtlist;
	};

#endif // _MISETUP_H
