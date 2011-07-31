// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// partsel.h

#ifndef _PARTSEL_H
#define _PARTSEL_H

#include "expr/exprbase.h"
#include "misc/selbase.h"

class part_select : public select_base
	{
	friend struct monitor_select;
	friend struct setup_select;
	friend struct write_select;

public:
	typedef select_base select_type;
	typedef expr_base expr_type;

	part_select(expr_type *, expr_type *);
	part_select(const part_select &);
	~part_select();

	select_size_type get_data(position_type &, position_type &) const;
	part_select *copy_constructor() const;

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	void setup(const setup_type &);

private:
	position_type _ln;
	position_type _rn;
	expr_type *_le;
	expr_type *_re;
	};

#endif // _PARTSEL_H
