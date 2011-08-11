// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// mirdwr.cc

#include "common/error.h"
#include "moditm/initial.h"
#include "moditm/always.h"
#include "moditm/cassign.h"
#include "moditm/func.h"
#include "moditm/task.h"
#include "moditm/modinst.h"
#include "moditm/mirdwr.h"
#include "moditm/tfrdwr.h"
#include "moditm/d_rdwr.h"
#include "misc/mrdwr.h"
#include "misc/decrdwr.h"
#include "stmt/srdwr.h"


enum
	{
	VBSOBJ_MODULE_INSTANCE = 0x06000001,
	VBSOBJ_MODULE_INSTANCE_LIST = 0x86000001
	};

struct write_module_instance : public module_instance_write
	{
	typedef module_instance::ostream_type ostream_type;

	write_module_instance(ostream_type &o)
		: _out(o)
		{}

	void operator()(const module_instance *) const;

	ostream_type &_out;
	};

void
write_module_instance::operator()(const module_instance *p) const
	{
	_out << (int) VBSOBJ_MODULE_INSTANCE;
	_out << p->_lineno;
	_out << p->_name.size();
	_out << p->_name;
	if (p->_port_conn != 0)
		{
		module_instance::port_conn_list::const_iterator
			itp(p->_port_conn->begin());
		module_instance::port_conn_list::const_iterator
			stop(p->_port_conn->end());
		unsigned long data = p->_port_conn->size();
		data &= 0xff;
		data <<= 16;
		data |= VBSOBJ_MISC_PORT_CONN_LIST;
		_out << data;
		for (; itp != stop; ++itp)
			(*itp)->write(write_port_conn(_out));
		}
	}


// Module item read/write function object.


module_item_base *
read_module_item::operator()() const
	{
	// Must be able to read both module items and tf declarations.
	vbs_err.set_data(vbs_error::SE_VBSOBJ, -1);
	vbs_err.out("module item");
	return 0;
	}


void
write_module_item::operator()(const initial *p) const
	{
	_out << (int) VBSOBJ_MI_INITIAL;
	_out << p->_lineno;
	p->_stmt->write(write_stmt(_out));
	}

void
write_module_item::operator()(const always *p) const
	{
	_out << (int) VBSOBJ_MI_ALWAYS;
	_out << p->_lineno;
	p->_stmt->write(write_stmt(_out));
	}

void
write_module_item::operator()(const cont_assign *p) const
	{
	if (p->_dec != 0)
		_out << (int) VBSOBJ_MI_CONT_ASSIGN_WITH_DELAY;
	else
		_out << (int) VBSOBJ_MI_CONT_ASSIGN;
	_out << p->_lineno;
	unsigned long data = p->_assign->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_STMT_ASSIGN_LIST;
	_out << data;
	cont_assign::assign_stmt_list::const_iterator itp(p->_assign->begin());
	cont_assign::assign_stmt_list::const_iterator stop(p->_assign->end());
	for (; itp != stop; ++itp)
		(*itp)->write(write_stmt(_out));
	if (p->_dec != 0)
		p->_dec->write(write_dec(_out));
	}

void
write_module_item::operator()(const function *p) const
	{
	if (p->_retsize != 0)
		_out << (int) VBSOBJ_MI_FUNCTION_WITH_RETSIZE;
	else
		_out << (int) VBSOBJ_MI_FUNCTION;
	_out << p->_lineno;
	_out << p->_name.size();
	_out << p->_name;
	if (p->_retsize != 0)
		p->_retsize->write(write_select(_out));
	p->_stmt->write(write_stmt(_out));
	unsigned long data = p->_decl->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_DECL_LIST;
	_out << data;
	function::decl_list::const_iterator itp(p->_decl->begin());
	function::decl_list::const_iterator stop(p->_decl->end());
	for (; itp != stop; ++itp)
		(*itp)->write(write_tfdecl(_out));
	}

void
write_module_item::operator()(const task *p) const
	{
	_out << (int) VBSOBJ_MI_TASK;
	_out << p->_lineno;
	_out << p->_name.size();
	_out << p->_name;
	p->_stmt->write(write_stmt(_out));
	unsigned long data = p->_decl->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_DECL_LIST;
	_out << data;
	task::decl_list::const_iterator itp(p->_decl->begin());
	task::decl_list::const_iterator stop(p->_decl->end());
	for (; itp != stop; ++itp)
		(*itp)->write(write_tfdecl(_out));
	}

void
write_module_item::operator()(const module_instantiation *p) const
	{
	_out << (int) VBSOBJ_MI_MODULE_INSTAN;
	_out << p->_lineno;
	_out << p->_name.size();
	_out << p->_name;
	unsigned long data = p->_instance->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_MODULE_INSTANCE_LIST;
	_out << data;
	module_instantiation::mod_inst_list::const_iterator
		itp(p->_instance->begin());
	module_instantiation::mod_inst_list::const_iterator
		stop(p->_instance->end());
	for (; itp != stop; ++itp)
		(*itp)->write(write_module_instance(_out));
	}


// Front ends to make virtual calls work.


void
write_module_item::operator()(const io_decl *p) const
	{ decl_write::operator()(p); }

void
write_module_item::operator()(const net_decl *p) const
	{ decl_write::operator()(p); }

void
write_module_item::operator()(const reg_decl *p) const
	{ decl_write::operator()(p); }

void
write_module_item::operator()(const int_decl *p) const
	{ decl_write::operator()(p); }

void
write_module_item::operator()(const time_decl *p) const
	{ decl_write::operator()(p); }

void
write_module_item::operator()(const param_decl *p) const
	{ decl_write::operator()(p); }
