// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// funccall.cc

#include "expr/funccall.h"

function_call::function_call(const char *n, arg_list *a)
	: _index(), _name(n) { _argument = a; }

function_call::function_call(const function_call &p)
	: common_base(p), expr_base(p), _index(), _name(p._name)
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
	}

function_call::~function_call()
	{ delete _argument; }

function_call *
function_call::get_function_call()
	{ return this; }

function_call *
function_call::copy_constructor() const
	{ return new function_call(*this); }

function_call::ostream_type &
function_call::display(ostream_type &s) const
	{
	s << _name << '(';
	if (_argument)
		{
		arg_list::iterator itp(_argument->begin());
		arg_list::iterator stop(_argument->end());
		(*itp)->display(s);
		for (++itp; itp != stop; ++itp)
			{
			s << ", ";
			(*itp)->display(s);
			}
		}
	s << ")";
	return s;
	}

void
function_call::write(const write_type &wr) const
	{ wr(this); }

bool
function_call::monitor(const monitor_type &mon) const
	{ return mon(this); }

const function_call::num_type &
function_call::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

function_call::size_type
function_call::setup(const setup_type &setup)
	{ return setup(this); }

void
function_call::trigger(const trigger_type &trigger)
	{ trigger(this); }
