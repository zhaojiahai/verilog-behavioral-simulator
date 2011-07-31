// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_task.cc

#include "common/st_task.h"

st_task::st_task(const str_type &n, stmt_type *s, decl_list *d)
	: st_node_base(n, "task"), _delayed(false), _done_setup(false)
	{
	_stmt = s;
	_iovariable = 0;
	_declaration = d;
	}

st_task::~st_task()
	{
	delete _stmt;
	delete _iovariable;
	}

st_task *
st_task::get_task()
	{ return this; }

st_task::ostream_type &
st_task::display(ostream_type &s) const
	{
	s << "('" << _name << "'," << type_name() << ',' << _scope << ')';
	return s;
	}

void
st_task::setup(const setup_type &setup)
	{ setup(this); }

bool
st_task::trigger(const trigger_type &trigger)
	{ return trigger(this); }
