// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// module.h

#ifndef _MODULE_H
#define _MODULE_H

#include "common/ptr.h"
#include "common/combase.h"
#include "misc/port.h"
#include "moditm/mibase.h"

struct module_setup;
struct module_read;
struct module_write;

class module : public virtual common_base
	{
	friend struct setup_module;
	friend struct read_module;
	friend struct write_module;

public:
	typedef port port_type;
	typedef basic_ptr<port_type> port_ptr;
	typedef list_type<port_ptr> port_list;
	typedef module_item_base module_item_type;
	typedef basic_ptr<module_item_type> module_item_ptr;
	typedef list_type<module_item_ptr> module_item_list;

	typedef module_setup setup_type;
	typedef module_read read_type;
	typedef module_write write_type;

	module(const str_type &, module_item_list *, port_list * = 0);
	~module();

	static module *read(const read_type &);
	void write(const write_type &) const;
	ostream_type &display(ostream_type &) const;
	const str_type &name() const;
	port_list *port_connection() const;
	void port_connection(port_list *);
	module_item_list *module_item() const;
	void module_item(module_item_list *);
	void setup(const setup_type &);

public:
	str_type _filename;

private:
	str_type _name;
	port_list *_port_list;
	module_item_list *_module_item;
	};

module::ostream_type &operator<<(module::ostream_type &s, const module &);

struct module_setup
	{
	virtual void operator()(module *) const {}
	};

struct module_read
	{
	virtual module *operator()() const { return 0; }
	};

struct module_write
	{
	virtual void operator()(const module *) const {}
	};

#endif // _MODULE_H
