// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// case.h

#ifndef _CASE_H
#define _CASE_H

#include "common/ptr.h"
#include "common/combase.h"
#include "expr/exprbase.h"
#include "stmt/stmtbase.h"

struct case_item_setup;
struct case_item_trigger;
struct case_item_write;

class case_item : public virtual common_base
	{
	friend struct setup_case_item;
	friend struct trigger_case_item;
	friend struct write_case_item;

public:
	typedef expr_base expr_type;
	typedef basic_ptr<expr_type> expr_ptr;
	typedef list_type<expr_ptr> expr_list;
	typedef stmt_base stmt_type;

	typedef case_item_setup setup_type;
	typedef case_item_trigger trigger_type;
	typedef case_item_write write_type;

	enum case_type
		{
		CASE_C,
		CASE_X,
		CASE_Z
		};

	case_item(stmt_type *, expr_list * = 0);
	case_item(const case_item &);
	~case_item();

	bool is_default() const
		{ return (_expr == 0); }
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	stmt_type *_stmt;
	expr_list *_expr;
	};

inline case_item::ostream_type &
operator<<(case_item::ostream_type &o, const case_item &ci)
	{ return ci.display(o); }

class case_stmt : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;
	friend struct write_stmt;

public:
	typedef expr_base expr_type;
	typedef case_item citem_type;
	typedef citem_type::case_type case_type;
	typedef basic_ptr<citem_type> citem_ptr;
	typedef list_type<citem_ptr> citem_list;

	case_stmt(case_type, expr_type *, citem_list *);
	case_stmt(const case_stmt &);
	virtual ~case_stmt();

	case_stmt *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	case_type _type;
	citem_ptr _def; // Default case, set by setup.
	expr_type *_expr;
	citem_list *_case_item;
	};

struct case_item_setup
	{
	virtual ~case_item_setup() {}
	virtual void operator()(case_item *) const {}
	};

struct case_item_trigger
	{
	virtual ~case_item_trigger() {}
	virtual bool operator()(case_item *) const { return true; }
	};

struct case_item_write
	{
	virtual ~case_item_write() {}
	virtual void operator()(const case_item *) const {}
	};

#endif // _CASE_H
