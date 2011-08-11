// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// error.h
//
// This is the standard error reporting mechanism for VBS.  Any error
// conditions that vbs detects will be enumerated here as a macro.  If
// the error is not here, it's most likely not detected.  In the future,
// I hope to make this object streamable.
//
// To add a new error to detect, include a message in messages (error.cc),
// and add a constant to value_type.

#ifndef _ERROR_H
#define _ERROR_H

#include <string>
#include <sstream>
#include "common/dstack.h"

struct vbs_error
	{
	typedef int size_type;
	typedef std::string str_type;
	typedef std::stringstream strstream_type;

	static str_type messages[];

	enum state_type
		{
		SS_NONE = 0x00,

		SS_PREPROCESS_ONLY = 0x01,
		SS_COMPILE_ONLY = 0x02,
		SS_READOBJ_ONLY = 0x04,
		SS_MODE_MASK = 0x0f,

		SS_PREPROCESS = 0x10,
		SS_COMPILE = 0x20,
		SS_SETUP = 0x40,
		SS_SIMULATE = 0x80,
		SS_STATE_MASK = 0xf0
		};

	enum value_type
		{
		SE_NONE, // No errors or warnings.

		// System errors.
		SE_FILEOPEN, // Unable to open file.
		SE_FILECLOSE, // Unable to close file.
		SE_FILEREAD, // Unable to read from file.
		SE_FILEWRITE, // Unable to write into file.
		SE_FILECREATE, // Unable to create file.
		SE_FILEDELETE, // Unable to delete file.

		// Errors related to symbol table.
		SE_STINIT, // Symbol table not initialized (if changed, change
	           // hash_value too).
		SE_STFULL, // Symbol table is full.
		SE_STDUP, // Duplicate symbol in symbol table.
		SE_STDEF, // Symbol not found.

		// Parser errors.
		SE_PSYMBOL, // Unexpected symbol.
		SE_PKEYWORD, // Unexpected keyword.
		SE_PEOF, // Unexpected end of file.

		// Errors related to task/function calls and module instantiations.
		SE_NTASK, // Not a task or system task.
		SE_NFUNCTION, // Not a function or system function.
		SE_TFSUPPORT, // System task/function not yet supported.
		SE_NARGLST, // Wrong number of arguments.
		SE_INVARG, // Invalid argument(s).
		SE_FMTSPEC, // Missing type specifier in format string.
		SE_FMTUNKN, // Unknown type specifier in format string.
		SE_FMTSYM, // Invalid symbol in format string.
		SE_FMTNOTERM, // No termination in format string.
		SE_NOFMT, // No format string specified.
		SE_PORT, // Ports and declarations do not match.
		SE_IODECL, // Invalid io declaration.
		SE_WIREDECL, // Invalid wire declaration.
		SE_REGDECL, // Invalid register declaration.
		SE_INTDECL, // Invalid integer declaration.
		SE_TIMEDECL, // Invalid time declaration.
		SE_MEMDECL, // Invalid memory declaration.
		SE_PARAMDECL, // Invalid parameter declaration.
		SE_RECURSE, // Recursive instantiation of module.

		// General Errors...
		SE_SUPPORT, // Unsupported construct.
		SE_INTERNAL, // Internal simulator error.
		SE_NTOPLVL, // No top-level module found.
		SE_RANGE, // Invalid Range.
		SE_IDXORDER, // Order of indices are wrong.
		SE_MULTDEFLT, // Only one default case is allowed.
		SE_TYPE, // Invalid type for this usage.
		SE_NLVAL, // Not an lvalue.
		SE_REPEAT, // Repeated operation.
		SE_VBSOBJ, // Error in VBS object file.
		SE_PROGRAM // Error in executing program.
		};

	str_type _program_name; // Our name to the system.
	Stack<str_type> _file_name; // File name with error.
	size_type _line_number; // Line number of error.
	value_type _value; // Error value.
	state_type _state; // State of simulator.

	vbs_error();

	const str_type &get_program_name() const;
	void set_program_name(const str_type &);
	void push_filename(const str_type &);
	void pop_filename(void);
	value_type get_data();
	void set_data(value_type, size_type);
	void set_state(state_type);
	void clear_state(state_type);
	bool test_state(state_type);
	void out(const str_type &);
	void out(strstream_type &);
	};

extern vbs_error vbs_err;

#endif // _ERROR_H
