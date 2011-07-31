// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2000,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// debug.cc
//
// The content of this file are private to this file.  Only the
// macros in the header know about the objects here.  The macros are
// defined at the bottom.

#ifdef VBS_DEBUG

#include "debug.h"

using namespace std;

debug::debug()
	: _enabled(DEBUG_NONE), _state(DEBUG_NONE)
	{
	_output_file = 0;
	_debug_out = 0;
	}

debug::~debug()
	{
	delete _output_file;
	if (_debug_out != &cout)
		delete _debug_out;
	}


// Interface functions to our debug object

debug _debug;

void
debug_initialize(const char *fn)
	{
	if (fn != 0)
		{
		_debug._output_file = new ofstream;
		_debug._output_file->open(fn);
		_debug._debug_out = _debug._output_file;
		}
	else
		{
		_debug._debug_out = &cout;
		}
	}

#endif // VBS_DEBUG
