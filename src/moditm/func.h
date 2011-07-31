// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// func.h

#ifndef _FUNC_H
#define _FUNC_H

#include "common/ptr.h"
#include "misc/partsel.h"
#include "stmt/stmtbase.h"
#include "moditm/tfbase.h"
#include "moditm/mibase.h"

class function : public module_item_base
	{
	friend struct setup_module_item;
	friend struct write_module_item;

public:
	typedef part_select range_type;
	typedef stmt_base stmt_type;
	typedef tfdecl_base decl_type;
	typedef basic_ptr<decl_type> decl_ptr;
	typedef list_type<decl_ptr> decl_list;

	function(const char *, stmt_type *, decl_list *, range_type * = 0);
	~function();

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	function *get_function();

private:
	str_type _name;
	range_type *_retsize;
	stmt_type *_stmt;
	decl_list *_decl;
	};

#endif // _FUNC_H
