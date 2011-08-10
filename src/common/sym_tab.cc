// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// sym_tab.cc

#include "common/error.h"
#include "common/sym_tab.h"


symbol_table::symbol_table(size_type s)
						// Last scope number is never global.
	: _size(s), _amount(0), _last_scope_number(1)
	{
	if (s > 0)
		{
		TABLESIZE = _size;
		_table = new bucket_type[_size];
		}
	else
		{
		TABLESIZE = 0;
		_table = 0;
		}
	}

symbol_table::~symbol_table()
	{ delete [] _table; }

symbol_table::ostream_type &
symbol_table::display(ostream_type &s) const
	{
	s << "size(" << _size << ")  amount(" << _amount << ')' << endl;
	for (size_type i = 0; i < _size; ++i)
		{
		if (_table[i].size() == 0)
			continue;
		bucket_type::iterator itp(_table[i].begin());
		bucket_type::iterator last(_table[i].end());
		for (; itp != last; ++itp)
			s << i << ": " << *(*itp) << endl;
		}
	return s;
	}

bool
symbol_table::initialize(size_type siz)
	{
	if (_size == 0)
		{
		_size = siz;
		TABLESIZE = siz;
		_table = new bucket_type[_size];
		return true;
		}
	return false;
	}

symbol_table::scope_type &
symbol_table::scope()
	{ return _last_scope_number; }

symbol_table::hash_type
symbol_table::add(node_type *p, bool init)
	{
	// Do we still have room?  If not, return error.
	if (_size == 0)
		return hash_type(vbs_error::SE_STINIT, -1);
	else if (_amount >= _size)
		return hash_type(vbs_error::SE_STFULL, -1);

	hash_type idx(hash(p->name(), p->scope(), init));
	node_type *tmp;
	size_type i = 1;
	while (i < _size)
		{
		tmp = get(idx);
		if (tmp == 0)
			{
			if (_table[idx._value].size() == 0)
				++_amount; // Used another bucket.
			_table[idx._value].push_back(p);
			break;
			}

		// Collision occured!
		if (tmp->name() == p->name())
			// When the scope, hash value, and the symbol
			// are the same, we have a duplicate!
			return hash_type(vbs_error::SE_STDUP, -1);

		// Not a duplicate, find another bucket.
		rehash(idx, ++i);
		}
	return idx;
	}

void
symbol_table::remove(const hash_type &idx)
	{
	if (idx._scope < 0 || _size == 0)
		return;

	bucket_type &bukt = _table[idx._value];
	bucket_type::iterator itp(bukt.begin());
	bucket_type::iterator last(bukt.end());
	for (; itp != last; ++itp)
		{
		if ((*itp)->scope() == idx._scope &&
			(!idx._test || (idx._test && (*itp)->get_instantiation() != 0)))
			{
			bukt.erase(itp);
			break;
			}
		}
	}

symbol_table::hash_type
symbol_table::find(const str_type &n, scopelist_type &sl, bool init)
	{
	// Are there any elements yet?  If not, return invalid element.
	if (_size == 0)
		return hash_type(vbs_error::SE_STINIT);
	else if (_amount == 0)
		return hash_type(vbs_error::SE_STDEF);

	hash_type idx, idxorig(hash(n, -1, init));
	node_type *node;
	// Search for nodes that are in the scope only.  I.e. global
	// scope might not be included.
	scopelist_type::iterator itp(sl.begin());
	scopelist_type::iterator stop(sl.end());
	for (; itp != stop; ++itp)
		{
		// Try each scope in turn.
		idx._test = idxorig._test;
		idx._value = idxorig._value;
		idx._scope = *itp;
		size_type i = 1;
		while (i < _size)
			{
			node = get(idx);
			// Is this node taken at all?
			if (node == 0) // Not in this scope, try next.
				break;
			else if (node->name() == n)
				return idx;
			// Collision, but not what we're looking for.
			rehash(idx, ++i);
			}
		}

	// Well, I guess it's not in the symbol table.
	return hash_type(vbs_error::SE_STDEF);
	}

symbol_table::node_type *
symbol_table::get(const hash_type &idx)
	{
	if (idx._scope < 0 || _size == 0)
		return 0;

	bucket_type &bukt = _table[idx._value];
	bucket_type::iterator itp(bukt.begin());
	bucket_type::iterator last(bukt.end());
	for (; itp != last; ++itp)
		{
		// Each bucket is a list of symbols with different scope.
		// So traverse the list and find the scope we're looking
		// for.  Duplicates are checked when the symbol was first
		// entered into the table.
		if ((*itp)->scope() == idx._scope &&
			(!idx._test || (idx._test && (*itp)->get_instantiation() != 0)))
			return (*itp).get();
		}
	return 0;
	}
