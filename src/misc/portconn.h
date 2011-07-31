// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// portconn.h

#ifndef _PORTCONN_H
#define _PORTCONN_H

#include "common/combase.h"
#include "expr/exprbase.h"

struct port_conn_setup;
struct port_conn_write;

class port_connection : public virtual common_base
	{
	friend struct setup_port_conn;
	friend struct setup_port;
	friend struct write_port_conn;

public:
	typedef expr_base expr_type;
	typedef port_conn_setup setup_type;
	typedef port_conn_write write_type;

	port_connection(expr_type * = 0);
	~port_connection();

	ostream_type &display(ostream_type &) const;
	void write(const write_type &) const;
	bool setup(const setup_type &);

private:
	expr_type *_expr;
	};

port_connection::ostream_type &
operator<<(port_connection::ostream_type &, const port_connection &);

struct port_conn_setup
	{
	virtual bool operator()(port_connection *) const { return false; }
	};

struct port_conn_write
	{
	virtual void operator()(const port_connection *) const {}
	};

#endif //_PORTCONN_H
