// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// modinst.h
//
// This file contains all objects needed for module instantiation.
// Which are module instance, and port connection objects.

#ifndef _MODINST_H
#define _MODINST_H

#include "common/ptr.h"
#include "common/combase.h"
#include "expr/exprbase.h"
#include "misc/portconn.h"
#include "moditm/mibase.h"

struct module_instance_setup;
struct module_instance_write;

class module_instance : public virtual common_base
	{
	friend struct setup_module_instance;
	friend struct write_module_instance;

public:
	typedef port_connection port_conn;
	typedef basic_ptr<port_conn> port_conn_ptr;
	typedef list_type<port_conn_ptr> port_conn_list;

	typedef module_instance_setup setup_type;
	typedef module_instance_write write_type;

	module_instance(const char *, port_conn_list *);
	~module_instance();

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	void setup(const setup_type &);

private:
	str_type _name;
	port_conn_list *_port_conn;
	};

inline module_instance::ostream_type &
operator<<(module_instance::ostream_type &o, const module_instance &mi)
	{ return mi.display(o); }

class module_instantiation : public module_item_base
	{
	friend struct setup_module_item;
	friend struct write_module_item;

public:
	typedef expr_base expr_type;
	typedef basic_ptr<expr_type> expr_ptr;
	typedef list_type<expr_ptr> param_value_list;
	typedef module_instance mod_inst;
	typedef basic_ptr<mod_inst> mod_inst_ptr;
	typedef list_type<mod_inst_ptr> mod_inst_list;

	module_instantiation(const char *, mod_inst_list *, param_value_list * = 0);
	~module_instantiation();

	ostream_type &display(ostream_type &) const;
	const str_type &name() const;
	void write(const write_type &) const;
	void setup(const setup_type &);
	module_instantiation *get_module_instantiation();

private:
	str_type _name;
	param_value_list *_param_value;
	mod_inst_list *_instance;
	};

struct module_instance_setup
	{
	virtual void operator()(module_instance *) const {}
	};

struct module_instance_write
	{
	virtual void operator()(const module_instance *) const {}
	};

#endif // _MODINST_H
