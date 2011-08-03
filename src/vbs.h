// Verilog Behavioral Simulator
// Copyright (C) 2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// vbs.h
//
// This is the library interface to allow applications to use the
// simulator.  This is achieved by allowing the engine to be
// deleted and re-created for a new simulation.

#ifndef _VBS_H
#define _VBS_H

class stmt_base;

class dump_info;
class scope_table;
class symbol_table;
template<class T> class event_queue;
template<class T> class time_wheel;

class vbs_engine
	{
public:
	static dump_info &dumpinfo()
		{ return *_instance->_dumpinfo; }
	static scope_table &scopetable()
		{ return *_instance->_scopetable; }
	static symbol_table &symboltable()
		{ return *_instance->_symboltable; }
	static event_queue<stmt_base> &eventqueue()
		{ return *_instance->_eventqueue; }
	static time_wheel<stmt_base> &timewheel()
		{ return *_instance->_timewheel; }

	static void initialize();
	static void reset();

private:
	vbs_engine();
	~vbs_engine();

	// Object used by the VCD sub-system.  It is self contained and
	// does not need to be processed.  Let its constructor/destructor
	// handle the memory management.
	dump_info *_dumpinfo;

	// Object used by the VCD sub-system to keep track of the scope
	// of dumped variables.
	scope_table *_scopetable;

	// Table to hold a symbols in the simulation.
	symbol_table *_symboltable;

	// Object used by the simulation engine to handle monitor events.
	event_queue<stmt_base> *_eventqueue;

	// Object to handle simulation time advancement.
	time_wheel<stmt_base> *_timewheel;

	static vbs_engine *_instance;
	};

#endif /* _VBS_H */
