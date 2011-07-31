// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// exprbase.h
//
// Base class for all expressions.

#ifndef _EXPRBASE_H
#define _EXPRBASE_H

#include "common/bvector.h"
#include "common/combase.h"

class unary_op_expr;
class binary_op_expr;
class ternary_op_expr;
class mintypmax_expr;

class function_call;
class concatenation;
class range_id;
class qouted_str;
class number;

struct expr_monitor;
struct expr_evaluate;
struct expr_setup;
struct expr_trigger;
struct expr_read;
struct expr_write;

struct expr_base : public virtual common_base
	{
	typedef bit_vector num_type;
	typedef num_type::size_type size_type;

	typedef expr_monitor monitor_type;
	typedef expr_evaluate evaluate_type;
	typedef expr_setup setup_type;
	typedef expr_trigger trigger_type;
	typedef expr_read read_type;
	typedef expr_write write_type;

	expr_base(bool ce = false)
		: _is_const_expr(ce)
		{ _result = 0; }
	expr_base(const expr_base &p)
		: common_base(p), _is_const_expr(p._is_const_expr)
		{ _result = (p._result != 0) ? new num_type(*p._result) : 0; }
	virtual ~expr_base()
		{ delete _result; }

	// Functions which evaluate/setup/trigger functions need.
	virtual function_call *get_function_call()
		{ return 0; }
	virtual qouted_str *get_qouted_str()
		{ return 0; }
	virtual range_id *get_range_id()
		{ return 0; }
	virtual number *get_number()
		{ return 0; }
	virtual bool constant() const
		{ return _is_const_expr; }

	virtual expr_base *copy_constructor() const
		{ return new expr_base(*this); }
	virtual ostream_type &display(ostream_type &s) const
		{ return s; }

	static expr_base *read(const read_type &);
	virtual void write(const write_type &) const
		{}

	virtual bool monitor(const monitor_type &) const
		{ return false; } // Return success or failure.
	virtual const num_type &evaluate(const evaluate_type &) const
		{ static num_type dummy(DC); return dummy; }
	virtual size_type setup(const setup_type &)
		{ return 0; }
	virtual void trigger(const trigger_type &)
		{}

protected:
	bool _is_const_expr;
	num_type *_result;
	};

inline expr_base::ostream_type &
operator<<(expr_base::ostream_type &s, const expr_base &d)
	{ return d.display(s); }

expr_base::ostream_type &
display_unary_expr(expr_base::ostream_type &, const expr_base &, const char *);
expr_base::ostream_type &
display_binary_expr(expr_base::ostream_type &,
	const expr_base &, const expr_base &, const char *);

struct expr_monitor
	{
	virtual bool operator()(const function_call *) const
		{ return false; }
	virtual bool operator()(const concatenation *) const
		{ return false; }
	virtual bool operator()(const number *) const
		{ return false; }
	virtual bool operator()(const qouted_str *) const
		{ return false; }
	virtual bool operator()(const range_id *) const
		{ return false; }
	virtual bool operator()(const unary_op_expr *) const
		{ return false; }
	virtual bool operator()(const binary_op_expr *) const
		{ return false; }
	virtual bool operator()(const ternary_op_expr *) const
		{ return false; }
	virtual bool operator()(const mintypmax_expr *) const
		{ return false; }
	};

struct expr_evaluate
	{
	typedef expr_base::num_type num_type;

	virtual const num_type &operator()(const function_call *) const
		{ static num_type dummy(DC); return dummy; }
	virtual const num_type &operator()(const concatenation *) const
		{ static num_type dummy(DC); return dummy; }
	virtual const num_type &operator()(const number *) const
		{ static num_type dummy(DC); return dummy; }
	virtual const num_type &operator()(const qouted_str *) const
		{ static num_type dummy(DC); return dummy; }
	virtual const num_type &operator()(const range_id *) const
		{ static num_type dummy(DC); return dummy; }
	virtual const num_type &operator()(const unary_op_expr *) const
		{ static num_type dummy(DC); return dummy; }
	virtual const num_type &operator()(const binary_op_expr *) const
		{ static num_type dummy(DC); return dummy; }
	virtual const num_type &operator()(const ternary_op_expr *) const
		{ static num_type dummy(DC); return dummy; }
	virtual const num_type &operator()(const mintypmax_expr *) const
		{ static num_type dummy(DC); return dummy; }
	};

struct expr_setup
	{
	typedef expr_base::size_type size_type;

	virtual size_type operator()(function_call *) const { return 0; }
	virtual size_type operator()(concatenation *) const { return 0; }
	virtual size_type operator()(number *) const { return 0; }
	virtual size_type operator()(qouted_str *) const { return 0; }
	virtual size_type operator()(range_id *) const { return 0; }
	virtual size_type operator()(unary_op_expr *) const { return 0; }
	virtual size_type operator()(binary_op_expr *) const { return 0; }
	virtual size_type operator()(ternary_op_expr *) const { return 0; }
	virtual size_type operator()(mintypmax_expr *) const { return 0; }
	};

struct expr_trigger
	{
	virtual void operator()(function_call *) const {}
	virtual void operator()(concatenation *) const {}
	virtual void operator()(number *) const {}
	virtual void operator()(qouted_str *) const {}
	virtual void operator()(range_id *) const {}
	virtual void operator()(unary_op_expr *) const {}
	virtual void operator()(binary_op_expr *) const {}
	virtual void operator()(ternary_op_expr *) const {}
	virtual void operator()(mintypmax_expr *) const {}
	};

struct expr_write
	{
	virtual void operator()(const function_call *) const {}
	virtual void operator()(const concatenation *) const {}
	virtual void operator()(const number *) const {}
	virtual void operator()(const qouted_str *) const {}
	virtual void operator()(const range_id *) const {}
	virtual void operator()(const unary_op_expr *) const {}
	virtual void operator()(const binary_op_expr *) const {}
	virtual void operator()(const ternary_op_expr *) const {}
	virtual void operator()(const mintypmax_expr *) const {}
	};

struct expr_read
	{
	typedef expr_base expr_type;

	virtual expr_type *operator()(void) const
		{ return 0; }
	};

#endif // _EXPRBASE_H
