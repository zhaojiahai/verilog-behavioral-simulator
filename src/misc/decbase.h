// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// decbase.h
//
// Base class for all delay event controls.

#ifndef _DECBASE_H
#define _DECBASE_H

#include "common/combase.h"

class delay_num;
class delay_id;
class ored_event_expr;

struct dec_setup;
struct dec_trigger;
struct dec_write;

struct dec_base : public virtual common_base
	{
	typedef dec_setup setup_type;
	typedef dec_trigger trigger_type;
	typedef dec_write write_type;

	dec_base()
		: _delayed(false)
		{}
	dec_base(const dec_base &p)
		: common_base(p), _delayed(p._delayed)
		{}
	virtual ~dec_base()
		{}

	virtual dec_base *copy_constructor() const
		{ return new dec_base(*this); }
	virtual bool is_event_control(void) const
		{ return false; }
	virtual ostream_type &display(ostream_type &s) const
		{ return s; }
	virtual void write(const write_type &) const
		{}
	virtual void setup(const setup_type &)
		{}
	virtual void trigger(const trigger_type &)
		{}

	bool _delayed; // Delayed already, trigger statement immediately.
	};

inline dec_base::ostream_type &
operator<<(dec_base::ostream_type &s, const dec_base &d)
	{ return d.display(s); }

struct dec_setup
	{
	virtual ~dec_setup() {}
	virtual void operator()(delay_num *) const {}
	virtual void operator()(delay_id *) const {}
	virtual void operator()(ored_event_expr *) const {}
	};

struct dec_trigger
	{
	virtual ~dec_trigger() {}
	virtual void operator()(delay_num *) const {}
	virtual void operator()(delay_id *) const {}
	virtual void operator()(ored_event_expr *) const {}
	};

struct dec_write
	{
	virtual ~dec_write() {}
	virtual void operator()(const delay_num *) const {}
	virtual void operator()(const delay_id *) const {}
	virtual void operator()(const ored_event_expr *) const {}
	};

#endif // _DECBASE_H
