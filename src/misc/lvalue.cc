// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// lvalue.cc

#include "misc/lvalue.h"

lvalue::lvalue(expr_list *elst)
	{ _exp_list = elst; }

lvalue::lvalue(const lvalue &p)
	: common_base(p)
	{
	expr_list::iterator itp(p._exp_list->begin());
	expr_list::iterator stop(p._exp_list->end());
	_exp_list = new expr_list;
	for (; itp != stop; ++itp)
		_exp_list->push_back((*itp)->copy_constructor());
	}

lvalue::~lvalue()
	{ delete _exp_list; }

void
lvalue::set_target(void)
	{
	expr_list::iterator itp(_exp_list->begin());
	expr_list::iterator stop(_exp_list->end());
	element_list::iterator eitp(_elmt_list.begin());
	position_type ms, ls;
	ident_type *rid;

	for (; itp != stop; ++itp, ++eitp)
		{
		rid = (*itp)->get_range_id();
		(*eitp).targ_size_type = rid->get_select(ms, ls);
		(*eitp).targ_ms = ms;
		(*eitp).targ_ls = ls;
		}
	}

void
lvalue::clr_target(void)
	{
	expr_list::iterator itp(_exp_list->begin());
	expr_list::iterator stop(_exp_list->end());
	element_list::iterator eitp(_elmt_list.begin());
	for (; itp != stop; ++itp, ++eitp)
		(*eitp).targ_size_type = ident_type::select_type::SELECT_NONE;
	}

lvalue::ostream_type &
lvalue::display(ostream_type &s) const
	{
	expr_list::iterator itp(_exp_list->begin());
	expr_list::iterator stop(_exp_list->end());

	if (_exp_list->size() > 1)
		{
		s << "{ ";
		(*itp)->display(s);
		for (++itp; itp != stop; ++itp)
			{
			s << ", ";
			(*itp)->display(s);
			}
		s << " }";
		}
	else
		{
		// Simple identifier.
		(*itp)->display(s);
		}

	return s;
	}

void
lvalue::write(const write_type &wr) const
	{ wr(this); }

lvalue::size_type
lvalue::setup(const setup_type &setup)
	{ return setup(this); }

void
lvalue::trigger(const trigger_type &trigger)
	{ trigger(this); }

lvalue::ostream_type &
operator<<(lvalue::ostream_type &s, const lvalue &d)
	{ return d.display(s); }
