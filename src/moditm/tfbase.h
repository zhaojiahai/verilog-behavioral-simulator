// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// tfbase.h

#ifndef _TFBASE_H
#define _TFBASE_H

#include "common/ptr.h"
#include "expr/rangeid.h"
#include "expr/exprbase.h"
#include "misc/partsel.h"
#include "moditm/mitfbase.h"

struct tfdecl_base;
struct tfdecl_read
	{
	virtual tfdecl_base *operator()() const { return 0; }
	};

struct tfdecl_base : public virtual mitf_base
	{
	typedef part_select range_type;
	typedef range_id ident_type;
	typedef expr_base expr_type;
	struct decl_assign
		{
		basic_ptr<ident_type> _ident;
		basic_ptr<expr_type> _rval;
		};
	typedef decl_assign decl_assign_type;
	typedef basic_ptr<decl_assign_type> decl_assign_ptr;
	typedef list_type<decl_assign_ptr> decl_assign_list;
	typedef basic_ptr<ident_type> ident_ptr;
	typedef list_type<ident_ptr> ident_list;
	typedef tfdecl_read read_type;

	virtual ~tfdecl_base()
		{}

	static tfdecl_base *read(const read_type &rd)
		{ return rd(); }
	};

inline tfdecl_base::ostream_type &
operator<<(tfdecl_base::ostream_type &s, const tfdecl_base &d)
	{ return d.display(s); }

#endif // _TFBASE_H
