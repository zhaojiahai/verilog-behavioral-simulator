// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// d_rdwr.cc

#include "moditm/d_io.h"
#include "moditm/d_net.h"
#include "moditm/d_reg.h"
#include "moditm/d_int.h"
#include "moditm/d_param.h"
#include "moditm/d_rdwr.h"
#include "misc/mrdwr.h"
#include "expr/erdwr.h"
#include "stmt/srdwr.h"


tfdecl_base *
decl_read::operator()() const
	{
#if 0
	VBSOBJ_DECL_TYPE type;
	_in >> (int &) type;
	select_type *sel = 0;
	if ((type & 0x00010000) != 0)
		sel = select_base::read(read_select(_in));
	type = (VBSOBJ_DECL_TYPE) (type & 0xfffeffff); // Mask out have bit select.
	unsigned long data;
	_in >> (unsigned long &) data;
	decl_base::ident_list *idlst;
	data >>= 16;
	data &= 0xff;
	for (int i = 0; i < data; ++i)
		{
		expr = expr_base::read(read_expr(_in));
		idlst->push_back(expr);
		}
	tfdecl_base *tfdecl = 0;
	switch (type)
		{
		case VBSOBJ_DECL_IN:
			tfdecl = new io_decl(io_decl::INPUT, idlst, sel);
			break;
		case VBSOBJ_DECL_OUT:
			tfdecl = new io_decl(io_decl::OUTPUT, idlst, sel);
			break;
		case VBSOBJ_DECL_INOUT:
			tfdecl = new io_decl(io_decl::INOUT, idlst, sel);
			break;
		case VBSOBJ_DECL_WIRE:
			tfdecl = new net_decl(net_decl::WIRE, idlst, sel);
			break;
		case VBSOBJ_DECL_REG:
			tfdecl = new reg_decl(idlst, sel);
			break;
		case VBSOBJ_DECL_INT:
			tfdecl = new int_decl(idlst);
			break;
		default:
			vbs_err.set_data(vbs_error::VBSOBJ, -1);
			vbs_err.out("tf declaration");
			break;
		}
	return tfdecl;
#else
	return 0;
#endif
	}


void
decl_write::operator()(const io_decl *p) const
	{
	if (p->_range != 0)
		_out << (int) VBSOBJ_DECL_IN_WITH_SELECT;
	else
		_out << (int) VBSOBJ_DECL_IN;
	_out << p->_lineno;
	if (p->_range != 0)
		p->_range->write(write_select(_out));
	unsigned long data = p->_id_list->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_DECL_IN_LIST;
	_out << data;
	ident_list::const_iterator itp(p->_id_list->begin());
	ident_list::const_iterator stop(p->_id_list->end());
	for (; itp != stop; ++itp)
		(*itp)->write(write_expr(_out));
	}

void
decl_write::operator()(const net_decl *p) const
	{
	if (p->_range != 0)
		_out << (int) VBSOBJ_DECL_WIRE_WITH_SELECT;
	else
		_out << (int) VBSOBJ_DECL_WIRE;
	_out << p->_lineno;
	if (p->_range != 0)
		p->_range->write(write_select(_out));
	unsigned long data = p->_id_list->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_DECL_WIRE_LIST;
	_out << data;
	net_decl::decl_assign_list::const_iterator itp(p->_id_list->begin());
	net_decl::decl_assign_list::const_iterator stop(p->_id_list->end());
	for (; itp != stop; ++itp)
		{
		(*itp)->_ident->write(write_expr(_out));
		if ((*itp)->_rval != 0)
			(*itp)->_rval->write(write_expr(_out));
		}
	}

void
decl_write::operator()(const reg_decl *p) const
	{
	if (p->_range != 0)
		_out << (int) VBSOBJ_DECL_REG_WITH_SELECT;
	else
		_out << (int) VBSOBJ_DECL_REG;
	_out << p->_lineno;
	if (p->_range != 0)
		p->_range->write(write_select(_out));
	unsigned long data = p->_id_list->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_DECL_REG_LIST;
	_out << data;
	reg_decl::decl_assign_list::const_iterator itp(p->_id_list->begin());
	reg_decl::decl_assign_list::const_iterator stop(p->_id_list->end());
	for (; itp != stop; ++itp)
		{
		(*itp)->_ident->write(write_expr(_out));
		if ((*itp)->_rval != 0)
			(*itp)->_rval->write(write_expr(_out));
		}
	}

void
decl_write::operator()(const int_decl *p) const
	{
	_out << (int) VBSOBJ_DECL_INT;
	_out << p->_lineno;
	unsigned long data = p->_id_list->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_DECL_INT_LIST;
	_out << data;
	int_decl::decl_assign_list::const_iterator itp(p->_id_list->begin());
	int_decl::decl_assign_list::const_iterator stop(p->_id_list->end());
	for (; itp != stop; ++itp)
		{
		(*itp)->_ident->write(write_expr(_out));
		if ((*itp)->_rval != 0)
			(*itp)->_rval->write(write_expr(_out));
		}
	}

void
decl_write::operator()(const time_decl *p) const
	{
	// FIXME: unimplemented.
	}

void
decl_write::operator()(const param_decl *p) const
	{
	_out << (int) VBSOBJ_DECL_PARAM;
	_out << p->_lineno;
	unsigned long data = p->_id_list->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_DECL_PARAM_LIST;
	_out << data;
	param_decl::decl_assign_list::const_iterator itp(p->_id_list->begin());
	param_decl::decl_assign_list::const_iterator stop(p->_id_list->end());
	for (; itp != stop; ++itp)
		{
		(*itp)->_ident->write(write_expr(_out));
		if ((*itp)->_rval != 0)
			(*itp)->_rval->write(write_expr(_out));
		}
	}
