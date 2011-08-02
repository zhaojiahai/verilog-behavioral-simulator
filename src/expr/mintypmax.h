// Verilog Behavioral Simulator
// Copyright (C) 1997,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mintypmax.h

#ifndef _MINTYPMAX_H
#define _MINTYPMAX_H

#include "expr/exprbase.h"

class mintypmax_expr : public expr_base
	{
	friend struct monitor_expr;
	friend struct evaluate_expr;
	friend struct setup_expr;
	friend struct trigger_expr;
	friend struct write_expr;

public:
	typedef expr_base expr_type;

	enum delay_type { MINDELAY, TYPDELAY, MAXDELAY };

	mintypmax_expr(expr_type *, expr_type * = 0, expr_type * = 0);
	mintypmax_expr(const mintypmax_expr &);
	virtual ~mintypmax_expr();

	mintypmax_expr *copy_constructor() const;
	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool monitor(const monitor_type &) const;
	const num_type &evaluate(const evaluate_type &) const;
	size_type setup(const setup_type &);
	void trigger(const trigger_type &);

	static void min_delay();
	static void typ_delay();
	static void max_delay();

protected:
	static delay_type _delay;

private:
	expr_type *_min_expr;
	expr_type *_typ_expr;
	expr_type *_max_expr;
	};

#endif // _MINTYPMAX_H
