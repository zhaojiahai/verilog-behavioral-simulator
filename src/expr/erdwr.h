// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// erdwr.h

#ifndef _ERDWR_H
#define _ERDWR_H

#include "exprbase.h"

struct select_base;

enum VBSOBJ_EXPR_TYPE
	{
	VBSOBJ_EXPR_LIST					= 0x81000000,
	VBSOBJ_EXPR_ADD						= 0x01000001,
	VBSOBJ_EXPR_AND_AND					= 0x01000002,
	VBSOBJ_EXPR_EQUAL_EQUAL				= 0x01000003,
	VBSOBJ_EXPR_FUNC_ENABLE				= 0x01000004,
	VBSOBJ_EXPR_GRT_EQUAL				= 0x01000005,
	VBSOBJ_EXPR_GRT_THAN				= 0x01000006,
	VBSOBJ_EXPR_INVERT					= 0x01000007,
	VBSOBJ_EXPR_LEFT_SHIFT				= 0x01000008,
	VBSOBJ_EXPR_LES_EQUAL				= 0x01000009,
	VBSOBJ_EXPR_LES_THAN				= 0x0100000a,
	VBSOBJ_EXPR_LOGIC_AND				= 0x0100000b,
	VBSOBJ_EXPR_LOGIC_OR				= 0x0100000c,
	VBSOBJ_EXPR_LOGIC_XOR				= 0x0100000d,
	VBSOBJ_EXPR_NOT						= 0x0100000e,
	VBSOBJ_EXPR_NOT_EQUAL				= 0x0100000f,
	VBSOBJ_EXPR_NUMBER					= 0x01000010,
	VBSOBJ_EXPR_OR_OR					= 0x01000011,
	VBSOBJ_EXPR_QOUTED_STR				= 0x01000012,
	VBSOBJ_EXPR_RIGHT_SHIFT				= 0x01000013,
	VBSOBJ_EXPR_RANGE_ID				= 0x01000014,
	VBSOBJ_EXPR_RANGE_ID_WITH_SELECT	= 0x01100014,
	VBSOBJ_EXPR_REDUCE_AND				= 0x01000015,
	VBSOBJ_EXPR_REDUCE_NAND				= 0x01000016,
	VBSOBJ_EXPR_REDUCE_OR				= 0x01000017,
	VBSOBJ_EXPR_REDUCE_NOR				= 0x01000018,
	VBSOBJ_EXPR_REDUCE_XOR				= 0x01000019,
	VBSOBJ_EXPR_REDUCE_XNOR				= 0x0100001a,
	VBSOBJ_EXPR_REDUCE_NXOR				= 0x0100001b,
	VBSOBJ_EXPR_SUB						= 0x0100001c,
	VBSOBJ_EXPR_UNARY					= 0x00001000,
	VBSOBJ_EXPR_BINARY					= 0x00002000,
	VBSOBJ_EXPR_TERNARY					= 0x00004000,
	VBSOBJ_EXPR_MINTYPMAX				= 0x00008000,
	VBSOBJ_EXPR_FUNCTION_ENABLE			= 0x00010000,
	VBSOBJ_EXPR_CONCATENATE				= 0x00020000,
	VBSOBJ_EXPR_INFO					= 0x0000f000
	};

struct read_expr : public expr_read
	{
	typedef expr_base expr_type;
	typedef expr_type::str_type str_type;
	typedef expr_type::istream_type istream_type;
	typedef number num_type;
	typedef select_base select_type;

	read_expr(istream_type &i)
		: _in(i)
		{}

	expr_type *operator()() const;

	istream_type &_in;
	};

struct write_expr : public expr_write
	{
	typedef expr_base expr_type;
	typedef expr_type::ostream_type ostream_type;

	write_expr(ostream_type &o)
		: _out(o)
		{}

	void operator()(const function_call *) const;
	void operator()(const concatenation *) const;
	void operator()(const number *) const;
	void operator()(const qouted_str *) const;
	void operator()(const range_id *) const;
	void operator()(const unary_op_expr *) const;
	void operator()(const binary_op_expr *) const;
	void operator()(const ternary_op_expr *) const;
	void operator()(const mintypmax_expr *) const;

	ostream_type &_out;
	};

#endif // _ERDWR_H
