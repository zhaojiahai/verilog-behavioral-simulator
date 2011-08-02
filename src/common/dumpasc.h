// Verilog Behavioral Simulator
// Copyright (C) 2000-2001,2011 George Varughese
//
// With modifications from:
//  Jimen Ching <jimen.ching@gmail.com>
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dumpasc.h

#ifndef _DUMPASC_H
#define _DUMPASC_H

#include "common/st_net.h"
#include "common/dumpinfo.h"

class dump_ascii : public dump_base
	{
public:
	typedef list_type<str_type> str_list;
	typedef net_type::num_type num_type;
	typedef net_type::position_type position_type;

	dump_ascii(dump_stream &);
	virtual ~dump_ascii();

	void create_header();
	void dump_values();
	void dump_all();
	void dump_on();
	void dump_off();

	void pre_second_pass();
	void second_pass(ident_type *, int, net_list *);
	void post_second_pass();

private:
	int _dump_scope;
	int _current_top_level;
	net_list *_dump_list_ptr;
	net_list n_lst;
	};

#endif // _DUMPASC_H
