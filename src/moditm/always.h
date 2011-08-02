// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// always.h

#ifndef _ALWAYS_H
#define _ALWAYS_H

#include "stmt/stmtbase.h"
#include "moditm/mibase.h"

class always : public module_item_base
	{
	friend struct setup_module_item;
	friend struct write_module_item;

public:
	typedef stmt_base stmt_type;

	always(stmt_type *);
	virtual ~always();

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);

private:
	stmt_type *_stmt;
	};

#endif // _ALWAYS_H
