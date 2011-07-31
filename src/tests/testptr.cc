// Verilog Behavioral Simulator
// Copyright (C) 1995,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// testptr.cc

#include <string>
#include <iostream>
#include "common/ptr.h"

using std::string;
using std::ostream;
using std::cout;
using std::endl;

class Object
	{
	friend ostream &operator<<(ostream &, const Object &);

public:
	Object(const char *s)
		: str(s) {}

	void func(ostream &s) const
		{ s << "func(" << str << ")" << endl; }

private:
	string str;
	};

ostream &
operator<<(ostream &s, const Object &o)
	{ s << o.str; return s; }

int
main()
	{
	basic_ptr<Object> rp(new Object("Hello world!"));
	cout << "rp = " << *rp << endl;

	counted_ptr<Object> cp1(new Object("Hello again world!"));
	cout << "cp1 = " << *cp1 << endl;

	counted_ptr<Object> cp2;
	cp2 = cp1;
	cout << "cp1 = " << *cp1 << endl;
	cout << "cp2 = " << *cp2 << endl;
	cp2->func(cout);

	return 0;
	}
