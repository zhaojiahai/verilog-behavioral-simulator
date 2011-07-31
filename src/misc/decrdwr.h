// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// decrdwr.h

#ifndef _DECRDWR_H
#define _DECRDWR_H

#include "misc/decbase.h"
#include "misc/evntexpr.h"

enum VBSOBJ_DEC_TYPE
	{
	VBSOBJ_DEC_NUMBER = 0x02000001,
	VBSOBJ_DEC_RANGEID = 0x02000002,
	VBSOBJ_DEC_EVENT_EXPR = 0x02000003,
	// Thrid byte is number of event expressions in the list.
	VBSOBJ_DEC_EVENT_EXPR_LIST = 0x82000003,
	VBSOBJ_DEC_ORED_EVENT_EXPR = 0x02000004
	};

struct write_event_expr : public event_expr_write
	{
	typedef event_expr::ostream_type ostream_type;

	write_event_expr(ostream_type &o)
		: _out(o)
		{}

	void operator()(const event_expr *) const;

	ostream_type &_out;
	};


struct write_dec : public dec_write
	{
	typedef dec_base::ostream_type ostream_type;

	write_dec(ostream_type &o)
		: _out(o)
		{}

	void operator()(const delay_num *) const;
	void operator()(const delay_id *) const;
	void operator()(const ored_event_expr *) const;

	ostream_type &_out;
	};

#endif // _DECRDWR_H
