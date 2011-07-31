// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// exprbase.cc

#include "expr/exprbase.h"


expr_base *
expr_base::read(const read_type &rd)
	{
	return rd();
	}

expr_base::ostream_type &
display_unary_expr(expr_base::ostream_type &s,
		const expr_base &e, const char *op)
	{
	s << op << '(';
	e.display(s);
	s << ')';
	return s;
	}

expr_base::ostream_type &
display_binary_expr(expr_base::ostream_type &s,
		const expr_base &l, const expr_base &r,
		const char *op)
	{
	s << '(';
	l.display(s);
	s << ") " << op << " (";
	r.display(s);
	s << ')';
	return s;
	}
