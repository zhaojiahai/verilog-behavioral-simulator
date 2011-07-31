// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// combase.h
//
// Class common to all objects, contains shared data.  Currently,
// this class is used to store the line number for error reporting.
// Thus any object that wish to report an error with line number,
// it has to inheret from this common_base class.  The since a module
// can not be coded across files, the filename is always the same for
// the same module, thus it does not need to be here.

// reg_decl will inherit from both tf_decl and module_item.  But we need
// <lineno> for both.  Since the lineno will be the same, we can
// try use a virtual base class.
//
// 95-9-16.  Since all classes have these informations, it was too
// wasteful to not to put in it a common object.

#ifndef _COMBASE_H
#define _COMBASE_H

#include <list>
#include <string>
#include <sstream>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

struct common_base
	{
	// Type used by many objects.
	template<typename T> struct list_type : public std::list<T> { };
	typedef std::string str_type;
	typedef std::stringstream strstream_type;
	typedef std::istream istream_type;
	typedef std::ostream ostream_type;

	common_base(long ln = -1)
		: _lineno(ln) {}
	common_base(const common_base &cm)
		: _lineno(cm._lineno) {}
	virtual ~common_base()
		{}

	long _lineno;
	};

#endif // _COMBASE_H
