// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_module.cc

#include "misc/port.h"
#include "moditm/mibase.h"
#include "common/st_mod.h"

st_module::st_module(const str_type &n)
	: st_node_base(n, "module"), _is_top_level(true)
	{
	_port_list = 0;
	_module_item = 0;
	}

st_module::~st_module()
	{
	delete _port_list;
	delete _module_item;
	}

const bool
st_module::is_top_level() const
	{ return _is_top_level; }

void
st_module::is_top_level(bool b)
	{ _is_top_level = b; }

st_module *
st_module::get_module()
	{ return this; }

st_module::module_item_list *
st_module::get_module_item()
	{ return _module_item; }

st_module::ostream_type &
st_module::display(ostream_type &s) const
	{
	s << "('" << _name << "'," << type_name() << ',' << _scope << ')';
	return s;
	}

void
st_module::setup(const setup_type &setup)
	{ setup(this); }
