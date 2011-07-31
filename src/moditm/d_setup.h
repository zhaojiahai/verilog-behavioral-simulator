// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_setup.h

#ifndef _D_SETUP_H
#define _D_SETUP_H

#include "common/dstack.h"
#include "common/st_net.h"
#include "moditm/tfbase.h"

struct decl_setup
	{
	typedef Stack<int> scopelist_type;
	typedef tfdecl_base::range_type range_type;
	typedef tfdecl_base::ident_type ident_type;
	typedef basic_ptr<ident_type> ident_ptr;
	typedef tfdecl_base::list_type<ident_ptr> ident_list;
	typedef st_net net_type;
	typedef net_type::num_type num_type;
	typedef net_type::hash_type hash_type;
	typedef net_type::io_list io_list;
	typedef net_type::iodirection_type iodirection_type;
	typedef net_type::net_type type;

	// For wire declaration.
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;
	typedef event_type::event_container event_cache_type;

	// For parameter declaration.
	typedef net_type::expr_type expr_type;
	typedef basic_ptr<expr_type> expr_ptr;
	typedef net_type::list_type<expr_ptr> param_list;
	typedef param_list::iterator param_value_ptr;

	decl_setup(scopelist_type &s, io_list *i = 0,
			   param_value_ptr *f = 0, const param_value_ptr *l = 0)
		: _scope(s)
		{
		_iovariable = i;
		_first = f;
		_last = l;
		}

	void net_setup(ident_ptr &, range_type *, type) const;
	void io_setup(ident_list *, range_type *, iodirection_type, bool) const;
	// Boolean to indicate whether the net should exist.
	// Default is yes.  A task or function definition would use false.
	void operator()(io_decl *, bool = true) const;
	void operator()(net_decl *) const;
	void operator()(reg_decl *) const;
	void operator()(int_decl *) const;
	void operator()(param_decl *) const;

	scopelist_type &_scope;
	io_list *_iovariable;
	param_value_ptr *_first;
	const param_value_ptr *_last;
	};

#endif // _D_SETUP_H
