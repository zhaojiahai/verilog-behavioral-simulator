// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// sym_tab.h
//
// Symbol Table.  This symbol table is an array of buckets.  Each bucket
// contains a list of nodes.  The list grows with the symbols of different
// scope.  Each scope has it's own node in the list, but only one node.
// If the symbol resulted in the same hash value, and has the same scope,
// it is either a duplicate or has collided.  If collision has occured, we
// rehash and repeat.

#ifndef _SYM_TAB_H
#define _SYM_TAB_H

#include "common/ptr.h"

// FIXME!
// Due to a bug in GCC, I can't make symbol_table a template class.
// So now we need to make st_node known...
#include "common/st_node.h"

#define HASHSIZE 65537 // Size of symbol table.

//template<class T>
class symbol_table
	{
public:
	typedef unsigned long size_type;
	//typedef T node_type;
	typedef st_node_base node_type;
	typedef node_type::hash_type hash_type;
	typedef node_type::scope_type scope_type;
	typedef node_type::scopelist_type scopelist_type;
	typedef node_type::str_type str_type;
	typedef basic_ptr<node_type> node_ptr;
	typedef node_type::list_type<node_ptr> bucket_type;
	typedef node_type::ostream_type ostream_type;

	symbol_table(size_type = 0);
	~symbol_table();

	ostream_type &display(ostream_type &) const;
	bool initialize(size_type sz);
	scope_type &scope(); // To increment scope.
	hash_type add(node_type *, bool init = false);
	void remove(const hash_type &);
	hash_type find(const str_type &, scopelist_type &, bool init = false);
	node_type *get(const hash_type &);

	// to iterate from outside
	bucket_type::iterator begin(size_type & i)
		{ return _table[i].begin(); }
	bucket_type::iterator end(size_type & i)
		{ return _table[i].end(); }
	size_type size() const
		{ return _size ; }
	size_type size(size_type &i) const
		{ return _table[i].size(); }
	size_type amount() const
		{ return _amount; }

private:
	size_type _size;
	size_type _amount;
	scope_type _last_scope_number;
	bucket_type *_table;
	};

inline symbol_table::ostream_type &
operator<<(symbol_table::ostream_type &s, const symbol_table &d)
	{ return d.display(s); }

#endif // _SYM_TAB_H
