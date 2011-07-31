// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_func.cc

#include "common/st_func.h"

st_function::st_function(const str_type &n, stmt_type *s, decl_list *d)
	: st_node_base(n, "function"), _done_setup(false)
	{
	_stmt = s;
	_iovariable = 0;
	_declaration = d;
	_monitor = 0;
	}

st_function::~st_function()
	{
	delete _stmt;
	delete _iovariable;
	delete _monitor;
	}

st_function *
st_function::get_function()
	{ return this; }

void
st_function::assignment(const num_type &num, position_type ms, position_type ls,
						position_type m, position_type l)
	{
	if (m >= 0)
		{
		if (ms >= 0)
			_storage(m, l) = num(ms, ls);
		else
			_storage(m, l) = num;
		}
	else
		{
		if (ms >= 0)
			_storage = num(ms, ls);
		else
			_storage = num;
		}
	}

st_function::ostream_type &
st_function::display(ostream_type &s) const
	{
	s << "('" << _name << "'," << type_name() << ',' << _scope << ')';
	return s;
	}

bool
st_function::monitor(const monitor_type &mon)
	{ return mon(this); }

const st_function::num_type &
st_function::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

void
st_function::setup(const setup_type &setup)
	{ setup(this); }
