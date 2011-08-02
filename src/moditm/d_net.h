// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_net.h

#ifndef _D_NET_H
#define _D_NET_H

#include "moditm/tfbase.h"
#include "moditm/mibase.h"

class net_decl : public module_item_base, public tfdecl_base
	{
	friend struct decl_setup;
	friend struct decl_write;

public:
	enum declaration_type
		{
		NONE,
		WIRE,
		TRI,
		TRI1,
		SUPPLY0,
		WAND,
		TRIAND,
		TRI0,
		SUPPLY1,
		WOR,
		TRIOR
		};

	net_decl(declaration_type, decl_assign_list *, range_type * = 0);
	virtual ~net_decl();

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);

private:
	declaration_type _type;
	range_type *_range;
	decl_assign_list *_id_list;
	};

#endif // _D_NET_H
