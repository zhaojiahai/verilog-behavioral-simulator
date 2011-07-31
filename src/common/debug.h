// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2000,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// debug.h
//
// This is the standard debugging system for VBS.  See the documentation
// for the description of how to use this system.  These are the debugging
// levels we will want to support.
//
// Set the following debug flags
//   DEBUG_SIM_STATE ...... Print state of simulator.
//   DEBUG_PARSER ......... Print creation of each object.
//                          Notify when a module is parsed.
//   DEBUG_SYMBOL_TABLE ... Print symbol table contents.
//   DEBUG_TIME_WHEEL ..... Print output on each time node.
//                          'initial' statement is appended to time wheel.
//                          Time at which statement is appended to future.
//   DEBUG_USERX .......... Printout sequence information of nested blocks
//	                       (useful for debugging nested blocks).

#ifndef _DEBUG_H
#define _DEBUG_H

#include <iostream>
#include <fstream>

struct debug
	{
	typedef std::ostream ostream_type;
	typedef std::ofstream ofstream_type;

	enum debug_level_type
		{
		NONE			= 0x00000000,
		SIM_STATE		= 0x00000001,
		PARSER			= 0x00000002,
		SYMBOL_TABLE	= 0x00000004,
		TIME_WHEEL		= 0x00000008,
		EVENT			= 0x00000010,
		USERX			= 0x00000020,
		// Temporary usage.
		USER1			= 0x10000000,
		USER2			= 0x20000000,
		USER3			= 0x40000000,
		USER4			= 0x80000000,
		ALL				= 0xffffffff
		};

	debug();
	~debug();

	bool output_ok() const
		{ return (_state & _enabled ? 1 : 0); }
	void set_level(debug_level_type dl)
		{ _enabled = debug_level_type(_enabled | dl); }
	void unset_level(debug_level_type dl)
		{ _enabled = debug_level_type(_enabled & ~dl); }
	void set_state(debug_level_type dl)
		{ _state = dl; }

	debug_level_type _enabled;
	debug_level_type _state;
	ofstream_type *_output_file;
	ostream_type *_debug_out;
	};

#define DEBUG_NONE			debug::NONE
#define DEBUG_SIM_STATE		debug::SIM_STATE
#define DEBUG_PARSER		debug::PARSER
#define DEBUG_SYMBOL_TABLE	debug::SYMBOL_TABLE
#define DEBUG_TIME_WHEEL	debug::TIME_WHEEL
#define DEBUG_EVENT			debug::EVENT
#define DEBUG_USERX			debug::USERX
#define DEBUG_USER1			debug::USER1
#define DEBUG_USER2			debug::USER2
#define DEBUG_USER3			debug::USER3
#define DEBUG_USER4			debug::USER4
#define DEBUG_ALL			debug::ALL

#ifdef VBS_DEBUG

void debug_initialize(const char *);
inline void
debug_enable(debug::debug_level_type dl)
	{ extern debug _debug; _debug.set_level(dl); }
inline void
debug_disable(debug::debug_level_type dl)
	{ extern debug _debug; _debug.unset_level(dl); }
inline void
debug_state(debug::debug_level_type dl)
	{ extern debug _debug; _debug.set_state(dl); }

template<class T> inline void
debug_output(const T &arg)
	{
	extern debug _debug;
	if (_debug.output_ok())
		*(_debug._debug_out) << arg;
	}

template<class T> inline void
debug_foutput(const T &func)
	{
	extern debug _debug;
	if (_debug.output_ok())
		func(*(_debug._debug_out));
	}

#define DEBUG_INITIALIZE(fn)	debug_initialize(fn)
#define DEBUG_ENABLE(dl)		debug_enable(dl)
#define DEBUG_DISABLE(dl)		debug_disable(dl)
#define DEBUG_STATE(dl)			debug_state(dl)
#define DEBUG_OUTPUT(t)			debug_output(t)
#define DEBUG_FOUTPUT(t)		debug_foutput(t)

#else

#define DEBUG_INITIALIZE(fn)
#define DEBUG_ENABLE(dl)
#define DEBUG_DISABLE(dl)
#define DEBUG_STATE(dl)
#define DEBUG_OUTPUT(t)
#define DEBUG_FOUTPUT(t)

#endif // VBS_DEBUG

#endif // _DEBUG_H
