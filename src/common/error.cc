// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// error.cc
//
// Modularized error messages.  See error.h on how to add more error
// detection.

#include "common/error.h"

// progname: file_name[line_number] -value- messages (arg)
// Example:
//
//	vbs: test.v[101] -4- syntax error, unexpected symbol (xxx)

vbs_error vbs_err;

vbs_error::str_type vbs_error::messages[] =
	{
	str_type("no errors or warnings"),

	// File system errors.
	str_type("unable to open file"),
	str_type("unable to close file"),
	str_type("unable to read from file"),
	str_type("unable to write into file"),
	str_type("unable to create file"),
	str_type("unable to delete file"),

	// Errors related to symbol table.
	str_type("symbol table not initialized"),
	str_type("symbol table is full"),
	str_type("duplicate symbol in symbol table"),
	str_type("undefined symbol"),

	// Paser errors.
	str_type("unexpected symbol"),
	str_type("unexpected keyword"),
	str_type("unexpected end of file"),

	// Errors related to task and function calls.
	str_type("not a task or system task"),
	str_type("not a function or system function"),
	str_type("system task/function not yet supported"),
	str_type("Wrong number of arguments"),
	str_type("Invalid argument(s)"),
	str_type("missing type specifier in format string"),
	str_type("unknown type specifier in format string"),
	str_type("invalid symbol in format string"),
	str_type("no termination in format string"),
	str_type("no format string specified"),
	str_type("ports and declarations do not match"),
	str_type("invalid io declaration"),
	str_type("invalid wire declaration"),
	str_type("invalid register declaration"),
	str_type("invalid integer declaration"),
	str_type("invalid memory declaration"),
	str_type("invalid parameter declaration"),
	str_type("recursive instantiation of module"),

	// General Errors...
	str_type("unsupported construct"),
	str_type("internal simulator error"),
	str_type("no top-level module found:\n"
			 "\tA top-level module is a module that is not instantiated by\n"
			 "\tany other module.  Simulation requires a top-level module\n"
			 "\tas a driver to initiate test vectors"),
	str_type("invalid range"),
	str_type("order of indices are wrong"),
	str_type("only one default case is allowed"),
	str_type("invalid type for this usage"),
	str_type("not an lvalue"),
	str_type("repeated operation"),
	str_type("error in object file"),
	str_type("error in executing program")
	};

vbs_error::vbs_error()
	: _state(SS_NONE)
	{}

void
vbs_error::out(strstream_type &mesg)
	{
	str_type s;
	s = mesg.str();
	out(s);
	}

void
vbs_error::out(const str_type &mesg)
	{
	extern void vbs_warn(int, const str_type &, const char *, const str_type &, int ln, const str_type &, const str_type &);
	extern void vbs_fatal(int, const str_type &, const char *, const str_type &, int ln, const str_type &, const str_type &);

	const char *state;
	switch (_state)
		{
		case SS_SIMULATE: state = "simulation"; break;
		case SS_SETUP: state = "setup"; break;
		case SS_COMPILE: state = "compilation"; break;
		case SS_PREPROCESS: state = "preprocess"; break;
		default: state = "unknown"; break;
		}

	// Should never return.
	if ((_state & SS_PREPROCESS) || (_state & SS_COMPILE))
		vbs_warn(_value, messages[_value], state, _file_name.top(), _line_number, mesg, _program_name);
	else
		vbs_fatal(_value, messages[_value], state, _file_name.top(), _line_number, mesg, _program_name);
	}

const vbs_error::str_type &
vbs_error::get_program_name() const
	{ return _program_name; }

void
vbs_error::set_program_name(const str_type &pn)
	{ _program_name = pn; }

void
vbs_error::push_filename(const str_type &fn)
	{ _file_name.push(fn); }

void
vbs_error::pop_filename(void)
	{ _file_name.pop(); }

void
vbs_error::set_data(value_type num, size_type ln)
	{
	_value = num;
	_line_number = ln;
	}

void
vbs_error::set_state(state_type s)
	{ _state = (state_type) (_state | s); }

void
vbs_error::clear_state(state_type s)
	{ _state = (state_type) (_state & ~(s)); }

bool
vbs_error::test_state(state_type s)
	{ return (_state & s); }
