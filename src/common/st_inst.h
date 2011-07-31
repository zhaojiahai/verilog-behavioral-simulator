// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_inst.h
//
// Symbol table object to support module instantiation.

#ifndef _ST_INST_H
#define _ST_INST_H

#include "common/st_node.h"

class st_instantiation : public st_node_base
	{
	friend struct setup_st_node;

public:
	st_instantiation(const str_type &);

	int level() const;
	void level(int);
	st_instantiation *get_instantiation();

	ostream_type &display(ostream_type &s) const;
	void setup(const setup_type &);

private:
	int _level;
	};

#endif // _ST_INST_H
