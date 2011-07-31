// Verilog Behavioral Simulator
// Copyright (C) 1997,2001-2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// concat.h

#ifndef _CONCAT_H
#define _CONCAT_H

#include "expr/exprbase.h"
#include "common/ptr.h"

class concatenation : public expr_base
	{
	friend struct monitor_expr;
	friend struct evaluate_expr;
	friend struct setup_expr;
	friend struct trigger_expr;
	friend struct write_expr;

public:
	typedef expr_base::ostream_type ostream_type;
	typedef expr_base expr_type;
	typedef basic_ptr<expr_type> expr_ptr;
	typedef list_type<expr_ptr> expr_list;
	typedef expr_type::num_type num_type;
	typedef num_type::position_type position_type;
	typedef num_type::size_type size_type;
	struct element_type
		{
		position_type ms;
		position_type ls;
		element_type(position_type m = -1, position_type l = -1)
			: ms(m), ls(l)
			{}
		};
	typedef list_type<element_type> element_list;

	concatenation(expr_list *, expr_type * = 0);
	concatenation(const concatenation &);
	~concatenation();

	concatenation *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	const num_type &evaluate(const evaluate_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);

private:
	size_type _repeat_count;
	expr_type *_repeat_expr;
	element_list _elmt_list;
	expr_list *_exp_list;
	};

#endif // _CONCAT_H
