// Verilog Behavioral Simulator
// Copyright (C) 2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// vbs.cc

#include "common/error.h"
#include "common/dumpinfo.h"
#include "common/scp_tab.h"
#include "common/sym_tab.h"
#include "common/event.h"
#include "common/time_whl.h"
#include "expr/emon.h"
#include "misc/msetup.h"
#include "stmt/stmtbase.h"
#include "stmt/ssetup.h"
#include "moditm/misetup.h"
#include "moditm/d_setup.h"
#include "vbs.h"


vbs_engine::vbs_engine()
	{
	_dumpinfo = new dump_info;
	_scopetable = new scope_table;

	_symboltable = new symbol_table;
	_eventqueue = new event_queue<stmt_base>;
	_timewheel = new time_wheel<stmt_base>;

	_modulelist = new std::list<hash_value>;
	}

vbs_engine::~vbs_engine()
	{
	delete _dumpinfo;
	delete _scopetable;

	// The rest needs to be deleted in a specific order to allow any
	// smart pointers to deallocate first.
	delete _modulelist;

	delete _timewheel;
	delete _eventqueue;
	delete _symboltable; // Delete last since it contains everything.
	}

void
vbs_engine::initialize()
	{
	if (_instance == 0)
		_instance = new vbs_engine;

	// Clear other global variables to restart the engine.

	// Reset parser error code reporting.
	extern vbs_error::value_type parse_error_code;
	parse_error_code = vbs_error::SE_NONE;

	// Reallocate event list used by monitor_expr.
	monitor_expr::reset();

	// Reallocate postponed list of postponed setup statements.
	setup_stmt::reset();

	// Reallocate statements for port IO.
	setup_port::reset();

	// Reallocate statement list.
	setup_module_item::reset();

	// Reallocate statement list.
	decl_setup::reset();
	}

void
vbs_engine::reset()
	{
	delete _instance;
	_instance = 0;

	// Reallocate instance and perform other initialization.
	initialize();
	}

vbs_engine *vbs_engine::_instance;
