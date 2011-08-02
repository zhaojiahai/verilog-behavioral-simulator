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
// dump_vcd.h

#ifndef _DUMPVCD_H
#define _DUMPVCD_H

#include <map>
#include "common/st_net.h"
#include "common/dumpinfo.h"

class dump_vcd : public dump_base
	{
public:
	typedef net_type::num_type num_type;
	typedef std::map<net_type *, str_type, std::less<net_type*> > hashtable_type;

	dump_vcd(dump_stream &);
	~dump_vcd();

	void create_header();
	void dump_values();
	void dump_all();
	void dump_on();
	void dump_off();

	void pre_second_pass();
	void second_pass(ident_type *, int, net_list *);
	void post_second_pass();

private:
	str_type file_name;
	str_type _get_next_name_string; // Stores possible chars.
	str_type _get_next_name; // Return value.
	int _get_next_name_index;
	int _get_next_name_chars;
	int _get_next_name_end;
	int _current_top_level;
	net_list n_lst; // List of all nets.
	hashtable_type name_table; // Returns the st_net * for a mangled name.

	str_type get_next_name();
	};

#endif // _DUMPVCD_H
