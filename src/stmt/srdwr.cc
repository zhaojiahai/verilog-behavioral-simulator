// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// srdwr.cc

#include "expr/erdwr.h"
#include "misc/decrdwr.h"
#include "misc/lvalue.h"
#include "misc/mrdwr.h"
#include "stmt/finish.h"
#include "stmt/write.h"
#include "stmt/monitor.h"
#include "stmt/xtime.h"
#include "stmt/seqblk.h"
#include "stmt/taskenbl.h"
#include "stmt/assign.h"
#include "stmt/ifelse.h"
#include "stmt/case.h"
#include "stmt/loopstmt.h"
#include "stmt/srdwr.h"


// Case item for case statement.

enum VBSOBJ_CASE_TYPE
	{
	VBSOBJ_CASE_ITEM = 0x08000001,
	VBSOBJ_CASE_ITEM_LIST = 0x08000001
	};

struct write_case_item : public case_item_write
	{
	typedef case_item::ostream_type ostream_type;

	write_case_item(ostream_type &);
	void operator()(const case_item *) const;

	ostream_type &_out;
	};

write_case_item::write_case_item(ostream_type &o)
	: _out(o)
	{}

void
write_case_item::operator()(const case_item *p) const
	{
	_out << (int) VBSOBJ_CASE_ITEM;
	_out << p->_lineno;
	p->_stmt->write(write_stmt(_out));
	case_item::expr_list::const_iterator itp(p->_expr->begin());
	case_item::expr_list::const_iterator stop(p->_expr->end());
	unsigned long data = p->_expr->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_EXPR_LIST;
	_out << data;
	for (; itp != stop; ++itp)
		(*itp)->write(write_expr(_out));
	}


// All statements.

void
write_stmt::operator()(const null_stmt *p) const
	{
	if (p->_dec != 0)
		_out << (int) VBSOBJ_STMT_NULL_WITH_DEC;
	else
		_out << (int) VBSOBJ_STMT_NULL;
	if (p->_dec != 0)
		p->_dec->write(write_dec(_out));
	}

void
write_stmt::operator()(const seq_block_stmt *p) const
	{
	if (p->_dec != 0)
		_out << (int) VBSOBJ_STMT_SEQ_BLOCK_WITH_DEC;
	else
		_out << (int) VBSOBJ_STMT_SEQ_BLOCK;
	seq_block_stmt::stmt_list::const_iterator itp(p->_stmt->begin());
	seq_block_stmt::stmt_list::const_iterator stop(p->_stmt->end());
	unsigned long data = p->_stmt->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_STMT_LIST;
	_out << data;
	for (; itp != stop; ++itp)
		(*itp)->write(write_stmt(_out));
	if (p->_dec != 0)
		p->_dec->write(write_dec(_out));
	}

void
write_stmt::operator()(const task_enable_stmt *p) const
	{
	if (p->_dec != 0)
		_out << (int) VBSOBJ_STMT_TASK_ENABLE_WITH_DEC;
	else
		_out << (int) VBSOBJ_STMT_TASK_ENABLE;
	_out << p->_name.size();
	_out << p->_name;
	if (p->_argument != 0)
		{
		task_enable_stmt::arg_list::const_iterator itp(p->_argument->begin());
		task_enable_stmt::arg_list::const_iterator stop(p->_argument->end());
		unsigned long data = p->_argument->size();
		data &= 0xff;
		data <<= 16;
		data |= VBSOBJ_EXPR_LIST;
		_out << data;
		for (; itp != stop; ++itp)
			(*itp)->write(write_expr(_out));
		}
	if (p->_dec != 0)
		p->_dec->write(write_dec(_out));
	}

void
write_stmt::operator()(const assignment_stmt *p) const
	{
	if (p->_dec != 0 && p->_delayed_store != 0)
		_out << (int) VBSOBJ_STMT_ASSIGN_WITH_DEC_HOLD;
	else if (p->_dec != 0)
		_out << (int) VBSOBJ_STMT_ASSIGN_WITH_DEC;
	else if (p->_delayed_store != 0)
		_out << (int) VBSOBJ_STMT_ASSIGN_WITH_HOLD;
	else
		_out << (int) VBSOBJ_STMT_ASSIGN;
	p->_lval->write(write_lvalue(_out));
	p->_rval->write(write_expr(_out));
	if (p->_delayed_store != 0)
		p->_delayed_store->write(write_dec(_out));
	if (p->_dec != 0)
		p->_dec->write(write_dec(_out));
	}

void
write_stmt::operator()(const if_else_stmt *p) const
	{
	if (p->_dec != 0)
		_out << (int) VBSOBJ_STMT_IF_ELSE_WITH_DEC;
	else
		_out << (int) VBSOBJ_STMT_IF_ELSE;
	p->_expr->write(write_expr(_out));
	p->_true_stmt->write(write_stmt(_out));
	if (p->_false_stmt != 0)
		p->_false_stmt->write(write_stmt(_out));
	if (p->_dec != 0)
		p->_dec->write(write_dec(_out));
	}

void
write_stmt::operator()(const loop_stmt *p) const
	{
	if (p->_dec != 0)
		{
		switch (p->_type)
			{
			case loop_stmt::FOREVER:
				_out << (int) VBSOBJ_STMT_FOR_WITH_DEC;
				break;
			case loop_stmt::REPEAT:
				_out << (int) VBSOBJ_STMT_FOR_WITH_DEC;
				break;
			case loop_stmt::WHILE:
				_out << (int) VBSOBJ_STMT_FOR_WITH_DEC;
				break;
			case loop_stmt::FOR:
				_out << (int) VBSOBJ_STMT_FOR_WITH_DEC;
				break;
			}
		}
	else
		{
		switch (p->_type)
			{
			case loop_stmt::FOREVER:
				_out << (int) VBSOBJ_STMT_FOREVER;
				break;
			case loop_stmt::REPEAT:
				_out << (int) VBSOBJ_STMT_REPEAT;
				break;
			case loop_stmt::WHILE:
				_out << (int) VBSOBJ_STMT_WHILE;
				break;
			case loop_stmt::FOR:
				_out << (int) VBSOBJ_STMT_FOR;
				break;
			}
		}
	if (p->_expr != 0)
		p->_expr->write(write_expr(_out));
	p->_stmt->write(write_stmt(_out));
	if (p->_preassign != 0)
		p->_preassign->write(write_stmt(_out));
	if (p->_postassign != 0)
		p->_postassign->write(write_stmt(_out));
	if (p->_dec != 0)
		p->_dec->write(write_dec(_out));
	}

void
write_stmt::operator()(const case_stmt *p) const
	{
	if (p->_dec != 0)
		_out << (int) VBSOBJ_STMT_CASE_WITH_DEC;
	else
		_out << (int) VBSOBJ_STMT_NULL;
	p->_expr->write(write_expr(_out));
	case_stmt::citem_list::const_iterator itp(p->_case_item->begin());
	case_stmt::citem_list::const_iterator stop(p->_case_item->end());
	unsigned long data = p->_case_item->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_CASE_ITEM_LIST;
	_out << data;
	for (; itp != stop; ++itp)
		(*itp)->write(write_case_item(_out));
	if (p->_dec != 0)
		p->_dec->write(write_dec(_out));
	}
