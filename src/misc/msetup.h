// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// msetup.h

#ifndef _MSETUP_H
#define _MSETUP_H

#include "common/dstack.h"
#include "misc/module.h"
#include "misc/port.h"
#include "misc/portconn.h"
#include "misc/lvalue.h"
#include "misc/selbase.h"

class number;
class range_id;
class st_net;

// sim_parsed_module

struct setup_module : public module_setup
	{
	typedef st_module module_type;

	setup_module(module_type *p)
		{ _stmod = p; }

	void operator()(module *) const;

	module_type *_stmod;
	};

struct setup_port : public port_setup
	{
	typedef Stack<int> scopelist_type;
	typedef range_id ident_type;
	typedef number num_type;
	typedef port_connection port_conn_type;
	typedef st_net net_type;

	setup_port(scopelist_type &s, port_conn_type *p)
		: _scope(s)
		{ _port_conn = p; }

	static void reset();

	void operator()(port *) const;

	scopelist_type &_scope;
	port_conn_type *_port_conn;

	// Save allocated assignment statements for IN/INOUT
	// connections.  Must be kept to allow event processing
	// and prevent memory leak.
	static std::list< basic_ptr<stmt_base> > *_stmtlist;
	};

struct setup_port_conn : public port_conn_setup
	{
	typedef Stack<int> scopelist_type;
	typedef range_id ident_type;

	setup_port_conn(scopelist_type &s)
		: _scope(s)
		{}

	bool operator()(port_connection *) const;

	scopelist_type &_scope;
	};

struct setup_select : public select_setup
	{
	typedef Stack<int> scopelist_type;

	setup_select(scopelist_type &s)
		: _scope(s)
		{}

	void operator()(bit_select *) const;
	void operator()(part_select *) const;

	scopelist_type &_scope;
	};

struct setup_lvalue : public lvalue_setup
	{
	typedef Stack<int> scopelist_type;
	typedef st_node_base node_type;
	typedef node_type::strstream_type strstream_type;

	setup_lvalue(scopelist_type &s)
		: _scope(s)
		{}

	size_type operator()(lvalue *) const;

	scopelist_type &_scope;
	};

#endif // _MSETUP_H
