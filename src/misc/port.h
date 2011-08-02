// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// port.h

#ifndef _PORT_H
#define _PORT_H

#include "common/hash.h"
#include "common/combase.h"

struct port_setup;
struct port_read;
struct port_write;

class port : public virtual common_base
	{
	friend struct setup_port;
	friend struct read_port;
	friend struct write_port;

public:
	typedef hash_value hash_type;
	typedef port_setup setup_type;
	typedef port_read read_type;
	typedef port_write write_type;

	port(const char *);

	const str_type &name() const
		{ return _name; }
	const hash_type &index() const
		{ return _index; }
	void index(const hash_type &h)
		{ _index = h; }
	static port *read(const read_type &);
	void write(const write_type &) const;
	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);

private:
	hash_type _index; // Index into symbol table (for setup only).
	str_type _name;
	};

port::ostream_type &operator<<(port::ostream_type &, const port &);

struct port_setup
	{
	virtual ~port_setup() {}
	virtual void operator()(port *) const {}
	};

struct port_read
	{
	virtual ~port_read() {}
	virtual port *operator()() const { return 0; }
	};

struct port_write
	{
	virtual ~port_write() {}
	virtual void operator()(const port *) const {}
	};

#endif // _PORT_H
