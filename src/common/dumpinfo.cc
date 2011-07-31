// Verilog Behavioral Simulator
// Copyright (C) 2000-2001 George Varughese
//
// With modifications from:
//  Jimen Ching <jching@flex.com> 
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dumpinfo.cc

#include "common/time_whl.h"
#include "common/dumpinfo.h"
#include "common/dumpvcd.h"
#include "common/dumpasc.h"
#include "common/dumpstrm.h"
#include "stmt/stmtbase.h"
#include "sim.h" // sim_run_status()

extern time_wheel<stmt_base> timewheel;

dump_info::dump_info()
	{
	_dump_status = false;
	_dump_base = 0;
	_dump_out = new dump_stream_nc; // default
	_dump_file = "*invalid*"; // default
	}

dump_info::~dump_info()
	{
	if (_dump_base != 0)
		delete _dump_base;

	if (_dump_out != 0)
		{
		_dump_out->close();
		delete _dump_out;
		}
	}

void
dump_info::dump_format(const str_type &df)
	{
	if (_dump_base != 0)
		delete _dump_base;

	if (df == "ascii")
		{
		_dump_base = new dump_ascii(*_dump_out);
		_dump_status = true;
		}
	else if (df == "vcd")
		{
		_dump_base = new dump_vcd(*_dump_out);
		_dump_status = true;
		}
	else
		{
		_dump_base = 0;
		_dump_status = false;
		}
	}

void
dump_info::dump_compress(int lvl)
	{
	if (_dump_out != 0)
		delete _dump_out;

	if (lvl > 0)
		_dump_out = new dump_stream_c(lvl);
	else
		_dump_out = new dump_stream_nc;
	}

void
dump_info::dump_file(const str_type &n)
	{
	// If format has not been specified, default to VCD.
	if (_dump_base == 0)
		dump_format("vcd");

	// Open file for output.
	const str_type &unqouted(n.substr(1, n.size() - 2));
	if (_dump_out->compress())
		sim_run_status(3, (unqouted + ".gz").c_str());
	else
		sim_run_status(3, unqouted.c_str());
	_dump_out->open(unqouted.c_str());
	_dump_file = unqouted.c_str();
	// Output header.
	_dump_base->create_header();
	}

void
dump_info::dump_vars(const str_type &s)
	{ _dump_vars.insert(_dump_vars.end(),s); }

void
dump_info::dump_vars_all()
	{ _dump_vars.insert(_dump_vars.end(),top_level()); }

dump_info::str_list *
dump_info::dump_vars()
	{ return &_dump_vars; }

dump_info::str_type
dump_info::first_dump_vars() 
	{
	curr = _dump_vars.begin();
	last = _dump_vars.end();
	if (curr != last)
		return (*curr);
	else
		return "";
	}

dump_info::str_type
dump_info::next_dump_vars()
	{
	++curr;
	if (curr != last)
		return (*curr);
	return "";
	}

void
dump_info::levels(const int l) 
	{ _levels = l; }

dump_info::str_type 
dump_info::top_level()
	{ return _top_level; }

void 
dump_info::top_level(const str_type &t ) 
	{ _top_level = t; }

int
dump_info::levels() 
	{ return _levels; }

bool
dump_info::dump_status()
	{ return _dump_status; }

bool
dump_info::dump_status(bool b)
	{ return (_dump_status = b); }

void
dump_info::dump_on(bool b)
	{ 
	_dump_on = b;
	if (timewheel.current_time() != 0)
		{
		if (_dump_base != 0)
			{
			if (_dump_on)
				_dump_base->dump_on();
			else
				_dump_base->dump_off();
			}
		}
	}

bool
dump_info::dump_on()
	{ return _dump_on; }

void
dump_info::dump_all()
	{ 
	if (_dump_base != 0)
		_dump_base->dump_all();
	}

void
dump_info::dump_vars_scope(const str_type &n)
	{ _dump_vars_scope = n; }

dump_info::str_type
dump_info::dump_vars_scope()
	{ return _dump_vars_scope; }	

void
dump_info::dump_values()
	{
	if (_dump_base != 0)
		_dump_base->dump_values();
	}

void 
dump_info::pre_second_pass()
	{ 
	if (_dump_base != 0)
		{
		if (_dump_file.compare("*invalid*") == 0)
			dump_file("\"vbs.dmp\""); // No file specified, use default.
		_dump_base->pre_second_pass();
		}
	}

void
dump_info::second_pass(ident_type *rid, int lvls, net_list * n)
	{ 
	if (_dump_base != 0)
		_dump_base->second_pass(rid, lvls, n);
	}

void 
dump_info::post_second_pass()
	{
	if (_dump_base != 0)
		_dump_base->post_second_pass();
	}
