// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// srdwr.h

#ifndef _SRDWR_H
#define _SRDWR_H

#include <iostream>
#include "stmt/stmtbase.h"
#include "stmt/case.h"

enum VBSOBJ_STMT_TYPE
	{
	VBSOBJ_STMT_LIST = 0x88000000,
	VBSOBJ_STMT_NULL = 0x08000001,
	VBSOBJ_STMT_NULL_WITH_DEC = 0x08010001,
	VBSOBJ_STMT_SEQ_BLOCK = 0x08000002,
	VBSOBJ_STMT_SEQ_BLOCK_WITH_DEC = 0x08010002,
	VBSOBJ_STMT_TASK_ENABLE = 0x08000003,
	VBSOBJ_STMT_TASK_ENABLE_WITH_DEC = 0x08010003,
	VBSOBJ_STMT_ASSIGN = 0x08000004,
	VBSOBJ_STMT_ASSIGN_WITH_DEC = 0x08010004,
	VBSOBJ_STMT_ASSIGN_WITH_HOLD = 0x08020004,
	VBSOBJ_STMT_ASSIGN_WITH_DEC_HOLD = 0x08030004,
	VBSOBJ_STMT_ASSIGN_LIST = 0x88000004,
	VBSOBJ_STMT_IF = 0x08000005,
	VBSOBJ_STMT_IF_WITH_DEC = 0x08010005,
	VBSOBJ_STMT_IF_ELSE = 0x08000006,
	VBSOBJ_STMT_IF_ELSE_WITH_DEC = 0x08010006,
	VBSOBJ_STMT_CASE = 0x08000007,
	VBSOBJ_STMT_CASE_WITH_DEC = 0x08010007,
	VBSOBJ_STMT_FOR = 0x08000008,
	VBSOBJ_STMT_FOR_WITH_DEC = 0x08010008,
	VBSOBJ_STMT_FOREVER = 0x08000009,
	VBSOBJ_STMT_FOREVER_WITH_DEC = 0x08010009,
	VBSOBJ_STMT_REPEAT = 0x0800000a,
	VBSOBJ_STMT_REPEAT_WITH_DEC = 0x0801000a,
	VBSOBJ_STMT_WHILE = 0x0800000b,
	VBSOBJ_STMT_WHILE_WITH_DEC = 0x0801000b
	};

struct write_stmt : public stmt_write
	{
	typedef std::ostream ostream_type;

	write_stmt(ostream_type &o)
		: _out(o)
		{}
	virtual ~write_stmt()
		{}

	void operator()(const null_stmt *) const;
	void operator()(const seq_block_stmt *) const;
	void operator()(const task_enable_stmt *) const;
	void operator()(const assignment_stmt *) const;
	void operator()(const if_else_stmt *) const;
	void operator()(const case_stmt *) const;
	void operator()(const loop_stmt *) const;

	ostream_type &_out;
	};

#endif // _SRDWR_H
