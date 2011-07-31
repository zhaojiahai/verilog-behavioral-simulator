// Verilog Behavioral Simulator
// Copyright (C) 1995,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// testtw.cc

#include <list>
#include <string>
#include <iostream>
#include "common/time_whl.h"

using std::list;
using std::string;
using std::ostream;
using std::cout;
using std::endl;

class Obj
	{
public:
	template <typename T> struct list_type : public std::list<T> { };
	typedef ostream ostream_type;

	Obj(char *);
	ostream_type &display(ostream_type&) const;
	bool sim_trigger(Obj *);

private:
	string str;
	};

template<class T>
class twhandler : public time_wheel_handler<T>
	{
	typedef time_wheel_handler<T>::time_type time_type;
	typedef time_wheel_handler<T>::event_list_type event_list_type;
	typedef typename event_list_type::iterator iterator;

	void operator()(time_type t, event_list_type &lst) const
		{
		iterator itp(lst.begin());
		iterator stop(lst.end());
		cout << "Time " << t << ':' << endl;
		for (; itp != stop; ++itp)
			(*itp)->sim_trigger(*itp);
		}
	};

time_wheel<Obj> timewheel;

int
main()
	{
	Obj a("abc"), b("bcd"), c("cde");

	timewheel.add_event(0, &a);
	timewheel.add_event(15, &b);
	timewheel.add_event(20, &c);
	timewheel.display(cout);
	twhandler<Obj> x;
	timewheel.trigger(x);
	cout << "We're done" << endl;

	return 0;
	}


Obj::Obj(char *s)
	: str(s) {}

Obj::ostream_type &
Obj::display(ostream_type &s) const
	{ s << str << endl; return s; }

bool
Obj::sim_trigger(Obj *)
	{
	cout << "At " << timewheel.current_time()
		<< " triggering: " << str << endl;
	return true;
	}
