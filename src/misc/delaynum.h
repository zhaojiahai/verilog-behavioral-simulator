// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// delaynum.h
//
// Delay by constant number.

#ifndef _DELAYNUM_H
#define _DELAYNUM_H

#include "common/bvector.h"
#include "common/event.h"
#include "stmt/stmtbase.h"
#include "misc/decbase.h"

class delay_num : public dec_base, public bit_vector
	{
	friend struct setup_dec;
	friend struct trigger_dec;
	friend struct write_dec;

public:
	typedef bit_vector num_type;
	typedef dec_base::ostream_type ostream_type;
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;

	delay_num(const num_type &);
	delay_num(const delay_num &);

	delay_num *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	int trigger(const trigger_type &);

private:
	// In case the delay amount is zero, we need to append to the
	// event queue rather than the time wheel.
	event_type *_event;
	};

#endif // _DELAYNUM_H
