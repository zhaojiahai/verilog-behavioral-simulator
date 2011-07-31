// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// decsetup.cc

#include "common/error.h"
#include "common/sym_tab.h"
#include "expr/esetup.h"
#include "expr/eeval.h"
#include "expr/emon.h"
#include "misc/delaynum.h"
#include "misc/delayid.h"
#include "misc/evntexpr.h"
#include "misc/decsetup.h"

extern symbol_table symboltable;


struct setup_event_expr : public event_expr_setup
	{
	typedef Stack<int> scopelist_type;
	typedef stmt_base stmt_type;
	typedef stmt_type::strstream_type strstream_type;
	typedef event_expr::expr_type expr_type;
	typedef expr_type::num_type num_type;
	typedef event_base<stmt_type> event_type;
	typedef event_type::event_container event_cache_type;

	setup_event_expr(scopelist_type &s, event_cache_type *e)
		: _scope(s)
		{ _cache = e; }

	void operator()(event_expr *) const;

	scopelist_type &_scope;
	event_cache_type *_cache;
	};

void
setup_event_expr::operator()(event_expr *p) const
	{
	// Setup expression for later use.
	if (p->_type != TRIG_NEGEDGE
	 && p->_type != TRIG_POSEDGE
	 && p->_type != TRIG_CHNG)
		{
		strstream_type buf;
		buf << *p;
		vbs_err.set_data(vbs_error::SE_INTERNAL, p->_lineno);
		vbs_err.out(buf);
		}

	p->_expr->setup(setup_expr(_scope));
	const num_type &initial_value(p->_expr->evaluate(evaluate_expr()));
	if (p->_type == TRIG_NEGEDGE)
		{
		p->_event = new negedge_event<stmt_type>(_cache, initial_value);
		p->_event->monitor_bit(p->_expr);
		}
	else if (p->_type == TRIG_POSEDGE)
		{
		p->_event = new posedge_event<stmt_type>(_cache, initial_value);
		p->_event->monitor_bit(p->_expr);
		}
	else if (p->_type == TRIG_CHNG)
		{
		p->_event = new change_event<stmt_type>(_cache, initial_value);
		}

	// Check to make sure we can actually monitor this expression.
	monitor_expr::event_ptr eptr(p->_event);
	if (!p->_expr->monitor(monitor_expr(&eptr)))
		{
		strstream_type buf;
		buf << *(p->_expr);
		vbs_err.set_data(vbs_error::SE_TYPE, p->_lineno);
		vbs_err.out(buf);
		}
	}


// Setup delay or event control function object.

void
setup_dec::operator()(delay_num *p) const
	{
	// An event object is needed in case the delay amount is zero.
	// In which case, we need to append this event into the event
	// queue.
	num_type zero(0UL);
	if (*p == zero)
		{
		event_cache_type *c = new event_cache_type(false, _stmt);
		p->_event = new nonblock_event<stmt_type>(c, DC);
		}
	}

void
setup_dec::operator()(delay_id *p) const
	{
	// Setup expression.
	p->_expr->setup(setup_expr(_scope));

	// This object is always needed because we do not know if the
	// expression will evaluate to zero.  If it does, we need this
	// object.
	event_cache_type *c = new event_cache_type(false, _stmt);
	p->_event = new nonblock_event<stmt_type>(c, DC);
	}

void
setup_dec::operator()(ored_event_expr *p) const
	{
	ored_event_expr::event_expr_list::iterator itp(p->_ored_list->begin());
	ored_event_expr::event_expr_list::iterator stop(p->_ored_list->end());
	p->_cache = new event_cache_type(_parent_stmt->_always, _parent_stmt);
	for (; itp != stop; ++itp)
		(*itp).get()->setup(setup_event_expr(_scope, p->_cache));
	}
