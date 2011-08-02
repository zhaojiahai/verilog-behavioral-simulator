// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mitfbase.h

#ifndef _MITFBASE_H
#define _MITFBASE_H

#include "common/combase.h"

// Module items.
class initial;
class always;
class cont_assign;
class function;
class task;
class module_instantiation;

// Declarations.
class io_decl;
class net_decl;
class reg_decl;
class int_decl;
class param_decl;

struct mitf_setup;
//struct mitf_read; // See below.
struct mitf_write;

struct mitf_base : public virtual common_base
	{
	typedef mitf_setup setup_type;
	// Lastest draft ANSI C++ standard supports changing from returning
	// base to returning derived.  But gcc doesn't support that yet.
	//typedef mitf_read read_type;
	typedef mitf_write write_type;

	virtual ~mitf_base()
		{}

	//static mitf_base *read(const read_type &)
	//	{ return 0; }
	virtual void write(const write_type &) const
		{}
	virtual ostream_type &display(ostream_type &s) const
		{ return s; }
	virtual void setup(const setup_type &)
		{}
	};

inline mitf_base::ostream_type &
operator<<(mitf_base::ostream_type &s, const mitf_base &d)
	{ return d.display(s); }

struct mitf_setup
	{
	virtual ~mitf_setup() {}
	virtual void operator()(initial *) const {}
	virtual void operator()(always *) const {}
	virtual void operator()(cont_assign *) const {}
	virtual void operator()(function *) const {}
	virtual void operator()(task *) const {}
	virtual void operator()(module_instantiation *) const {}
	// Declarations.
	virtual void operator()(io_decl *) const {}
	virtual void operator()(net_decl *) const {}
	virtual void operator()(reg_decl *) const {}
	virtual void operator()(int_decl *) const {}
	virtual void operator()(param_decl *) const {}
	};

//struct mitf_read
//	{
//	virtual mitf_base *operator()() const { return 0; }
//	};

struct mitf_write
	{
	virtual ~mitf_write() {}
	virtual void operator()(const initial *) const {}
	virtual void operator()(const always *) const {}
	virtual void operator()(const cont_assign *) const {}
	virtual void operator()(const function *) const {}
	virtual void operator()(const task *) const {}
	virtual void operator()(const module_instantiation *) const {}
	// Declarations.
	virtual void operator()(const io_decl *) const {}
	virtual void operator()(const net_decl *) const {}
	virtual void operator()(const reg_decl *) const {}
	virtual void operator()(const int_decl *) const {}
	virtual void operator()(const param_decl *) const {}
	};

#endif // _MITFBASE_H
