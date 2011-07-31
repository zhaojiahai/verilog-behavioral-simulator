// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mibase.h

#ifndef _MIBASE_H
#define _MIBASE_H

#include "moditm/mitfbase.h"

struct module_item_base;
struct module_item_read
	{
	virtual module_item_base *operator()() const { return 0; }
	};

struct module_item_base : public virtual mitf_base
	{
	typedef module_item_read read_type;

	virtual ~module_item_base()
		{}

	// Must redefine read function.
	static module_item_base *read(const read_type &rd)
		{ return rd(); }
	virtual function *get_function()
		{ return 0; }
	virtual task *get_task()
		{ return 0; }
	virtual module_instantiation *get_module_instantiation()
		{ return 0; }
	};

inline module_item_base::ostream_type &
operator<<(module_item_base::ostream_type &s, const module_item_base &d)
	{ return d.display(s); }

#endif // _MIBASE_H
