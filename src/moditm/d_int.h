// Verilog Behavioral Simulator
// Copyright (C) 1997,2001,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_int.h

#ifndef _D_INT_H
#define _D_INT_H

#include "moditm/tfbase.h"
#include "moditm/mibase.h"

class int_decl : public module_item_base, public tfdecl_base
	{
	friend struct decl_setup;
	friend struct decl_write;

public:
	int_decl(decl_assign_list *);
	virtual ~int_decl();

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);

private:
	decl_assign_list *_id_list;
	};

#endif // _D_INT_H
