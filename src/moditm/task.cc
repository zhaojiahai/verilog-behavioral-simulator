// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// task.cc

#include "moditm/task.h"

task::task(const char *n, stmt_type *s, decl_list *d)
	: _name(n)
	{ _stmt = s; _decl = d; }

task::~task()
	{ delete _stmt; delete _decl; }

task::ostream_type &
task::display(ostream_type &s) const
	{
	s << "task " << _name << ';' << endl;
	decl_list::iterator idx(_decl->begin());
	decl_list::iterator stop(_decl->end());
	for (; idx != stop; ++idx)
		s << *(*idx);
	s << *_stmt;
	s << "endtask" << endl;
	return s;
	}

void
task::write(const write_type &wr) const
    { wr(this); }

void
task::setup(const setup_type &setup)
	{ setup(this); }

task *
task::get_task()
	{ return this; }
