// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2003 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// case.cc

#include "expr/exprbase.h"
#include "stmt/case.h"

case_item::case_item(stmt_type *s, expr_list *e)
	{
	_stmt = s;
	_expr = e;
	}

case_item::case_item(const case_item &p)
	: common_base(p)
	{
	_stmt = p._stmt->copy_constructor();
	if (!p.is_default())
		{
		_expr = new expr_list;
		expr_list::iterator itp(p._expr->begin());
		expr_list::iterator stop(p._expr->end());
		for (; itp != stop; ++itp)
			_expr->push_back((*itp)->copy_constructor());
		}
	else
		_expr = 0;
	}

case_item::~case_item()
	{
	delete _stmt;
	delete _expr;
	}

case_item::ostream_type &
case_item::display(ostream_type &s) const
	{
	if (is_default())
		s << "default : " << endl;
	else
		{
		expr_list::iterator itp(_expr->begin());
		expr_list::iterator stop(_expr->end());
		s << *(*itp);
		for (++itp; itp != stop; ++itp)
			s << ", " << *(*itp);
		s << ":" << endl;
		}
	s << *_stmt;
	return s;
	}

void
case_item::write(const write_type &wr) const
	{ wr(this); }

void
case_item::setup(const setup_type &setup)
	{ setup(this); }

bool
case_item::trigger(const trigger_type &trigger)
	{ return trigger(this); }


// Case statement.

case_stmt::case_stmt(case_type t, expr_type *e, citem_list *c)
	: _type(t)
	{
	_expr = e;
	_case_item = c;
	}

case_stmt::case_stmt(const case_stmt &p)
	: common_base(p), stmt_base(p), _type(p._type)
	{
	_expr = p._expr->copy_constructor();
	_case_item = new citem_list;
	citem_list::iterator itp(p._case_item->begin());
	citem_list::iterator stop(p._case_item->end());
	for (; itp != stop; ++itp)
		_case_item->push_back(new case_item(*(*itp)));
	}

case_stmt::~case_stmt()
	{
	delete _expr;
	delete _case_item;
	}

case_stmt *
case_stmt::copy_constructor() const
	{ return new case_stmt(*this); }

case_stmt::ostream_type &
case_stmt::display(ostream_type &s) const
	{
	stmt_base::display(s); // Delay or event control...
	switch (_type)
		{
		case citem_type::CASE_X: s << "casex ("; break;
		case citem_type::CASE_Z: s << "casez ("; break;
		default: s << "case ("; break;
		}
	s << *_expr << ")" << endl;
	citem_list::iterator itp(_case_item->begin());
	citem_list::iterator stop(_case_item->end());
	for (; itp != stop; ++itp)
		s << *(*itp);
	s << "endcase" << endl;
	return s;
	}

void
case_stmt::write(const write_type &wr) const
	{ wr(this); }

void
case_stmt::setup(const setup_type &setup)
	{ setup(this); }

bool
case_stmt::trigger(const trigger_type &trigger)
	{ return trigger(this); }
