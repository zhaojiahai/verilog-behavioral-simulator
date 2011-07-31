// Verilog Behavioral Simulator
// Copyright (C) 1995 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// testdb.cc

#include <string>
#include "common/debug.h"

class Obj
	{
	string str;
public:
	Obj()
		: str("Hello world") {}
	ostream &display(ostream &s) const
		{ s << str; return s; }
	};

ostream &
operator<<(ostream &s, const Obj &o)
	{ return o.display(s); }

int
main(int argc, char *argv[])
	{
	Obj o;

	if (argc > 1)
		DEBUG_INITIALIZE(argv[1]);
	else
		DEBUG_INITIALIZE(0);
	DEBUG_ENABLE(DEBUG_USER1);

	DEBUG_STATE(DEBUG_USER1);
	DEBUG_OUTPUT(o);
	DEBUG_OUTPUT("\n");

	return 0;
	}
