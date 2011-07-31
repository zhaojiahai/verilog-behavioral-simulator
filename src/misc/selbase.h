// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// selbase.h

#ifndef _SELBASE_H
#define _SELBASE_H

#include "common/combase.h"

class bit_select;
class part_select;
struct select_base;

struct select_monitor
	{
	virtual bool operator()(const bit_select *) const { return false; }
	virtual bool operator()(const part_select *) const { return false; }
	};

struct select_setup
	{
	virtual void operator()(bit_select *) const {}
	virtual void operator()(part_select *) const {}
	};

struct select_write
	{
	virtual void operator()(const bit_select *) const {}
	virtual void operator()(const part_select *) const {}
	};

struct select_read
	{
	virtual select_base *operator()() const { return 0; }
	};

struct select_base : public virtual common_base
	{
	enum SELECT_SIZE { SELECT_BIT, SELECT_PART, SELECT_NONE };

	typedef signed long position_type;
	typedef SELECT_SIZE select_size_type;

	typedef select_monitor monitor_type;
	typedef select_setup setup_type;
	typedef select_write write_type;

	select_base()
		{}
	select_base(const select_base &s)
		: common_base(s) {}
	virtual ~select_base()
		{}

	virtual select_size_type get_data(position_type &ms, position_type &ls)
		const { ms = -1; ls = -1; return SELECT_NONE; }
	virtual select_base *copy_constructor() const
		{ return new select_base(*this); }
	virtual ostream_type &display(ostream_type &s) const
		{ return s; }
	static select_base *read(const select_read &rd)
		{ return rd(); }
	virtual void write(const write_type &) const
		{}
	virtual bool monitor(const monitor_type &) const
		{ return false; }
	virtual void setup(const setup_type &)
		{}
	};

inline select_base::ostream_type &
operator<<(select_base::ostream_type &s, const select_base &sb)
	{ return sb.display(s); }

#endif // _SELBASE_H
