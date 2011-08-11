// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// tfsetup.h

#ifndef _TFSETUP_H
#define _TFSETUP_H

#include "moditm/mitfbase.h"
#include "moditm/d_setup.h"

struct setup_tfdecl : public mitf_setup, public decl_setup
	{
	setup_tfdecl(scopelist_type &s, io_list *i = 0)
		: decl_setup(s, i)
		{}

	// Front end for the declaration setup functions.
	void operator()(io_decl *) const;
	void operator()(net_decl *) const;
	void operator()(reg_decl *) const;
	void operator()(int_decl *) const;
	void operator()(time_decl *) const;
	};

#endif // _TFSETUP_H
