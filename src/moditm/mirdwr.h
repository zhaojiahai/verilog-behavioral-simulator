// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mirdwr.h

#ifndef _MIRDWR_H
#define _MIRDWR_H

#include "moditm/mitfbase.h"
#include "moditm/d_rdwr.h"

enum VBSOBJ_MI_TYPE
	{
	VBSOBJ_MI_LIST = 0x85000000,
	VBSOBJ_MI_INITIAL = 0x05000001,
	VBSOBJ_MI_ALWAYS = 0x05000002,
	VBSOBJ_MI_CONT_ASSIGN = 0x05000003,
	VBSOBJ_MI_CONT_ASSIGN_WITH_DELAY = 0x05010003,
	VBSOBJ_MI_FUNCTION = 0x05000004,
	VBSOBJ_MI_FUNCTION_WITH_RETSIZE = 0x05010004,
	VBSOBJ_MI_TASK = 0x05000004,
	VBSOBJ_MI_MODULE_INSTAN = 0x05000006
	};

struct read_module_item : public module_item_read, decl_read
	{
	typedef module_item_base::istream_type istream_type;

	read_module_item(istream_type &i)
		: decl_read(i)
		{}

	module_item_base *operator()() const;
	};

struct write_module_item : public mitf_write, public decl_write
	{
	typedef module_item_base::ostream_type ostream_type;

	write_module_item(ostream_type &o)
		: decl_write(o)
		{}

	void operator()(const initial *) const;
	void operator()(const always *) const;
	void operator()(const cont_assign *) const;
	void operator()(const function *) const;
	void operator()(const task *) const;
	void operator()(const module_instantiation *) const;
	// Front end for the declaration write functions.
	void operator()(const io_decl *) const;
	void operator()(const net_decl *) const;
	void operator()(const reg_decl *) const;
	void operator()(const int_decl *) const;
	void operator()(const param_decl *) const;
	};

#endif // _MIRDWR_H
