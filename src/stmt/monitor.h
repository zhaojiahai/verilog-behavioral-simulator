// Verilog Behavioral Simulator
// Copyright (C) 1996-1997 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// monitor.h

#ifndef _MONITOR_H
#define _MONITOR_H

#include "stmt/stmtbase.h"

class systask_monitor : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;

public:
	systask_monitor(); // No arguments, name is fixed!

	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	str_type _name;
	};

#endif // _MONITOR_H
