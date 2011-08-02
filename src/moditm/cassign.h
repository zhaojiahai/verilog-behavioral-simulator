// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// cassign.h

#ifndef _CASSIGN_H
#define _CASSIGN_H

#include "common/ptr.h"
#include "misc/decbase.h"
#include "moditm/mibase.h"
#include "stmt/assign.h"

class cont_assign : public module_item_base
	{
	friend struct setup_module_item;
	friend struct write_module_item;

public:
	typedef dec_base dec_type;
	typedef assignment_stmt assign_stmt;
	typedef basic_ptr<assign_stmt> assign_stmt_ptr;
	typedef list_type<assign_stmt_ptr> assign_stmt_list;

	cont_assign(assign_stmt_list *, dec_type * = 0);
	virtual ~cont_assign();

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);

private:
	dec_type *_dec; // Optional delay control.
	assign_stmt_list *_assign;
	};

#endif // _CASSIGN_H
