// Verilog Behavioral Simulator
// Copyright (C) 2000 George Varughese
//
// With modifications from:
// Jimen Ching <jimen.ching@gmail.com>
// 
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dumptsk.h

#ifndef _DUMPTSK_H
#define _DUMPTSK_H

#include "stmt/stmtbase.h"

class systask_dumpon : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;

public:
	systask_dumpon(); // No arguments, name is fixed!

	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);
	
private:
	str_type _name;
	};

class systask_dumpoff : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;

public:
	systask_dumpoff(); // No arguments, name is fixed!

	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	str_type _name;
	};

class systask_dumpall : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;

public:
	systask_dumpall(); // No arguments, name is fixed!

	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	str_type _name;
	};

//////////////////////////////////////////////////////////
// $dumpvars(0,scope);
//////////////////////////////////////////////////////////
class systask_dumpvars : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;

public:
	systask_dumpvars(); // No arguments, name is fixed!

	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	str_type _name;
	};

class systask_dumpfile : public stmt_base
	{
	friend struct setup_stmt;
	friend struct trigger_stmt;

public:
	systask_dumpfile(); // No arguments, name is fixed!

	ostream_type &display(ostream_type &) const;
	void setup(const setup_type &);
	bool trigger(const trigger_type &);

private:
	str_type _name;
	};

#endif // _DUMPTSK_H
