// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_trig.h
//
// Function object to trigger symbol table nodes.  Only the task node
// needs triggering.

#ifndef _ST_TRIG_H
#define _ST_TRIG_H

#include "common/st_node.h"

class task_stmt;
class stmt_base;

struct trigger_st_node : public st_node_trigger
	{
	typedef stmt_base stmt_type;
	typedef task_enable_stmt task_caller_type;
	typedef st_task task_type;

	trigger_st_node(task_caller_type *, stmt_type *);
	bool operator()(st_task *) const;

	task_caller_type *_caller;
	stmt_type *_parent;
	};

#endif // _ST_TRIG_H
