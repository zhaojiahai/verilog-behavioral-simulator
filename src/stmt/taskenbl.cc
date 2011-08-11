// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// taskenbl.cc

#include "stmt/taskenbl.h"

task_enable_stmt::task_enable_stmt(const char *n, arg_list *a)
	: _name(n), _scope(-1), _delayed(false)
	{ _argument = a; _event = 0; }

task_enable_stmt::task_enable_stmt(const task_enable_stmt &p)
	: common_base(p), stmt_base(p),
	  _name(p._name), _scope(p._scope), _delayed(p._delayed),
	  _index(p._index)
	{
	if (p._argument != 0)
		{
		_argument = new arg_list;
		arg_list::iterator itp(p._argument->begin());
		arg_list::iterator stop(p._argument->end());
		for (; itp != stop; ++itp)
			_argument->push_back((*itp)->copy_constructor());
		}
	else
		_argument = 0;
	_event = 0;
	}

task_enable_stmt::~task_enable_stmt()
	{ delete _argument; delete _event; }

bool
task_enable_stmt::delayed() const
	{ return _delayed; }

task_enable_stmt *
task_enable_stmt::get_task_enable()
	{ return this; }

task_enable_stmt *
task_enable_stmt::copy_constructor() const
	{ return new task_enable_stmt(*this); }

task_enable_stmt::ostream_type &
task_enable_stmt::display(ostream_type &s) const
	{
	stmt_base::display(s); // Delay or event control...
	s << _name;
	if (_argument != 0)
		{
		s << '(';
		arg_list::iterator itp(_argument->begin());
		arg_list::iterator stop(_argument->end());
		s << *(*itp);
		for (++itp; itp != stop; ++itp)
			s << ", " << *(*itp);
		s << ')';
		}
	s << ';' << endl;
	return s;
	}

void
task_enable_stmt::write(const write_type &wr) const
	{ wr(this); }

void
task_enable_stmt::setup(const setup_type &setup)
	{ setup(this); }

bool
task_enable_stmt::trigger(const trigger_type &trigger)
	{ return trigger(this); }
