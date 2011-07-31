// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_mon.cc

#include "common/st_func.h"
#include "common/st_net.h"
#include "common/st_mon.h"

monitor_st_node::monitor_st_node(event_ptr *e)
	{ _event = e; }

bool
monitor_st_node::operator()(st_function *p) const
	{
	if (_event != 0
	 && p->_name != "$time" // IEEE Std 1364-1995 says these system
	 && p->_name != "$stime" // functions can not be monitored.
	 && p->_name != "$realtime")
		{
		if (p->_monitor == 0)
			p->_monitor = new monitor_list;
		p->_monitor->push_back(*_event);
		}
	return true;
	}

bool
monitor_st_node::operator()(st_net *p) const
	{
	if (_event != 0)
		{
		if (p->_monitor == 0)
			p->_monitor = new monitor_list;
		p->_monitor->push_back(*_event);
		}
	return true;
	}
