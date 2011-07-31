// Verilog Behavioral Simulator
// Copyright (C) 1996,2001,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// erdwr.cc

#include "common/error.h"
#include "expr/funccall.h"
#include "expr/concat.h"
#include "expr/number.h"
#include "expr/qstr.h"
#include "expr/rangeid.h"
#include "expr/unary_op.h"
#include "expr/binary_op.h"
#include "expr/ternary_op.h"
#include "expr/mintypmax.h"
#include "expr/erdwr.h"
#include "misc/mrdwr.h"


expr_base *
read_expr::operator()() const
	{
	VBSOBJ_EXPR_TYPE type;
	_in >> (int &) type;
	long ln;
	_in >> ln;
	expr_base *expr = 0;
	if ((type & VBSOBJ_EXPR_INFO) == VBSOBJ_EXPR_TERNARY)
		{
		expr_base *expr, *true_expr, *false_expr;
		expr = expr_base::read(read_expr(_in));
		true_expr = expr_base::read(read_expr(_in));
		false_expr = expr_base::read(read_expr(_in));
		expr = new ternary_op_expr(expr, true_expr, false_expr);
		}
	else if ((type & VBSOBJ_EXPR_INFO) == VBSOBJ_EXPR_BINARY)
		{
		expr_base *left, *right;
		left = expr_base::read(read_expr(_in));
		right = expr_base::read(read_expr(_in));
		type = (VBSOBJ_EXPR_TYPE) (type & 0x00000fff);
		switch (type)
			{
			case VBSOBJ_EXPR_ADD:
				expr = new binary_op_expr(binary_op_expr::ADD, left, right);
				break;
			case VBSOBJ_EXPR_SUB:
				expr = new binary_op_expr(binary_op_expr::SUB, left, right);
				break;
			case VBSOBJ_EXPR_EQUAL_EQUAL:
				expr = new binary_op_expr(binary_op_expr::EQUAL_EQUAL, left, right);
				break;
			case VBSOBJ_EXPR_NOT_EQUAL:
				expr = new binary_op_expr(binary_op_expr::NOT_EQUAL, left, right);
				break;
			case VBSOBJ_EXPR_AND_AND:
				expr = new binary_op_expr(binary_op_expr::AND_AND, left, right);
				break;
			case VBSOBJ_EXPR_OR_OR:
				expr = new binary_op_expr(binary_op_expr::OR_OR, left, right);
				break;
			case VBSOBJ_EXPR_LES_THAN:
				expr = new binary_op_expr(binary_op_expr::LESS_THAN, left, right);
				break;
			case VBSOBJ_EXPR_LES_EQUAL:
				expr = new binary_op_expr(binary_op_expr::LESS_EQUAL, left, right);
				break;
			case VBSOBJ_EXPR_GRT_THAN:
				expr = new binary_op_expr(binary_op_expr::GRT_THAN, left, right);
				break;
			case VBSOBJ_EXPR_GRT_EQUAL:
				expr = new binary_op_expr(binary_op_expr::GRT_EQUAL, left, right);
				break;
			case VBSOBJ_EXPR_LOGIC_AND:
				expr = new binary_op_expr(binary_op_expr::LOGIC_AND, left, right);
				break;
			case VBSOBJ_EXPR_LOGIC_OR:
				expr = new binary_op_expr(binary_op_expr::LOGIC_OR, left, right);
				break;
			case VBSOBJ_EXPR_LOGIC_XOR:
				expr = new binary_op_expr(binary_op_expr::LOGIC_XOR, left, right);
				break;
			case VBSOBJ_EXPR_LEFT_SHIFT:
				expr = new binary_op_expr(binary_op_expr::LEFT_SHIFT, left, right);
				break;
			case VBSOBJ_EXPR_RIGHT_SHIFT:
				expr = new binary_op_expr(binary_op_expr::RIGHT_SHIFT, left, right);
				break;
			default:
				vbs_err.set_data(vbs_error::SE_VBSOBJ, -1);
				vbs_err.out("expression");
				break;
			}
		}
	else if ((type & VBSOBJ_EXPR_INFO) == VBSOBJ_EXPR_UNARY)
		{
		expr_base *sub_expr;
		sub_expr = expr_base::read(read_expr(_in));
		switch (type)
			{
			case VBSOBJ_EXPR_ADD:
				expr = new unary_op_expr(unary_op_expr::PLUS_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_SUB:
				expr = new unary_op_expr(unary_op_expr::MINUS_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_NOT:
				expr = new unary_op_expr(unary_op_expr::NOT_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_INVERT:
				expr = new unary_op_expr(unary_op_expr::INVERT_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_REDUCE_AND:
				expr = new unary_op_expr(unary_op_expr::AND_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_REDUCE_NAND:
				expr = new unary_op_expr(unary_op_expr::NAND_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_REDUCE_OR:
				expr = new unary_op_expr(unary_op_expr::OR_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_REDUCE_NOR:
				expr = new unary_op_expr(unary_op_expr::NOR_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_REDUCE_XOR:
				expr = new unary_op_expr(unary_op_expr::XOR_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_REDUCE_XNOR:
				expr = new unary_op_expr(unary_op_expr::XNOR_EXPR, sub_expr);
				break;
			case VBSOBJ_EXPR_REDUCE_NXOR:
				expr = new unary_op_expr(unary_op_expr::NXOR_EXPR, sub_expr);
				break;
			default:
				vbs_err.set_data(vbs_error::SE_VBSOBJ, -1);
				vbs_err.out("expression");
				break;
			}
		}
	else
		{
		switch (type)
			{
			case VBSOBJ_EXPR_FUNC_ENABLE:
				{
				str_type::size_type size;
				_in >> (str_type::size_type &) size;
				char name[256];
				_in.get(name, size);
				_in >> size;
				unsigned long data;
				_in >> (unsigned long &) data;
				function_call::arg_list *arglst = 0;
				if (size > 0)
					{
					arglst = new function_call::arg_list;
					for (str_type::size_type i = 0; i < size; ++i)
						{
						expr_type *sub_expr = expr_type::read(read_expr(_in));
						arglst->push_back(sub_expr);
						}
					}
				expr = new function_call(name, arglst);
				break;
				}
			case VBSOBJ_EXPR_NUMBER:
				{
				bool msb_lower;
				_in >> (bool &) msb_lower;
				num_type::position_type ms, ls;
				if (msb_lower)
					{
					_in >> (num_type::position_type &) ms;
					_in >> (num_type::position_type &) ls;
					}
				else
					{
					_in >> (num_type::position_type &) ls;
					_in >> (num_type::position_type &) ms;
					}
				num_type *n= new number(ms, ls);
				num_type::size_type size;
				_in >> (num_type::size_type &) size;
				for (num_type::size_type i = 0; i < size; ++i)
					_in >> (char &) (*n)(i);
				expr = n;
				break;
				}
			case VBSOBJ_EXPR_QOUTED_STR:
				{
				str_type::size_type size;
				_in >> (str_type::size_type &) size;
				char buf[256];
				_in.get(buf, size);
				expr = new qouted_str(buf);
				break;
				}
			case VBSOBJ_EXPR_RANGE_ID:
				{
				str_type::size_type size;
				_in >> (str_type::size_type &) size;
				char name[256];
				_in.get(name, size);
				expr = new range_id(name);
				break;
				}
			case VBSOBJ_EXPR_RANGE_ID_WITH_SELECT:
				{
				str_type::size_type size;
				_in >> (str_type::size_type &) size;
				char name[256];
				_in.get(name, size);
				select_type *sel = select_type::read(read_select(_in));
				expr = new range_id(name, sel);
				break;
				}
			default:
				vbs_err.set_data(vbs_error::SE_VBSOBJ, -1);
				vbs_err.out("expression");
				break;
			}
		}
	expr->_lineno = ln;
	return expr;
	}


void
write_expr::operator()(const function_call *p) const
	{
	_out << (int) VBSOBJ_EXPR_FUNC_ENABLE;
	_out << p->_lineno;
	_out << p->_name.size();
	_out << p->_name.c_str();
	_out << p->_argument->size();
	unsigned long data = p->_argument->size();
	data &= 0xff;
	data <<= 16;
	data |= VBSOBJ_EXPR_LIST;
	_out << data;
	function_call::arg_list::const_iterator itp(p->_argument->begin());
	function_call::arg_list::const_iterator stop(p->_argument->end());
	for (; itp != stop; ++itp)
		(*itp)->write(write_expr(_out));
	}

void
write_expr::operator()(const concatenation *p) const
	{
	_out << (int) VBSOBJ_EXPR_CONCATENATE;
	_out << p->_lineno;
	_out << p->_exp_list->size();
	concatenation::expr_list::const_iterator itp(p->_exp_list->begin());
	concatenation::expr_list::const_iterator stop(p->_exp_list->end());
	for (; itp != stop; ++itp)
		(*itp)->write(write_expr(_out));
	}

void
write_expr::operator()(const number *p) const
	{
	_out << (int) VBSOBJ_EXPR_NUMBER;
	_out << p->_lineno;
	_out << p->msb_lower();
	_out << p->start_pos();
	_out << p->stop_pos();
	number::position_type size = (number::position_type) p->size();
	const bit_vector &bit_array(*p);
	_out << size;
	for (number::position_type i = 0; i < size; ++i)
		_out << (char) bit_array(i);
	}

void
write_expr::operator()(const qouted_str *p) const
	{
	_out << (int) VBSOBJ_EXPR_QOUTED_STR;
	_out << p->_lineno;
	_out << p->get_str().size();
	_out << p->get_str();
	}

void
write_expr::operator()(const range_id *p) const
	{
	if (p->_select != 0)
		_out << (int) VBSOBJ_EXPR_RANGE_ID_WITH_SELECT;
	else
		_out << (int) VBSOBJ_EXPR_RANGE_ID;
	_out << p->_lineno;
	_out << p->_name.size();
	_out << p->_name.c_str();
	if (p->_select != 0)
		p->_select->write(write_select(_out));
	}

void
write_expr::operator()(const unary_op_expr *p) const
	{
	switch (p->_operator)
		{
		case unary_op_expr::PLUS_EXPR:
			_out << (int) VBSOBJ_EXPR_ADD;
			break;
		case unary_op_expr::MINUS_EXPR:
			_out << (int) VBSOBJ_EXPR_SUB;
			break;
		case unary_op_expr::NOT_EXPR:
			_out << (int) VBSOBJ_EXPR_NOT;
			break;
		case unary_op_expr::INVERT_EXPR:
			_out << (int) VBSOBJ_EXPR_INVERT;
			break;
		case unary_op_expr::AND_EXPR:
			_out << (int) VBSOBJ_EXPR_REDUCE_AND;
			break;
		case unary_op_expr::NAND_EXPR:
			_out << (int) VBSOBJ_EXPR_REDUCE_NAND;
			break;
		case unary_op_expr::OR_EXPR:
			_out << (int) VBSOBJ_EXPR_REDUCE_OR;
			break;
		case unary_op_expr::NOR_EXPR:
			_out << (int) VBSOBJ_EXPR_REDUCE_NOR;
			break;
		case unary_op_expr::XOR_EXPR:
			_out << (int) VBSOBJ_EXPR_REDUCE_XOR;
			break;
		case unary_op_expr::XNOR_EXPR:
			_out << (int) VBSOBJ_EXPR_REDUCE_XNOR;
			break;
		case unary_op_expr::NXOR_EXPR:
			_out << (int) VBSOBJ_EXPR_REDUCE_NXOR;
			break;
		}
	_out << p->_lineno;
	p->_expr->write(write_expr(_out));
	}

void
write_expr::operator()(const binary_op_expr *p) const
	{
	_out << (int) VBSOBJ_EXPR_ADD;
	_out << p->_lineno;
	p->_left->write(write_expr(_out));
	p->_right->write(write_expr(_out));
	}

void
write_expr::operator()(const ternary_op_expr *p) const
	{
	_out << (int) VBSOBJ_EXPR_TERNARY;
	_out << p->_lineno;
	p->_expr->write(write_expr(_out));
	p->_true_expr->write(write_expr(_out));
	p->_false_expr->write(write_expr(_out));
	}

void
write_expr::operator()(const mintypmax_expr *p) const
	{
	_out << (int) VBSOBJ_EXPR_MINTYPMAX;
	_out << p->_lineno;
	p->_min_expr->write(write_expr(_out));
	p->_typ_expr->write(write_expr(_out));
	p->_max_expr->write(write_expr(_out));
	}
