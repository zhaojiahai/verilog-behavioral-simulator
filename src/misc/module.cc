// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// module.cc

#include "misc/module.h"

module::module(const str_type &n, module_item_list *m, port_list *p)
	: _name(n)
	{
	_port_list = p;
	_module_item = m;
	}

module::~module()
	{
	delete _port_list;
	delete _module_item;
	}

const module::str_type &
module::name() const
	{ return _name; }

module::port_list *
module::port_connection() const
	{ return _port_list; }

void
module::port_connection(port_list *p)
	{ _port_list = p; }

module::module_item_list *
module::module_item() const
	{ return _module_item; }

void
module::module_item(module_item_list *m)
	{ _module_item = m; }

module::ostream_type &
module::display(ostream_type &s) const
	{
	s << "module " << _name;
	if (_port_list != 0)
		{
		port_list::iterator itp(_port_list->begin());
		port_list::iterator stop(_port_list->end());
		cout << '(' << (*itp)->name();
		for (++itp; itp != stop; ++itp)
			s << ", " << (*itp)->name();
		s << ')';
		}
	s << ';' << endl;
	if (_module_item != 0)
		{
		module_item_list::iterator itp(_module_item->begin());
		module_item_list::iterator stop(_module_item->end());
		for (; itp != stop; ++itp)
			s << *(*itp);
		}
	s << "endmodule" << endl;
	return s;
	}

module *
module::read(const read_type &rd)
	{ return rd(); }

void
module::write(const write_type &wr) const
	{ wr(this); }

void
module::setup(const setup_type &setup)
	{ setup(this); }

module::ostream_type &
operator<<(module::ostream_type &s, const module &d)
	{ return d.display(s); }
