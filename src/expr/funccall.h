// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// funccall.h

#ifndef _FUNCCALL_H
#define _FUNCCALL_H

#include "common/ptr.h"
#include "common/hash.h"
#include "expr/exprbase.h"

class function_call : public expr_base
	{
	friend struct monitor_expr;
	friend struct evaluate_expr;
	friend struct setup_expr;
	friend struct trigger_expr;
	friend struct write_expr;

	friend struct evaluate_st_node;

public:
	typedef hash_value hash_type;
	typedef expr_base expr_type;
	typedef basic_ptr<expr_type> arg_type;
	typedef list_type<arg_type> arg_list;

	function_call(const char *, arg_list * = 0);
	function_call(const function_call &);
	virtual ~function_call();

	const hash_type &index() const
		{ return _index; }
	function_call *get_function_call();

	function_call *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	const num_type &evaluate(const evaluate_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);

private:
	hash_type _index; // Index of function into symbol table.
	str_type _name;
	arg_list *_argument;
	};

#endif // _FUNCCALL_H
