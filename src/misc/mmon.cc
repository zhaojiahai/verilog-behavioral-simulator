// Verilog Behavioral Simulator
// Copyright (C) 2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mmon.cc

#include "expr/emon.h"
#include "stmt/stmtbase.h"
#include "misc/bitsel.h"
#include "misc/partsel.h"
#include "misc/mmon.h"

bool
monitor_select::operator()(const bit_select *p) const
	{ return p->_expr->monitor(monitor_expr(_event, true)); }

bool
monitor_select::operator()(const part_select *p) const
	{
	return (p->_le->monitor(monitor_expr(_event, true))
		&& p->_re->monitor(monitor_expr(_event, true)));
	}
