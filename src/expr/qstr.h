// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// qstr.h

#ifndef _QSTR_H
#define _QSTR_H

#include "expr/exprbase.h"

class qouted_str : public expr_base
	{
	friend struct evaluate_expr;
	friend struct setup_expr;
	friend struct trigger_expr;
	friend struct write_expr;

public:
	typedef expr_base::str_type str_type; // Another G++ 2.7.2 bug???
	typedef str_type::value_type value_type;

	qouted_str(const char *);
	qouted_str(const qouted_str &);

	str_type &get_str();
	const str_type &get_str() const;
	qouted_str *get_qouted_str();
	qouted_str *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	const num_type &evaluate(const evaluate_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);

private:
	str_type _str;
	num_type _value;
	};

#endif // _QSTR_H
