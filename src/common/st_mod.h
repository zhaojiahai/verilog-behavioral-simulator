// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_module.h
//
// Symbol table node to store module definition.

#ifndef _ST_MODULE_H
#define _ST_MODULE_H

#include "common/ptr.h"
#include "common/st_node.h"
#include "misc/port.h"
#include "moditm/mibase.h"

class st_module : public st_node_base
	{
	friend struct setup_st_node;
	friend struct setup_module;

public:
	typedef list_type<hash_type> io_list;
	typedef port port_type;
	typedef basic_ptr<port_type> port_ptr;
	typedef list_type<port_ptr> port_list;
	typedef module_item_base module_item_type;
	typedef basic_ptr<module_item_type> module_item_ptr;
	typedef list_type<module_item_ptr> module_item_list;

	st_module(const str_type &);
	virtual ~st_module();

	const bool is_top_level() const;
	void is_top_level(bool);
	st_module *get_module();
	module_item_list *get_module_item();

	ostream_type &display(ostream_type &s) const;
	void setup(const setup_type &);

private:
	bool _is_top_level;
	port_list *_port_list;
	module_item_list *_module_item;

public:
	str_type _filename;
	};

#endif // _ST_MODULE_H
