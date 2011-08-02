// Verilog Behavioral Simulator
// Copyright (C) 2000-2001 George Varughese
//
// With modifications from:
//  Jimen Ching <jimen.ching@gmail.com>
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dumpinfo.h

#ifndef _DUMPINFO_H
#define _DUMPINFO_H

#include <string>
#include <sstream>
#include <list>

class st_net;
class range_id;
class dump_stream;

class dump_base
	{
public:
	template<typename T> struct list_type : public std::list<T> { };
	typedef std::string str_type;
	typedef std::stringstream strstream_type;
	typedef st_net net_type;
	// Require std::list to be compatible with scope_table.
	typedef std::list<net_type*> net_list;
	typedef range_id ident_type;

	dump_base(dump_stream &d)
		: _dump_out(d)
		{}
	virtual ~dump_base()
		{}

	virtual void create_header() = 0;
	virtual void dump_values() = 0;
	virtual void dump_all() = 0;
	virtual void dump_on() = 0;
	virtual void dump_off() = 0;

	virtual void pre_second_pass() = 0;
	virtual void second_pass(ident_type *, int, net_list *) = 0;
	virtual void post_second_pass() = 0;

protected:
	dump_stream &_dump_out;
	};

class dump_info 
	{
public:
	typedef dump_base::str_type str_type;
	typedef dump_base::list_type<str_type> str_list;
	typedef dump_base::net_list net_list;
	typedef dump_base::ident_type ident_type;

	dump_info();
	~dump_info();

	void dump_format(const str_type &);
	void dump_compress(int lvl);
	void dump_file(const str_type &);
	void dump_vars(const str_type &);
	void dump_vars_all();
	str_list *dump_vars();
	str_type first_dump_vars(); // restart
	str_type next_dump_vars();

	void levels(const int);
	int levels();
	str_type top_level(); // return the top level 
	void top_level(const str_type & ) ;

	bool dump_status();
	bool dump_status(bool);
	void dump_all();
	bool dump_on();
	void dump_on(bool);
	void dump_vars_scope(const str_type &);
	str_type dump_vars_scope();
	void dump_values();

	void pre_second_pass();
	void second_pass(ident_type *rid, int lvls, net_list *);
	void post_second_pass();

private:
	str_type _dump_vars_scope;
	str_type _dump_file;
	int _levels;
	str_type _top_level;
	str_list _dump_vars;
	bool _dump_status;
	bool _dump_on;
	int _dump_levels;
	dump_base *_dump_base;
	dump_stream *_dump_out;

	// Working variables.
	str_list::iterator curr;
	str_list::iterator last;
	};

#endif // _DUMPINFO_H
