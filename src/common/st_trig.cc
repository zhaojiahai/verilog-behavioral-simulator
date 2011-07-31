// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_trig.cc

#include "common/st_net.h"
#include "common/st_task.h"
#include "common/st_trig.h"
#include "stmt/taskenbl.h"
#include "stmt/strigger.h"

trigger_st_node::trigger_st_node(task_caller_type *t, stmt_type *p)
	{
	_caller = t;
	_parent = p;
	}

bool
trigger_st_node::operator()(st_task *p) const
	{
	const st_task::str_type &name(p->name());

	// Test whether we were delayed before.
	if (p->_delayed == true)
		{
		p->_delayed = false;
		goto continue_from_delay;
		}

	// Initialize local variables.
	if (p->_iovariable != 0)
		entry_iovars(*(p->_iovariable), *(_caller->_argument));

	// Trigger statement and pass in the caller.  The argument is
	// usually the parent statement, but the trigger function needs
	// data from the caller if the task is a system task.
	if (name[0] == '$')
		{
		// System tasks do not have delays in them.
		p->_stmt->trigger(trigger_stmt(_caller));
		}
	else
		{
		if (p->_stmt->trigger(trigger_stmt(_parent)) == false)
			{
			p->_delayed = true;
			return false;
			}
		}

	// Set output variables.
continue_from_delay:
	if (p->_iovariable != 0)
		exit_iovars(*(p->_iovariable), *(_caller->_argument));

	return true;
	}
