// Verilog Behavioral Simulator
// Copyright (C) 1995 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// testeq.cc

#include <string>
#include "common/event.h"

class Obj
	{
	friend ostream &operator<<(ostream &, const Obj &);
	string str;
public:
	struct expr_type { struct num_type {}; };

	struct thing
		{ typedef bool num_type; }; // For event object.
	typedef thing bit_type; // For event object.

	Obj(const char *s)
		: str(s) {}
	bool sim_trigger(Obj *);
	};

ostream &
operator<<(ostream &s, const Obj &o)
	{ s << o.str; return s; }

bool
Obj::sim_trigger(Obj *)
	{ cout << "Triggering: " << str << endl; return true; }

template<class T>
class ObjHandler : public event_queue_handler<T>
	{
	void operator()(T *obj) const
		{ obj->sim_trigger(obj); }
	};

int
main()
	{
	event_queue<Obj> eq;
	Obj a("abc"), b("bcd");

	event_base<Obj>::event_container *c1 = new
		event_base<Obj>::event_container(&a);
	event_base<Obj>::event_container *c2 = new
		event_base<Obj>::event_container(&b);

	Obj::expr_type::num_type num1;
	Obj::expr_type::num_type num2;
	change_event<Obj> *w = new change_event<Obj>(c1, num1);
	negedge_event<Obj> *x = new negedge_event<Obj>(c1, num2);
	posedge_event<Obj> *y = new posedge_event<Obj>(c2, num2);
	change_event<Obj> *z = new change_event<Obj>(c2, num1);

	eq.add_event(w);
	eq.add_event(x);
	eq.add_event(y);
	eq.add_event(z);
	eq.trigger(ObjHandler<Obj>());

	cout << endl << "We're done" << endl;

	return 0;
	}
