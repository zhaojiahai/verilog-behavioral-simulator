// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_io.h

#ifndef _D_IO_H
#define _D_IO_H

#include "moditm/tfbase.h"
#include "moditm/mibase.h"

class io_decl : public module_item_base, public tfdecl_base
	{
	friend struct decl_setup;
	friend struct decl_write;

public:
	enum io_decl_type
		{
		INPUT,
		OUTPUT,
		INOUT
		};

	io_decl(io_decl_type, ident_list *, range_type * = 0);
	virtual ~io_decl();

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);

private:
	io_decl_type _type;
	range_type *_range;
	ident_list *_id_list;
	};

#endif // _D_IO_H
