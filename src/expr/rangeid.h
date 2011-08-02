// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// rangeid.h

#ifndef _RANGEID_H
#define _RANGEID_H

#include "common/hash.h"
#include "common/combase.h"
#include "expr/exprbase.h"
#include "misc/selbase.h"

class range_id : public expr_base
	{
	friend struct monitor_expr;
	friend struct evaluate_expr;
	friend struct setup_expr;
	friend struct trigger_expr;
	friend struct write_expr;

	friend struct setup_lvalue;
	friend struct trigger_lvalue;

public:
	typedef hash_value hash_type;
	typedef select_base select_type;
	typedef select_type::SELECT_SIZE select_size_type;
	typedef select_type::position_type position_type;

	range_id(const char *, select_type *r = 0);
	range_id(const range_id &);
	virtual ~range_id();

	const str_type &name() const
		{ return _name; }
	select_type *select()
		{ return _select; }
	const hash_type &index() const
		{ return _index; }
	void index(const hash_type &h)
		{ _index = h; }
	select_size_type get_select(position_type &ms, position_type &ls) const
		{
		select_size_type type;
		if (_select != 0)
			type = _select->get_data(ms, ls);
		else
			{
			ms = -1;
			ls = -1;
			type = select_type::SELECT_NONE;
			}
		return type;
		}
	range_id *get_range_id();

	range_id *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	const num_type &evaluate(const evaluate_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);

protected:
	hash_type _index; // Index into symbol table.
	str_type _name; // Name of variable.
	select_type *_select; // Optional range/bit select.
	};

#endif // _RANGEID_H
