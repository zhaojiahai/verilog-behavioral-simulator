// Verilog Behavioral Simulator
// Copyright (C) 1996-1997,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// tfrdwr.h

#ifndef _TFRDWR_H
#define _TFRDWR_H

#include "moditm/mitfbase.h"
#include "moditm/d_rdwr.h"


struct read_tfdecl : /*public mitf_read,*/ public decl_read
	{
	typedef mitf_base::istream_type istream_type;

	read_tfdecl(istream_type &i)
		: decl_read(i)
		{}
	};

struct write_tfdecl : public mitf_write, public decl_write
	{
	typedef mitf_base::ostream_type ostream_type;

	write_tfdecl(ostream_type &o)
		: decl_write(o)
		{}

	// Front end for the declaration write functions.
	void operator()(const io_decl *) const;
	void operator()(const net_decl *) const;
	void operator()(const reg_decl *) const;
	void operator()(const int_decl *) const;
	};

#endif // _TFRDWR_H
