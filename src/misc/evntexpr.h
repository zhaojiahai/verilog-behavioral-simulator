// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// evntexpr.h
//
// Event expression:  this class and OredEvntExpression must be defined
// because Sim_setup of EvntExpression requires instantiating Event<Stmts>.
// Thus, Stmts must be defined before this class.

#ifndef _EVNTEXPR_H
#define _EVNTEXPR_H

#include <list>
#include "common/ptr.h"
#include "common/combase.h"
#include "common/event.h" // TRIG_NONE.
#include "expr/exprbase.h"
#include "misc/decbase.h"
#include "stmt/stmtbase.h"

struct event_expr_setup;
struct event_expr_trigger;
struct event_expr_write;

class event_expr : public virtual common_base
	{
	friend struct trigger_event_expr;
	friend struct setup_event_expr;
	friend struct write_event_expr;

public:
	typedef expr_base expr_type;
	typedef TRIG_TYPE trig_type;

	typedef event_expr_setup setup_type;
	typedef event_expr_trigger trigger_type;
	typedef event_expr_write write_type;

	event_expr(expr_type *, trig_type = TRIG_NONE);
	event_expr(const event_expr &);
	virtual ~event_expr();

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	void trigger(const trigger_type &);

private:
	trig_type _type;
	expr_type *_expr;

	// Keep a private copy of the event to be reused.
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;
	event_type *_event;
	};

struct event_expr_setup
	{
	virtual ~event_expr_setup() {}
	virtual void operator()(event_expr *) const {}
	};

struct event_expr_trigger
	{
	virtual ~event_expr_trigger() {}
	virtual void operator()(event_expr *) const {}
	};

struct event_expr_write
	{
	virtual ~event_expr_write() {}
	virtual void operator()(const event_expr *) const {}
	};

inline event_expr::ostream_type &
operator<<(event_expr::ostream_type &o, const event_expr &ee)
	{ return ee.display(o); }


// OR'ed event expression.

class ored_event_expr : public dec_base
	{
	friend struct trigger_dec;
	friend struct setup_dec;
	friend struct write_dec;

public:
	typedef event_expr event_expr_type;
	typedef basic_ptr<event_expr_type> event_expr_ptr;
	typedef list_type<event_expr_ptr> event_expr_list;

	ored_event_expr(event_expr_list *);
	ored_event_expr(const ored_event_expr &);
	virtual ~ored_event_expr();

	ored_event_expr *copy_constructor() const;
	bool is_event_control(void) const
		{ return true; }
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	int trigger(const trigger_type &);

private:
	event_expr_list *_ored_list;

	// Keep a private copy of the cache to be reused.
	typedef stmt_base stmt_type;
	typedef event_base<stmt_type> event_type;
	typedef event_type::event_container event_cache_type;
	event_cache_type *_cache;
	};

#endif // _EVNTEXPR_H
