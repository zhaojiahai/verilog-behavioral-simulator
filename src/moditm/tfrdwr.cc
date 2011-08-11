// Verilog Behavioral Simulator
// Copyright (C) 1996-1997,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// tfrdwr.cc

#include "moditm/d_rdwr.h"
#include "moditm/tfrdwr.h"


void
write_tfdecl::operator()(const io_decl *p) const
	{ decl_write::operator()(p); }

void
write_tfdecl::operator()(const net_decl *p) const
	{ decl_write::operator()(p); }

void
write_tfdecl::operator()(const reg_decl *p) const
	{ decl_write::operator()(p); }

void
write_tfdecl::operator()(const int_decl *p) const
	{ decl_write::operator()(p); }

void
write_tfdecl::operator()(const time_decl *p) const
	{ decl_write::operator()(p); }
