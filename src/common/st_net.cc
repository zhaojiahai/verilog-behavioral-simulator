// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_net.cc

#include "common/st_net.h"

st_net::st_net(const str_type &n, net_type nt, iodirection_type iodir)
	: st_node_base(n, ""), // Update _type_name after.
	  _type(nt), _iodirection(iodir), _port_ms(-1), _port_ls(-1)
	{
	if (_type == WIRE)
		_type_name = "wire";
	else if (_type == REGISTER)
		_type_name = "reg";
	else if (_type == INTEGER)
		_type_name = "integer";
	else if (_type == MEMORY)
		_type_name = "reg []";
	else if (_type == PARAMETER)
		_type_name = "parameter";
	else
		_type_name = "port";

	_storage = 0;
	_result = 0;
	_upper_mem_idx = -1;
	_lower_mem_idx = -1;
	_port_index = 0;
	_monitor = 0;
	}

st_net::~st_net()
	{
	if (_upper_mem_idx != -1)
		delete [] _storage;
	else
		delete _storage;
	delete _result;
	delete _port_index;
	delete _monitor;
	}

st_net *
st_net::get_net()
	{ return this; }

st_net::ostream_type &
st_net::display(ostream_type &s) const
	{
	s << "('" << _name << "'," << type_name() << ',' << _scope << ')';
	return s;
	}

bool
st_net::monitor(const monitor_type &mon)
	{ return mon(this); }

const st_net::num_type &
st_net::evaluate(const evaluate_type &eval) const
	{ return eval(this); }

void
st_net::setup(const setup_type &setup)
	{ setup(this); }


// Trigger member utility functions.


void
st_net::assignment(const char *fn, int b, position_type m, position_type l)
	{
	st_net_assign assign(*this);
	assign(fn, b, m, l);
	}

void
st_net::assignment(const num_type &num, position_type ms, position_type ls,
				   position_type m, position_type l)
	{
	st_net_assign assign(*this);
	assign(num, ms, ls, m, l);
	}
