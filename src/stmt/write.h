// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// write.h

#ifndef _WRITE_H
#define _WRITE_H

#include "stmt/stmtbase.h"

class systask_write : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;

public:
	systask_write(); // No arguments, name is fixed!

	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	str_type _name;
	};

#endif // _WRITE_H
