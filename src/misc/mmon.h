// Verilog Behavioral Simulator
// Copyright (C) 2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mmon.h

#ifndef _MMON_H
#define _MMON_H

#include "common/ptr.h"
#include "common/event.h"
#include "misc/selbase.h"

struct stmt_base;

struct monitor_select : public select_monitor
	{
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;
	typedef counted_ptr<event_type> event_ptr;

	monitor_select(event_ptr *e)
		{ _event = e; }

	bool operator()(const bit_select *) const;
	bool operator()(const part_select *) const;

	event_ptr *_event;
	};

#endif // _MMON_H
