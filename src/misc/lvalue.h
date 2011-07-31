// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// lvalue.h

#ifndef _LVALUE_H
#define _LVALUE_H

#include "common/ptr.h"
#include "expr/exprbase.h"
#include "expr/rangeid.h"

class st_node_base;
struct lvalue_setup;
struct lvalue_trigger;
struct lvalue_write;

class lvalue : public virtual common_base
	{
	friend struct setup_lvalue;
	friend struct trigger_lvalue;
	friend struct write_lvalue;

public:
	typedef range_id ident_type;
	typedef ident_type::select_type select_type;
	typedef ident_type::select_size_type select_size_type;
	typedef ident_type::position_type position_type;
	typedef st_node_base node_type;
	typedef expr_base expr_type;
	typedef basic_ptr<expr_type> expr_ptr;
	typedef list_type<expr_ptr> expr_list;
	typedef expr_type::num_type num_type;
	typedef num_type::size_type size_type;
	struct element_type
		{
		position_type ms;
		position_type ls;
		position_type targ_ms;
		position_type targ_ls;
		select_size_type targ_size_type;
		element_type(position_type m = -1, position_type l = -1)
			: ms(m), ls(l)
			{}
		};
	typedef list_type<element_type> element_list;

	typedef lvalue_setup setup_type;
	typedef lvalue_trigger trigger_type;
	typedef lvalue_write write_type;

	lvalue(expr_list *);
	lvalue(const lvalue &);
	~lvalue();

	void set_target(void);
	void clr_target(void);

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);

protected:
	element_list _elmt_list;
	expr_list *_exp_list;
	};

lvalue::ostream_type &operator<<(lvalue::ostream_type &s, const lvalue &);

struct lvalue_setup
	{
	typedef lvalue::size_type size_type;
	virtual size_type operator()(lvalue *) const { return 0; }
	};

struct lvalue_trigger
	{
	virtual void operator()(lvalue *) const {}
	};

struct lvalue_write
	{
	virtual void operator()(const lvalue *) const {}
	};

#endif // _LVALUE_H
