// Verilog Behavioral Simulator
// Copyright (C) 1996,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mrdwr.cc

#include "common/error.h"
#include "misc/port.h"
#include "misc/portconn.h"
#include "misc/module.h"
#include "misc/bitsel.h"
#include "misc/partsel.h"
#include "misc/mrdwr.h"
#include "expr/erdwr.h"
#include "moditm/mirdwr.h"

module *
read_module::operator()() const
	{
	VBSOBJ_MISC_TYPE type;
	_in >> (int &) type;
	if (type != VBSOBJ_MISC_MODULE)
		{
		vbs_err.set_data(vbs_error::SE_VBSOBJ, -1);
		vbs_err.out("module");
		}

	// Get line number, filename and module name.
	long ln;
	_in >> ln;
	str_type::size_type size;
	_in >> size;
	char fn[256];
	_in.get(fn, size);
	_in >> size;
	char name[256];
	_in.get(name, size);

	// Check for port list.
	unsigned long data;
	_in >> data;
	size = (data >> 16) & 0xff;
	data &= 0xffff;
	port_list_type *pl = 0;
	if (data == VBSOBJ_MISC_PORT_LIST)
		{
		pl = new port_list_type;
		for (unsigned int i = 0; i < size; ++i)
			{
			port_type *p = port_type::read(read_port(_in));
			pl->push_back(p);
			}

		// Check for module item list.
		_in >> data;
		size = (data >> 16) & 0xff;
		data &= 0xffff;
		}

	// Also check for module item list, independent of whether we have
	// a port list or not.
	module_item_list_type *mil = 0;
	if (data == VBSOBJ_MI_LIST)
		{
		mil = new module_item_list_type;
		module_item_type *mi;
		for (unsigned int i = 0; i < size; ++i)
			{
			mi = module_item_type::read(read_module_item(_in));
			mil->push_back(mi);
			}
		}
	else
		{
		// If there is no module item list, it is an error.
		vbs_err.set_data(vbs_error::SE_VBSOBJ, -1);
		vbs_err.out("empty module item list");
		}

	module_type *mod = new module_type(name, mil, pl);
	mod->_filename = fn;
	mod->_lineno = ln;

	return mod;
	}

void
write_module::operator()(const module *p) const
	{
	_out << (int) VBSOBJ_MISC_MODULE;
	_out << p->_lineno;
	_out << p->_filename.size();
	_out << p->_filename;
	_out << p->_name.size();
	_out << p->_name;
	// Write port list if any.
	if (p->_port_list != 0)
		{
		unsigned long data = p->_port_list->size();
		data &= 0xff;
		data <<= 16;
		data |= VBSOBJ_MISC_PORT_LIST;
		_out << data;
		port_list_type::const_iterator itp(p->_port_list->begin());
		port_list_type::const_iterator stop(p->_port_list->end());
		for (; itp != stop; ++itp)
			(*itp)->write(write_port(_out));
		}
	// Write module item list.
//	if (p->_module_item != 0)
		{
		unsigned long data = p->_module_item->size();
		data &= 0xff;
		data <<= 16;
		data |= VBSOBJ_MI_LIST;
		_out << data;
		module_item_list_type::const_iterator itp(p->_module_item->begin());
		module_item_list_type::const_iterator stop(p->_module_item->end());
		for (; itp != stop; ++itp)
			(*itp)->write(write_module_item(_out));
		}
	}


port *
read_port::operator()() const
	{
	VBSOBJ_MISC_TYPE type;
	_in >> (int &) type;
	if (type != VBSOBJ_MISC_PORT)
		{
		vbs_err.set_data(vbs_error::SE_VBSOBJ, -1);
		vbs_err.out("port");
		}

	long ln;
	_in >> ln;
	str_type::size_type size;
	_in >> size;
	char name[256];
	_in.get(name, size);
	port *p = new port(name);
	p->_lineno = ln;
	return p;
	}


void
write_port::operator()(const port *p) const
	{
	_out << (int) VBSOBJ_MISC_PORT;
	_out << p->_lineno;
	_out << p->_name.size();
	_out << p->_name;
	}


void
write_port_conn::operator()(const port_connection *p) const
	{
	_out << (int) VBSOBJ_MISC_PORT_CONN;
	_out << p->_lineno;
	p->_expr->write(write_expr(_out));
	}


select_base *
read_select::operator()() const
	{
	vbs_err.set_data(vbs_error::SE_VBSOBJ, -1);
	vbs_err.out("select");
	return 0;
	}


void
write_select::operator()(const bit_select *p) const
	{
	_out << (int) VBSOBJ_MISC_BIT;
	_out << p->_lineno;
	p->_expr->write(write_expr(_out));
	}

void
write_select::operator()(const part_select *p) const
	{
	_out << (int) VBSOBJ_MISC_RANGE;
	_out << p->_lineno;
	// Write the constant
	p->_le->write(write_expr(_out));
	p->_re->write(write_expr(_out));
	}


void
write_lvalue::operator()(const lvalue *p) const
	{
	_out << (int) VBSOBJ_LVALUE;
	_out << p->_lineno;
	_out << p->_exp_list->size();
	lvalue::expr_list::iterator itp(p->_exp_list->begin());
	lvalue::expr_list::iterator stop(p->_exp_list->end());
	for (; itp != stop; ++itp)
		(*itp)->write(write_expr(_out));
	}
