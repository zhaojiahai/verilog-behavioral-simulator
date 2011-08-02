// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_node.h
//
// STnode is a base class for all nodes in the symbol table.

#ifndef _ST_NODE_H
#define _ST_NODE_H

#include "common/bvector.h"
#include "common/hash.h"
#include "common/dstack.h"
#include "common/combase.h"

class st_function;
class st_instantiation;
class st_module;
class st_net;
class st_task;

struct st_node_monitor;
struct st_node_evaluate;
struct st_node_setup;
struct st_node_trigger;

struct st_node_base : public virtual common_base
	{
	typedef bit_vector num_type;
	typedef hash_value hash_type;
	typedef hash_type::scope_type scope_type;
	typedef Stack<int> scopelist_type;

	typedef st_node_monitor monitor_type;
	typedef st_node_evaluate evaluate_type;
	typedef st_node_setup setup_type;
	typedef st_node_trigger trigger_type;

	st_node_base(const str_type &n, const str_type &tn)
		: _scope(-1), _instance_scope(-1), _name(n), _type_name(tn)
		{}
	virtual ~st_node_base()
		{}

	// Common variables between all symbol table nodes.
	const str_type &type_name() const
		{ return _type_name; }
	const str_type &name() const
		{ return _name; }
	const scope_type scope() const
		{ return _scope; }
	void scope(scope_type sc)
		{ _scope = sc; }
	const scope_type instance_scope() const
		{ return _instance_scope; }
	void instance_scope(scope_type sc)
		{ _instance_scope = sc; }

	// Use these to determine the node type.
	// Should replace with dynamic_cast.
	virtual st_function *get_function()
		{ return 0; }
	virtual st_instantiation *get_instantiation()
		{ return 0; }
	virtual st_module *get_module()
		{ return 0; }
	virtual st_net *get_net()
		{ return 0; }
	virtual st_task *get_task()
		{ return 0; }

	virtual ostream_type &display(ostream_type &s) const
		{ return s; }
	virtual bool monitor(const monitor_type &) const
		{ return false; }
	virtual const num_type &evaluate(const evaluate_type &) const
		{ static num_type dummy(DC); return dummy; }
	virtual void setup(const setup_type &)
		{}
	virtual bool trigger(const trigger_type &)
		{ return true; }

protected:
	scope_type _scope;
	scope_type _instance_scope;
	str_type _name;
	str_type _type_name;
	};

inline st_node_base::ostream_type &
operator<<(st_node_base::ostream_type &s, const st_node_base &d)
	{ return d.display(s); }

struct st_node_monitor
	{
	virtual ~st_node_monitor()
		{}
	virtual bool operator()(st_function *) const
		{ return false; }
	virtual bool operator()(st_net *) const
		{ return false; }
	};

struct st_node_evaluate
	{
	typedef st_node_base::num_type num_type;

	virtual ~st_node_evaluate()
		{}
	virtual const num_type &operator()(const st_function *) const
		{ static num_type dummy(DC); return dummy; }
	virtual const num_type &operator()(const st_net *) const
		{ static num_type dummy(DC); return dummy; }
	};

struct st_node_setup
	{
	virtual ~st_node_setup() {}
	virtual void operator()(st_function *) const {}
	virtual void operator()(st_instantiation *) const {}
	virtual void operator()(st_module *) const {}
	virtual void operator()(st_net *) const {}
	virtual void operator()(st_task *) const {}
	};

struct st_node_trigger
	{
	virtual ~st_node_trigger() {}
	virtual bool operator()(st_task *) const { return true; }
	};

// Symbol name lookup.
hash_value st_node_find(const char *, Stack<int> &, int * = 0);

#endif // _ST_NODE_H
