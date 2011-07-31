// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// func.cc

#include "moditm/func.h"

function::function(const char *n, stmt_type *s, decl_list *d, range_type *r)
	: _name(n)
	{
	_retsize = r;
	_stmt = s;
	_decl = d;
	}

function::~function()
	{
	delete _retsize;
	delete _stmt;
	delete _decl;
	}

function::ostream_type &
function::display(ostream_type &s) const
	{
	s << "function ";
	if (_retsize != 0)
		s << *_retsize << ' ';
	s << _name << ';' << endl;
	decl_list::iterator idx(_decl->begin());
	decl_list::iterator stop(_decl->end());
	for (; idx != stop; ++idx)
		s << *(*idx);
	s << *_stmt;
	s << "endfunction" << endl;
	return s;
	}

void
function::write(const write_type &wr) const
    { wr(this); }

void
function::setup(const setup_type &setup)
	{ setup(this); }

function *
function::get_function()
	{ return this; }
