// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// delayid.h

#ifndef _DELAYID_H
#define _DELAYID_H

#include "common/hash.h"
#include "common/event.h"
#include "expr/exprbase.h"
#include "stmt/stmtbase.h"
#include "misc/decbase.h"

class delay_id : public dec_base
	{
	friend struct setup_dec;
	friend struct trigger_dec;
	friend struct write_dec;

public:
	typedef hash_value hash_type;
	typedef expr_base expr_type;
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;

	delay_id(expr_type *);
	delay_id(const delay_id &);
	virtual ~delay_id();

	delay_id *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	int trigger(const trigger_type &);

private:
	expr_type *_expr;

	// In case the delay amount is zero, we need to append to the
	// event queue rather than the time wheel.
	event_type *_event;
	};

#endif // _DELAYID_H
