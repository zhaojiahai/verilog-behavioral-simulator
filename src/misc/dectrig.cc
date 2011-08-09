// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002,2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dectrig.cc

#include "common/error.h"
#include "common/time_whl.h"
#include "common/sym_tab.h"
#include "expr/eeval.h"
#include "misc/delaynum.h"
#include "misc/delayid.h"
#include "misc/evntexpr.h"
#include "misc/dectrig.h"
#include "vbs.h"


// Event expression trigger object.

struct trigger_event_expr : public event_expr_trigger
	{
	void operator()(event_expr *) const;
	};

void
trigger_event_expr::operator()(event_expr *p) const
	{
	p->_event->activate();
	}


// Delay or event control trigger object.

int
trigger_dec::operator()(delay_num *p) const
	{
	int retval;
	unsigned long next_time = *p;
	if (next_time == 0)
		{
		// Delay is zero, append to eventqueue to be executed
		// as a non-blocking event.
		if (!p->_event->is_queued())
			{
			event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
			eventqueue.add_event(p->_event);
			}
		retval = 0;
		}
	else
		{
		if (next_time == (unsigned long)-1)
			next_time = 0;

		// Find the time unit to append to and do it.
		time_wheel<stmt_base> &timewheel = vbs_engine::timewheel();
		time_wheel<stmt_base>::time_type t = timewheel.current_time();
		t += next_time;
		timewheel.add_event(t, _stmt);

		retval = 1;
		}
	return retval;
	}

int
trigger_dec::operator()(delay_id *p) const
	{
	// The delay is specified by the identifier.
	int retval;
	unsigned long next_time = p->_value;
	if (next_time == 0)
		{
		// Delay is zero, append to eventqueue to be executed
		// as a non-blocking event.
		if (!p->_event->is_queued())
			{
			event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
			eventqueue.add_event(p->_event);
			}
		retval = 0;
		}
	else
		{
		if (next_time == (unsigned long)-1)
			next_time = 0;

		// Find the time unit to append to and do it.
		time_wheel<stmt_base> &timewheel = vbs_engine::timewheel();
		time_wheel<stmt_base>::time_type t = timewheel.current_time();
		t += next_time;
		timewheel.add_event(t, _stmt);

		retval = 1;
		}
	return retval;
	}

int
trigger_dec::operator()(ored_event_expr *p) const
	{
	// Only one Event object can exist for the entire ored event list.
	// Otherwise, we will trigger two events when we should only
	// trigger one.  Add this event to the list for all events in
	// this expression.
	ored_event_expr::event_expr_list::iterator itp(p->_ored_list->begin());
	ored_event_expr::event_expr_list::iterator stop(p->_ored_list->end());
	for (; itp != stop; ++itp)
		(*itp).get()->trigger(trigger_event_expr());

	return -1;
	}
