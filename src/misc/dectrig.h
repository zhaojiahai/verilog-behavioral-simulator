// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dectrig.h

#ifndef _DECTRIG_H
#define _DECTRIG_H

#include "common/event.h"
#include "misc/decbase.h"
#include "stmt/stmtbase.h"

struct trigger_dec : public dec_trigger
	{
	typedef stmt_base stmt_type;
	typedef stmt_type::expr_type expr_type;
	typedef expr_type::num_type num_type;

	trigger_dec(stmt_type *st)
		{ _stmt = st; }

	int operator()(delay_num *) const;
	int operator()(delay_id *) const;
	int operator()(ored_event_expr *) const;

	stmt_type *_stmt;
	};

#endif // _DECTRIG_H
