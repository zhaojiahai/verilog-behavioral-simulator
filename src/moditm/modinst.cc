// Verilog Behavioral Simulator
// Copyright (C) 1995-1997 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// modinst.cc

#include "expr/exprbase.h" // port connection needs to instantiate expression.
#include "misc/portconn.h"
#include "moditm/modinst.h"

module_instance::module_instance(const char *n, port_conn_list *c)
	: _name(n) { _port_conn = c; }

module_instance::~module_instance()
	{ delete _port_conn; }

module_instance::ostream_type &
module_instance::display(ostream_type &s) const
	{
	s << _name << " (";
	port_conn_list::iterator idx(_port_conn->begin());
	port_conn_list::iterator stop(_port_conn->end());
	s << *(*idx);
	for (++idx; idx != stop; ++idx)
		s << ',' << *(*idx);
	s << ')';
	return s;
	}

void
module_instance::write(const write_type &wr) const
	{ wr(this); }

void
module_instance::setup(const module_instance_setup &setup)
	{ setup(this); }


module_instantiation::module_instantiation(const char *n,
		mod_inst_list *i, param_value_list *pv)
	: _name(n), _param_value(pv) { _instance = i; }

module_instantiation::~module_instantiation()
	{ delete _instance; }

module_instantiation::ostream_type &
module_instantiation::display(ostream_type &s) const
	{
	s << _name << ' ';
	mod_inst_list::iterator idx(_instance->begin());
	mod_inst_list::iterator stop(_instance->end());
	s << *(*idx);
	for (++idx; idx != stop; ++idx)
		s << ',' << endl << *(*idx);
	s << ';' << endl;
	return s;
	}

const module_instantiation::str_type &
module_instantiation::name() const
	{ return _name; }

void
module_instantiation::write(const write_type &wr) const
    { wr(this); }

void
module_instantiation::setup(const setup_type &setup)
	{ setup(this); }

module_instantiation *
module_instantiation::get_module_instantiation()
	{ return this; }
