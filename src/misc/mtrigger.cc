// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mtrigger.cc

#include "common/sym_tab.h"
#include "common/st_func.h"
#include "common/st_net.h"
#include "misc/lvalue.h"
#include "misc/mtrigger.h"

extern symbol_table symboltable;

// Lvalue for assignment statement.

void
trigger_lvalue::operator()(lvalue *l) const
	{
	lvalue::expr_list::iterator itp(l->_exp_list->begin());
	lvalue::expr_list::iterator stop(l->_exp_list->end());
	lvalue::element_list::iterator eitp(l->_elmt_list.begin());
	lvalue::ident_type *rid;
	node_type *node;
	net_type *net;
	func_type *func;
	position_type targ_ms, targ_ls, ms, ls;
	select_size_type type;

	for (; itp != stop; ++itp, ++eitp)
		{
		// Get the storage object from the symbol table.
		rid = (*itp)->get_range_id();
		node = symboltable.get(rid->index());

		ms = (*eitp).ms;
		ls = (*eitp).ls;
		type = (*eitp).targ_size_type;
		if (type != select_base::SELECT_NONE)
			{
			// Non-blocking assignment--target was pre-evaluated.
			targ_ms = (*eitp).targ_ms;
			targ_ls = (*eitp).targ_ls;
			}
		else
			{
			// Blocking assignment--evaluate target now.
			type = rid->get_select(targ_ms, targ_ls);
			}

		net = node->get_net();
		if (net != 0)
			net->assignment(_num, ms, ls, targ_ms, targ_ls);
		else
			{
			func = node->get_function();
			func->assignment(_num, ms, ls, targ_ms, targ_ls);
			}
		}
	}
