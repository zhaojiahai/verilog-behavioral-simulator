// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// evntexpr.cc

#include "expr/exprbase.h"
#include "misc/evntexpr.h"

event_expr::event_expr(expr_type *e, TRIG_TYPE t)
	: _type(t)
	{
	_expr = e;
	_event = 0;
	}

event_expr::event_expr(const event_expr &p)
	: common_base(p), _type(p._type)
	{
	_expr = p._expr->copy_constructor();
	_event = 0;
	}

event_expr::~event_expr()
	{
	// _event is managed by a counted_ptr, so no need to delete manually.
	delete _expr;
	}

event_expr::ostream_type &
event_expr::display(ostream_type &s) const
	{
	if (_type == TRIG_POSEDGE)
		s << "posedge ";
	else if (_type == TRIG_NEGEDGE)
		s << "negedge ";
	else if (_type != TRIG_CHNG)
		s << "change  ";
	s << *_expr;
	return s;
	}

void
event_expr::write(const write_type &wr) const
	{ wr(this); }

void
event_expr::setup(const setup_type &setup)
	{ setup(this); }

void
event_expr::trigger(const trigger_type &trigger)
	{ trigger(this); }


// Ored event expressions.

ored_event_expr::ored_event_expr(event_expr_list *e)
	{
	_ored_list = e;
	_cache = 0;
	}

ored_event_expr::ored_event_expr(const ored_event_expr &p)
	: common_base(p), dec_base(p)
	{
	_ored_list = new event_expr_list;
	event_expr_list::iterator itp(p._ored_list->begin());
	event_expr_list::iterator stop(p._ored_list->end());
	for (; itp != stop; ++itp)
		_ored_list->push_back(new event_expr_type(*(*itp)));
	_cache = 0;
	}

ored_event_expr::~ored_event_expr()
	{ delete _ored_list; }

ored_event_expr *
ored_event_expr::copy_constructor() const
	{ return new ored_event_expr(*this); }

ored_event_expr::ostream_type &
ored_event_expr::display(ostream_type &s) const
	{
	s << "@(";
	event_expr_list::iterator itp(_ored_list->begin());
	event_expr_list::iterator stop(_ored_list->end());
	s << *((*itp).get());
	for (++itp; itp != stop; ++itp)
		s << " or " << *((*itp).get());
	s << ')';
	return s;
	}

void
ored_event_expr::write(const write_type &wr) const
	{ wr(this); }

void
ored_event_expr::setup(const setup_type &setup)
	{ setup(this); }

int
ored_event_expr::trigger(const trigger_type &trigger)
	{ return trigger(this); }
