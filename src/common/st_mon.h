// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_mon.h
//
// Function object to evaluate a symbol table node.  The only nodes
// that can be evaluated right now are net and function nodes.

#ifndef _ST_MON_H
#define _ST_MON_H

#include "common/ptr.h"
#include "common/st_node.h"
#include "stmt/stmtbase.h"

struct monitor_st_node : public st_node_monitor
	{
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;
	typedef counted_ptr<event_type> event_ptr;
	typedef st_node_base::list_type<event_ptr> monitor_list;

	monitor_st_node(event_ptr *);
	bool operator()(st_net *) const;
	bool operator()(st_function *) const;

	event_ptr *_event;
	};

#endif // _ST_MON_H

