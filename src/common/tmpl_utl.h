// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// tmpl_utl.h
//
// A function object to display list contents.

#ifndef _TMPL_UTL_H
#define _TMPL_UTL_H

#include <iostream>

template<class T>
struct printlist
	{
	typedef std::ostream ostream_type;
	typedef T list_type;
	typedef typename list_type::iterator iterator;

	printlist(list_type *l)
		{ _list = l; }
	ostream_type &operator()(ostream_type &s) const
		{
		iterator itp(_list->begin());
		iterator stop(_list->end());
		for (; itp != stop; ++itp)
			s << (*itp) << endl;
		return s;
		}
	list_type *_list;
	};

template<class T>
struct ptrprintlist
	{
	typedef std::ostream ostream_type;
	typedef T list_type;
	typedef typename list_type::iterator iterator;

	ptrprintlist(list_type *l)
		{ _list = l; }
	ostream_type &operator()(ostream_type &s) const
		{
		if (_list != 0)
			{
			iterator itp(_list->begin());
			iterator stop(_list->end());
			for (; itp != stop; ++itp)
				s << *(*itp) << endl;
			}
		return s;
		}
	list_type *_list;
	};

#endif // _TMPL_UTL_H
