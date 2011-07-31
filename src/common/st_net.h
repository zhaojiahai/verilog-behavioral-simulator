// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_net.h
//
// Symbol table node to store net information.

#ifndef _ST_NET_H
#define _ST_NET_H

#include "common/ptr.h"
#include "common/event.h"
#include "common/st_node.h"
#include "stmt/stmtbase.h"

class st_net : public st_node_base
	{
	friend struct monitor_st_node;
	friend struct evaluate_st_node;
	friend struct setup_st_node;
	friend struct trigger_st_node;

	friend struct st_net_assign;
	friend struct st_net_evaluate;
	friend struct setup_expr;
	friend struct trigger_expr;
	friend struct setup_port;
	friend struct decl_setup;

public:
	typedef num_type::position_type position_type;
	typedef num_type::size_type size_type;
	typedef list_type<hash_type> hash_list;
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;
	typedef event_type::event_container cache_type;
	// Multiple expressions could be pointing to the same event.
	// Only one event is passed to the monitor function object.
	typedef counted_ptr<event_type> event_ptr;
	typedef list_type<event_ptr> monitor_list;

	typedef list_type<hash_type> io_list;
	typedef stmt_base::expr_type expr_type;
	typedef basic_ptr<expr_type> arg_type;
	typedef list_type<arg_type> arg_list;
	friend void entry_iovars(io_list &, arg_list &);
	friend void exit_iovars(io_list &, arg_list &);

	enum net_type { PORT, WIRE, REGISTER, INTEGER, MEMORY, PARAMETER };
	enum iodirection_type { UNDEF, IN, OUT, INOUT };

	st_net(const str_type &, net_type = PORT, iodirection_type = UNDEF);
	~st_net();

	st_net *get_net();

	// For $dump API.
	const position_type port_msb() const
		{ return _storage->stop_pos(); }
	const position_type port_lsb() const
		{ return _storage->start_pos(); }
	const size_type size() const
		{ return _storage->size(); }
	const size_type depth() const
		{ return _upper_mem_idx - _lower_mem_idx; }
	bool changed() const
		{ return _changed; }
	void changed(bool b)
		{ _changed = b; }

	ostream_type &display(ostream_type &) const;
	bool monitor(const monitor_type &);
	const num_type &evaluate(const evaluate_type &) const;
	void setup(const setup_type &);

	// Utility for trigger.
	void assignment(const char *, int, position_type, position_type);
	void assignment(const num_type &, position_type, position_type,
					position_type, position_type);

private:
	net_type _type;
	iodirection_type _iodirection;
	bool _changed;

	// Net types need temporary storage.  This is also used as a
	// check to make sure the net type was declared correctly.
	num_type *_storage;
	num_type *_result;
	position_type _upper_mem_idx, _lower_mem_idx;

	// Ports are nets too, and need special information.
	hash_type *_port_index;
	position_type _port_ms, _port_ls;

	// List of monitor statements for reg and wires.
	monitor_list *_monitor;
	};

// Function object to help with assignments.  Code in st_util.cc.
// Put declaration here so we don't need to include st_net.h in st_util.h.
struct st_net_assign
	{
	typedef st_net::num_type num_type;
	typedef st_net::position_type position_type;

	st_net_assign(st_net &n)
		: _net(n)
		{}
	void operator()(const char *, int, position_type, position_type);
	void operator()(const num_type &, position_type, position_type,
					position_type, position_type);
	st_net &_net;
	};

// Function object to help in simulation.
struct st_net_evaluate
	{
	typedef st_net net_type;
	typedef net_type::num_type num_type;
	typedef net_type::position_type position_type;

	st_net_evaluate(const st_net &n)
		: _net(n)
		{}
	const num_type &operator()(position_type, position_type,
		position_type, position_type, bool) const;
	const st_net &_net;
	};

#endif // _ST_NET_H
