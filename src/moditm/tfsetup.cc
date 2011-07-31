// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// tfsetup.cc

#include "moditm/d_setup.h"
#include "moditm/tfsetup.h"

void
setup_tfdecl::operator()(io_decl *p) const
	{ decl_setup::operator()(p, false); }

void
setup_tfdecl::operator()(net_decl *p) const
	{ decl_setup::operator()(p); }

void
setup_tfdecl::operator()(reg_decl *p) const
	{ decl_setup::operator()(p); }

void
setup_tfdecl::operator()(int_decl *p) const
	{ decl_setup::operator()(p); }
