// Verilog Behavioral Simulator
// Copyright (C) 2001 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// display.h

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "stmt/stmtbase.h"

class systask_display : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;

public:
	systask_display(); // No arguments, name is fixed!

	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	str_type _name;
	};

#endif // _DISPLAY_H
