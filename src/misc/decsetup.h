// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// decsetup.h

#ifndef _DECSETUP_H
#define _DECSETUP_H

#include "common/event.h"
#include "common/dstack.h"
#include "misc/decbase.h"
#include "stmt/stmtbase.h"

struct setup_dec : public dec_setup
	{
	typedef Stack<int> scopelist_type;
	typedef stmt_base stmt_type;
	typedef stmt_type::expr_type expr_type;
	typedef expr_type::num_type num_type;
	typedef event_base<stmt_type> event_type;
	typedef event_type::event_container event_cache_type;

	setup_dec(scopelist_type &s, stmt_type *st, stmt_type *p_st)
		: _scope(s)
		{ _stmt = st; _parent_stmt = p_st; }

	void operator()(delay_num *) const;
	void operator()(delay_id *) const;
	void operator()(ored_event_expr *) const;

	scopelist_type &_scope;
	stmt_type *_stmt;
	stmt_type *_parent_stmt;
	};

#endif // _DECSETUP_H
