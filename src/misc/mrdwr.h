// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mrdwr.h

#ifndef _MRDWR_H
#define _MRDWR_H

#include <iostream>
#include "misc/module.h"
#include "misc/port.h"
#include "misc/portconn.h"
#include "misc/lvalue.h"
#include "misc/selbase.h"

enum VBSOBJ_MISC_TYPE
	{
	VBSOBJ_MISC_MODULE = 0x03000001,
	VBSOBJ_MISC_PORT = 0x03000002,
	VBSOBJ_MISC_PORT_LIST = 0x83000002,
	VBSOBJ_MISC_PORT_CONN = 0x03000003,
	VBSOBJ_MISC_PORT_CONN_LIST = 0x83000003,
	VBSOBJ_MISC_BIT = 0x03000004,
	VBSOBJ_MISC_RANGE = 0x03000005
	};

struct read_module : public module_read
	{
	typedef module module_type;
	typedef module_type::istream_type istream_type;
	typedef module_type::str_type str_type;
	typedef module_type::module_item_list module_item_list_type;
	typedef module_type::port_list port_list_type;
	typedef port port_type;
	typedef module_item_base module_item_type;

	read_module(istream_type &i)
		: _in(i)
		{}

	module *operator()() const;

	istream_type &_in;
	};

struct write_module : public module_write
	{
	typedef module module_type;
	typedef module_type::ostream_type ostream_type;
	typedef module_type::module_item_list module_item_list_type;
	typedef module_type::port_list port_list_type;

	write_module(ostream_type &o)
		: _out(o)
		{}

	void operator()(const module *) const;

	ostream_type &_out;
	};

struct read_port : public port_read
	{
	typedef port port_type;
	typedef port_type::istream_type istream_type;
	typedef port_type::str_type str_type;

	read_port(istream_type &i)
		: _in(i)
		{}

	port *operator()() const;

	istream_type &_in;
	};

struct write_port : public port_write
	{
	typedef port port_type;
	typedef port_type::ostream_type ostream_type;

	write_port(ostream_type &o)
		: _out(o)
		{}

	void operator()(const port *) const;

	ostream_type &_out;
	};

struct write_port_conn : public port_conn_write
	{
	typedef port_connection port_conn_type;
	typedef port_conn_type::ostream_type ostream_type;

	write_port_conn(ostream_type &o)
		: _out(o)
		{}

	void operator()(const port_connection *) const;

	ostream_type &_out;
	};

struct read_select : public select_read
	{
	typedef select_base select_type;
	typedef select_type::istream_type istream_type;
	typedef bit_vector num_type;

	read_select(istream_type &i)
		: _in(i)
		{}

	select_type *operator()() const;

	istream_type &_in;
	};

struct write_select : public select_write
	{
	typedef select_base select_type;
	typedef select_type::ostream_type ostream_type;
	typedef bit_vector num_type;

	write_select(ostream_type &o)
		: _out(o)
		{}

	void operator()(const bit_select *) const;
	void operator()(const part_select *) const;

	ostream_type &_out;
	};

enum VBSOBJ_LVALUE_TYPE
	{
	VBSOBJ_LVALUE = 0x07000001
	};

struct write_lvalue : public lvalue_write
	{
	typedef lvalue lvalue_type;
	typedef lvalue_type::ostream_type ostream_type;

	write_lvalue(ostream_type &o)
		: _out(o)
		{}

	void operator()(const lvalue_type *) const;

	ostream_type &_out;
	};

#endif // _MRDWR_H
