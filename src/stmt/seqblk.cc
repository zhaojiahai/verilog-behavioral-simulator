// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002-2003 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// seqblk.cc

#include "stmt/seqblk.h"

seq_block_stmt::seq_block_stmt(stmt_list *s)
	: _pushed(false)
	{ _stmt = s; }

seq_block_stmt::seq_block_stmt(const seq_block_stmt &p)
	: common_base(p), stmt_base(p), _pushed(false)
	{
	_stmt = new stmt_list;
	stmt_list::iterator itp(p._stmt->begin());
	stmt_list::iterator stop(p._stmt->end());
	for (; itp != stop; ++itp)
		_stmt->push_back((*itp)->copy_constructor());
	}

seq_block_stmt::~seq_block_stmt()
	{ delete _stmt; }

seq_block_stmt *
seq_block_stmt::get_seq_block()
	{ return this; }

seq_block_stmt *
seq_block_stmt::copy_constructor() const
	{ return new seq_block_stmt(*this); }

seq_block_stmt::ostream_type &
seq_block_stmt::display(ostream_type &s) const
	{
	stmt_base::display(s); // Delay or event control...
	s << "begin" << endl;
	stmt_list::iterator itp(_stmt->begin());
	stmt_list::iterator stop(_stmt->end());
	for (; itp != stop; ++itp)
		s << *(*itp);
	s << "end" << endl;
	return s;
	}

void
seq_block_stmt::write(const write_type &wr) const
	{ wr(this); }

void
seq_block_stmt::setup(const setup_type &setup)
	{ setup(this); }

bool
seq_block_stmt::trigger(const trigger_type &trigger)
	{ return trigger(this); }
