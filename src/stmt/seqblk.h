// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// seqblk.h

#ifndef _SEQBLK_H
#define _SEQBLK_H

#include "common/ptr.h"
#include "common/dstack.h"
#include "stmt/stmtbase.h"

class seq_block_stmt : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;
	friend struct write_stmt;

public:
	typedef stmt_base stmt_type;
	typedef basic_ptr<stmt_type> stmt_ptr;
	typedef list_type<stmt_ptr> stmt_list;
	typedef Stack<seq_block_stmt *> stmt_stack;

	seq_block_stmt(stmt_list *);
	seq_block_stmt(const seq_block_stmt &);
	virtual ~seq_block_stmt();

	seq_block_stmt *get_seq_block();
	seq_block_stmt *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	stmt_list *_stmt;

public:
	// Simulation info.
	bool _pushed; // Are we on the stack?
	stmt_list::iterator _curstmt; // Current statement being exec'ed.
	stmt_stack _stk; // Stack of nested blocks.
	};

#endif // _SEQBLK_H
