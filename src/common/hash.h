// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// hash.h
//
// See "Compiler Design in C" by Allen Holub, (C) 1990 Prentice Hall.
// The hash value object is used by the symbol table to lookup and
// store symbols (names) into the table.  In the future, I hope to use
// one of the ANSI C++ STL containers to do this.  But even these
// containers need a key.  So we still need a hash object.

#ifndef _HASH_H
#define _HASH_H

#include <string>
#include <iostream>

// If the size of the symbol table is not known at the time the hash
// function was called.  Usually means the symbol table was not
// initialized yet.  This error value is in sync with the error
// reporting mechanism used by VBS.  
#define INIT_ERROR_VALUE 1

struct hash_value
	{
	typedef unsigned long index_type;
	typedef signed scope_type;
	typedef std::string str_type;
	typedef std::ostream ostream_type;

	hash_value(index_type idx = 0, scope_type sc = -1, bool t = false)
		: _test(t), _value(idx), _scope(sc) {}
	hash_value(const hash_value &hv)
		: _test(hv._test), _value(hv._value), _scope(hv._scope) {}
	hash_value &operator=(const hash_value &hv)
		{
		_test = hv._test;
		_value = hv._value;
		_scope = hv._scope;
		return *this;
		}
	bool operator==(const hash_value &hv) const
		{
		return (hv._test == _test &&
				hv._value == _value &&
				hv._scope == _scope);
		}

	bool _test;
	index_type _value;
	scope_type _scope;
	};

hash_value hash(const hash_value::str_type &, hash_value::scope_type, bool);
void rehash(hash_value &, hash_value::index_type);
hash_value::ostream_type &operator<<(hash_value::ostream_type &,
									 const hash_value &);

extern unsigned long TABLESIZE;

#endif // _HASH_H
