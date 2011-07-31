// Verilog Behavioral Simulator
// Copyright (C) 1996,2002 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// decrdwr.cc

#include "expr/erdwr.h"
#include "misc/delaynum.h"
#include "misc/delayid.h"
#include "misc/evntexpr.h"
#include "misc/decrdwr.h"


void
write_event_expr::operator()(const event_expr *p) const
	{
	_out << (int) VBSOBJ_DEC_EVENT_EXPR;
	_out << p->_lineno;
	if (p->_type == TRIG_NONE)
		_out << (char) 0;
	else if (p->_type == TRIG_NEGEDGE)
		_out << (char) 1;
	else if (p->_type == TRIG_POSEDGE)
		_out << (char) 2;
	else if (p->_type == TRIG_CHNG)
		_out << (char) 4;
	p->_expr->write(write_expr(_out));
	}


// Write delay or event control function object.

void
write_dec::operator()(const delay_num *p) const
	{
	_out << (int) VBSOBJ_DEC_NUMBER;
	_out << p->_lineno;
	_out << p->msb_lower();
	_out << p->start_pos();
	_out << p->stop_pos();
	delay_num::position_type size = (delay_num::position_type) p->size();
	const bit_vector &bit_array(*p);
	_out << size;
	for (delay_num::position_type i = 0; i < size; ++i)
		_out << (char) bit_array(i);
	}

void
write_dec::operator()(const delay_id *p) const
	{
	_out << (int) VBSOBJ_DEC_RANGEID;
	_out << p->_lineno;
	p->_expr->write(write_expr(_out));
	}

void
write_dec::operator()(const ored_event_expr *p) const
	{
	_out << (int) VBSOBJ_DEC_ORED_EVENT_EXPR;
	_out << p->_lineno;
	unsigned long data = p->_ored_list->size();
	data &= 0xff; // Limited to 255.
	data <<= 16; // Shift it into third byte.
	data |= VBSOBJ_DEC_EVENT_EXPR_LIST;
	_out << data;
	ored_event_expr::event_expr_list::const_iterator
		itp(p->_ored_list->begin());
	ored_event_expr::event_expr_list::const_iterator
		stop(p->_ored_list->end());
	for (; itp != stop; ++itp)
		(*itp)->write(write_event_expr(_out));
	}
