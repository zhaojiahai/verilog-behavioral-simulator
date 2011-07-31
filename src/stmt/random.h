// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// random.h
//
// The random system function is deriving from a statement class because
// the object is only going to be placed in a symbol table.  Thus, when
// $random is enabled, the symbol table node is triggered, and the trigger
// function is called.  Thus, it is not necessary to derive from Expression.

#ifndef _RANDOM_H
#define _RANDOM_H

#include "common/hash.h"
#include "stmt/stmtbase.h"

class sysfunc_random : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;

public:
	typedef hash_value hash_type;

	sysfunc_random(); // No arguments, name is fixed!

	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	hash_type _index; // Storage for return value.
	str_type _name;
	};

#endif // _RANDOM_H
