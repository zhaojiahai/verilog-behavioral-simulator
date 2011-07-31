// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dstack.h
//
// Stack container class using deque as container.  This implementation
// of a stack allows the user to iterate through the contents.  The ANSI
// C++ STL definition does not allow this.

#ifndef _DSTACK_H
#define _DSTACK_H

#include <deque>

template<class T>
class Stack
	{
public:
	typedef T value_type;
	typedef std::deque<value_type> container_type;
	typedef typename container_type::iterator iterator;
	typedef typename container_type::size_type size_type;

	bool empty()      const        { return c.empty(); }
	size_type size()  const        { return c.size(); }
	value_type&       top()        { return c.front(); }
	const value_type& top() const  { return c.front(); }
	void push(const value_type &x) { c.push_front(x); }
	void pop()                     { c.pop_front(); }

	iterator begin()               { return c.begin(); }
	iterator end()                 { return c.end(); }

protected:
	container_type c;
	};

#endif // _DSTACK_H
