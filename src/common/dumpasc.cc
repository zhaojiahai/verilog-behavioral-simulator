// Verilog Behavioral Simulator
// Copyright (C) 2000-2001 George Varughese
//
// With modifications from:
// Jimen Ching <jching@flex.com>
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dumpasc.cc

#include <cstdio> // For sprintf.

#include "common/dstack.h"
#include "common/scp_tab.h"
#include "common/st_net.h"
#include "common/st_eval.h"
#include "common/dumpstrm.h"
#include "common/dumpasc.h"
#include "expr/rangeid.h"
#include "expr/eeval.h"

//
// FIXME:  needs complate rewrite for $dump setup...
//

extern dump_info dumpinfo;
extern scope_table scopetable;

dump_ascii::dump_ascii(dump_stream &d)
	: dump_base(d)
	{
	_dump_list_ptr = new net_list;
	}

dump_ascii::~dump_ascii()
	{
	}

void
dump_ascii::second_pass(ident_type *rid, int lvls, net_list *n)
	{
	_dump_list_ptr->insert(_dump_list_ptr->end(),n->begin(),n->end());
	}

void
dump_ascii::pre_second_pass()
	{ }

void
dump_ascii::post_second_pass()
	{
	Stack<int> sc;
	int name_length = 0, tmp_length = 0;
	strstream_type str_out;
	str_type tmp_string, scoped_name("");
	str_list dump_header;
	net_list::iterator start(_dump_list_ptr->begin());
	net_list::iterator end(_dump_list_ptr->end());
	for(; start != end; ++start)
		{
		scoped_name = scopetable.top_level();
		scoped_name += scopetable.find((*start)->scope());
		if (scoped_name.length() > 0)
			scoped_name += ".";
		scoped_name += (*start)->name();
		tmp_length = scoped_name.length();
		if ((*start)->size() > 1)
			{
			int ms_no, ls_no;
			char index_str[100];
			sprintf(index_str,"%ld", (*start)->port_msb());
			ms_no = strlen(index_str);
			sprintf(index_str,"%ld", (*start)->port_lsb());
			ls_no = strlen(index_str);
			if (ms_no > ls_no)
				tmp_length = tmp_length + 2 + ms_no; // [10]
			else
				tmp_length = tmp_length + 2 + ls_no;
			}
		if (tmp_length > name_length)
			name_length = tmp_length;
		}

	// Convert "a" to "a[10]" format, bit_blasting.
	start = _dump_list_ptr->begin();
	for(; start != end; ++start)
		{
		scoped_name = "top";
		if ((*start)->scope() != 1)
			{
			scoped_name += ".";
			scoped_name += scopetable.find((*start)->scope());
			}
		if (scoped_name.length() > 0)
			scoped_name += ".";
		scoped_name += (*start)->name();
		tmp_length = name_length - scoped_name.length();
		tmp_string = ""; // empty
		if ((*start)->size() > 1)
			{
			position_type m =(*start)->port_msb();
			position_type l =(*start)->port_lsb();
			char tmp_num_str[200];
			for (position_type i = m; i >= l; --i)
				{
				sprintf(tmp_num_str,"%ld",i);
				int bits = strlen(tmp_num_str);
				int new_tmp_length = tmp_length -2 - bits;
				str_type string_to_insert(scoped_name + "[" + tmp_num_str + "]");
				if (new_tmp_length > 0)
					{
					while (new_tmp_length--)
				 		string_to_insert = " " + string_to_insert;
					}
				dump_header.insert(dump_header.end(),string_to_insert);
				}
			}
		else
			{
			if (tmp_length > 0)
				{
				while (tmp_length--)
					tmp_string += " ";
				}
			dump_header.insert(dump_header.end(), tmp_string + scoped_name);
			}
		}

	str_list::iterator s = dump_header.begin();
	str_list::iterator e = dump_header.end();
	for (int i = 0; i < name_length; ++i)
		{
		s = dump_header.begin();
		str_out << "! ";
		for(; s != e; ++s)
			str_out << (*s)[i];
		str_out << endl;
		}
	str_out << endl;
	_dump_out << str_out;
	}

void
dump_ascii::create_header()
	{
	// From current_scope find all net and add to _dump_list_ptr.
	}

void
dump_ascii::dump_values() 
	{
	strstream_type str_out;
	position_type msb, lsb;

	str_out << "  ";
	net_list::iterator start = _dump_list_ptr->begin();
	net_list::iterator end = _dump_list_ptr->end();
	for(; start != end; ++start)
		{
		const num_type &val((*start)->evaluate(evaluate_st_node()));
		msb = (*start)->port_msb() ; // stop_pos end msb
		lsb = (*start)->port_lsb() ; // start_pos begin lsb
		if ((*start)->size() > 1 )
			{
			for( position_type i = msb; i >= lsb; --i) 
				str_out << char(val(i)); // bit_blast
			}
		else 
			str_out << bool(val);
		}
	str_out << endl;
	_dump_out << str_out;
	}

void
dump_ascii::dump_all() 
	{}

void
dump_ascii::dump_on() 
	{}

void
dump_ascii::dump_off() 
	{}
