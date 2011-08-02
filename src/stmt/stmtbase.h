// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// stmtbase.h

#ifndef _STMTBASE_H
#define _STMTBASE_H

#include "common/combase.h"
#include "expr/exprbase.h"
#include "misc/decbase.h"

class null_stmt;
class seq_block_stmt;
class systask_finish;
class systask_display;
class systask_write;
class systask_monitor;
class systask_readmemb;
class systask_readmemh;
class systask_strobe;
class systask_dumpon;
class systask_dumpoff;
class systask_dumpall;
class systask_dumpvars;
class systask_dumpfile;
class sysfunc_time;
class sysfunc_stime;
class sysfunc_random;
class task_enable_stmt;
class assignment_stmt;
class if_else_stmt;
class case_stmt;
class loop_stmt;

struct stmt_setup
	{
	virtual ~stmt_setup() {}
	virtual void operator()(null_stmt *) const {}
	virtual void operator()(seq_block_stmt *) const {}
	virtual void operator()(systask_finish *) const {}
	virtual void operator()(systask_display *) const {}
	virtual void operator()(systask_write *) const {}
	virtual void operator()(systask_monitor *) const {}
	virtual void operator()(systask_readmemb *) const {}
	virtual void operator()(systask_readmemh *) const {}
	virtual void operator()(systask_strobe *) const {}
	virtual void operator()(systask_dumpon *) const {};
	virtual void operator()(systask_dumpoff *) const {};
	virtual void operator()(systask_dumpall *) const {};
	virtual void operator()(systask_dumpvars *) const {};
	virtual void operator()(systask_dumpfile *) const {}; 
	virtual void operator()(sysfunc_time *) const {}
	virtual void operator()(sysfunc_stime *) const {}
	virtual void operator()(sysfunc_random *) const {}
	virtual void operator()(task_enable_stmt *) const {}
	virtual void operator()(assignment_stmt *) const {}
	virtual void operator()(if_else_stmt *) const {}
	virtual void operator()(case_stmt *) const {}
	virtual void operator()(loop_stmt *) const {}
	};

struct stmt_trigger
	{
	virtual ~stmt_trigger() {}
	virtual bool operator()(null_stmt *) const { return true; }
	virtual bool operator()(seq_block_stmt *) const { return true; }
	virtual bool operator()(systask_finish *) const { return true; }
	virtual bool operator()(systask_display *) const { return true; }
	virtual bool operator()(systask_write *) const { return true; }
	virtual bool operator()(systask_monitor *) const { return true; }
	virtual bool operator()(systask_readmemb *) const { return true; }
	virtual bool operator()(systask_readmemh *) const { return true; }
	virtual bool operator()(systask_strobe *) const { return true; }
	virtual bool operator()(systask_dumpon *) const {return true ;};
	virtual bool operator()(systask_dumpoff *) const {return true ;};
	virtual bool operator()(systask_dumpall *) const {return true ;};
	virtual bool operator()(systask_dumpvars *) const {return true ;};
	virtual bool operator()(systask_dumpfile *) const {return true ;}; 
	virtual bool operator()(sysfunc_time *) const { return true; }
	virtual bool operator()(sysfunc_stime *) const { return true; }
	virtual bool operator()(sysfunc_random *) const { return true; }
	virtual bool operator()(task_enable_stmt *) const { return true; }
	virtual bool operator()(assignment_stmt *) const { return true; }
	virtual bool operator()(if_else_stmt *) const { return true; }
	virtual bool operator()(case_stmt *) const { return true; }
	virtual bool operator()(loop_stmt *) const { return true; }
	};

struct stmt_write
	{
	virtual ~stmt_write() {}
	virtual void operator()(const null_stmt *) const {}
	virtual void operator()(const seq_block_stmt *) const {}
	virtual void operator()(const task_enable_stmt *) const {}
	virtual void operator()(const assignment_stmt *) const {}
	virtual void operator()(const if_else_stmt *) const {}
	virtual void operator()(const case_stmt *) const {}
	virtual void operator()(const loop_stmt *) const {}
	};

struct stmt_base : public virtual common_base
	{
	typedef expr_base expr_type; // Used by event objects.
	typedef dec_base dec_type;

	typedef stmt_setup setup_type;
	typedef stmt_trigger trigger_type;
	typedef stmt_write write_type;

	stmt_base(dec_type *d = 0)
		: _always(false)
		{ _ec = 0; _dec = 0; }
	stmt_base(const stmt_base &s)
		: common_base(s), _always(s._always)
		{
		_ec = (s._ec != 0) ? s._ec->copy_constructor() : 0;
		_dec = (s._dec != 0) ? s._dec->copy_constructor() : 0;
		}
	virtual ~stmt_base()
		{ delete _ec; delete _dec; }

	// get_task_enable is used for triggering, where the argument is
	// needed.  This should be changed later to dynamic_cast when
	// compilers have better support for it.
	virtual task_enable_stmt *get_task_enable()
		{ return 0; }
	// get_seq_block is used for triggering nested statements.  This
	// should be changed later to dynamic_cast when compilers have
	// better support for it.
	virtual seq_block_stmt *get_seq_block()
		{ return 0; }
	// get_loop is used for processing nested statements.  This
	// should be changed later to dynamic_cast when compilers have
	// better support for it.
	virtual loop_stmt *get_loop()
		{ return 0; }

	virtual stmt_base *copy_constructor() const
		{ return new stmt_base(*this); }
	virtual ostream_type &display(ostream_type &s) const
		{
		if (_ec != 0) s << *_ec << endl;
		if (_dec != 0) s << *_dec;
		return s;
		}
	virtual void write(const write_type &) const
		{}
	virtual void setup(const setup_type &)
		{}
	virtual bool trigger(const trigger_type &)
		{ return true; }

	bool _always; // Part of an always procedural block.
	dec_type *_ec; // Optional event control.
	dec_type *_dec; // Optional delay or event control.
	};

class null_stmt : public stmt_base
	{
public:
	null_stmt()
		: stmt_base() {}
	null_stmt(const null_stmt &ns)
		: stmt_base(ns) {}

	virtual null_stmt *copy_constructor() const
		{ return new null_stmt(*this); }
	ostream_type &display(ostream_type &s) const
		{
		stmt_base::display(s); // Delay or event control...
		s << " ;" << endl;
		return s;
		}
	void write(const write_type &wr) const
		{ wr(this); }
	void setup(const setup_type &setup)
		{ setup(this); }
	bool trigger(const trigger_type &trigger)
		{ return trigger(this); }
	};

inline stmt_base::ostream_type &
operator<<(stmt_base::ostream_type &s, const stmt_base &d)
	{ return d.display(s); }

#endif // _STMTBASE_H
