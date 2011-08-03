// Verilog Behavioral Simulator
// Copyright (C) 2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// vbs.cc

#include "stmt/stmtbase.h"
#include "common/dumpinfo.h"
#include "common/scp_tab.h"
#include "common/sym_tab.h"
#include "common/event.h"
#include "common/time_whl.h"
#include "vbs.h"


vbs_engine::vbs_engine()
	{
	_dumpinfo = new dump_info;
	_scopetable = new scope_table;
	_symboltable = new symbol_table;
	_eventqueue = new event_queue<stmt_base>;
	_timewheel = new time_wheel<stmt_base>;
	}

vbs_engine::~vbs_engine()
	{
	delete _dumpinfo;
	delete _scopetable;
	delete _symboltable;
	delete _eventqueue;
	delete _timewheel;
	}

void
vbs_engine::initialize()
	{
	if (_instance == 0)
		_instance = new vbs_engine;
	}

void
vbs_engine::reset()
	{
	delete _instance;
	_instance = 0;
	}

vbs_engine *vbs_engine::_instance = 0;
