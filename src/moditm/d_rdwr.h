// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_rdwr.h

#ifndef _D_RDWR_H
#define _D_RDWR_H

#include "common/ptr.h"
#include "misc/selbase.h"
#include "moditm/tfbase.h"

enum VBSOBJ_DECL_TYPE
	{
	VBSOBJ_DECL_LIST = 0x84000000,
	VBSOBJ_DECL_REG = 0x04000001,
	VBSOBJ_DECL_REG_WITH_SELECT = 0x04010001,
	VBSOBJ_DECL_REG_LIST = 0x84000001,
	VBSOBJ_DECL_WIRE = 0x04000002,
	VBSOBJ_DECL_WIRE_WITH_SELECT = 0x04010002,
	VBSOBJ_DECL_WIRE_LIST = 0x84000002,
	VBSOBJ_DECL_IN = 0x04000003,
	VBSOBJ_DECL_IN_WITH_SELECT = 0x04010003,
	VBSOBJ_DECL_IN_LIST = 0x84000003,
	VBSOBJ_DECL_OUT = 0x04000004,
	VBSOBJ_DECL_OUT_WITH_SELECT = 0x04010004,
	VBSOBJ_DECL_OUT_LIST = 0x84000004,
	VBSOBJ_DECL_INOUT = 0x04000005,
	VBSOBJ_DECL_INOUT_WITH_SELECT = 0x04010005,
	VBSOBJ_DECL_INOUT_LIST = 0x84000005,
	VBSOBJ_DECL_INT = 0x04000006,
	VBSOBJ_DECL_INT_WITH_SELECT = 0x04010006,
	VBSOBJ_DECL_INT_LIST = 0x84000006,
	VBSOBJ_DECL_PARAM = 0x04000007,
	VBSOBJ_DECL_PARAM_LIST = 0x84000007
	};

struct decl_read
	{
	typedef tfdecl_base::istream_type istream_type;
	typedef select_base select_type;

	decl_read(istream_type &i)
		: _in(i)
		{}

	tfdecl_base *operator()() const;

	istream_type &_in;
	};

struct decl_write
	{
	typedef tfdecl_base::ostream_type ostream_type;
	typedef tfdecl_base::ident_type ident_type;
	typedef basic_ptr<ident_type> ident_ptr;
	typedef tfdecl_base::list_type<ident_ptr> ident_list;

	decl_write(ostream_type &o)
		: _out(o)
		{}

	void operator()(const io_decl *) const;
	void operator()(const net_decl *) const;
	void operator()(const reg_decl *) const;
	void operator()(const int_decl *) const;
	void operator()(const param_decl *) const;

	ostream_type &_out;
	};

#endif // _D_RDWR_H
