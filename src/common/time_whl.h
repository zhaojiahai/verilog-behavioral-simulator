// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// time_whl.h
//
// Time wheel class.  This class is a template class for the same reason
// as the "Events" class.  See Event.h for an explanation.

#ifndef _TIME_WHL_H
#define _TIME_WHL_H

#include <iterator>
#include "common/debug.h"

using std::cout;
using std::endl;

template<class T> struct time_wheel_handler;

template<class T>
class time_wheel_node
	{
public:
	typedef unsigned long time_type;
	typedef T data_type;
	typedef typename data_type::ostream_type ostream_type;
	typedef typename data_type::template list_type<data_type*> event_list_type;
	typedef typename event_list_type::const_iterator const_iterator;
	typedef time_wheel_handler<data_type*> handler_type;

	explicit time_wheel_node(time_type t = 0)
		: _time(t) {}
	time_wheel_node(const time_wheel_node<T> &tw)
		: _time(tw._time)
		{ append(tw._event); }

	void add_event(T *op)
		{ _event.push_back(op); }
	time_type time() const
		{ return _time; }
	ostream_type &display(ostream_type &s) const
		{
		const_iterator idx(_event.begin());
		const_iterator last(_event.end());
		s << "Time " << _time << ":" << endl;
		for (; idx != last; ++idx)
			(*idx)->display(s);
		return s;
		}
	void trigger(const time_wheel_handler<T> &);

protected:
	void append(const event_list_type &ev)
		{
		std::back_insert_iterator<event_list_type> bii(_event);
		copy(ev.begin(), ev.end(), bii);
		}

private:
	time_type _time; // Current time for this node. Absolute time.
	event_list_type _event; // List of events.
	};

template<class T>
struct time_wheel_handler
	{
	typedef time_wheel_node<T> node_type;
	typedef typename node_type::time_type time_type;
	typedef typename node_type::event_list_type event_list_type;

	virtual ~time_wheel_handler()
		{}

	// User function to perform operations on our object.
	virtual void operator()(time_type, event_list_type &) const
		{ cout << "No time wheel handler, using default!" << endl; }
	};

template<class T> inline void
time_wheel_node<T>::trigger(const time_wheel_handler<T> &h)
	{ h(_time, _event); }


// Time Wheel.  We can not use a list to store the nodes in this case
// because we need to insert nodes.  Unless in the future we extend
// the list class to include sorted lists.


template<class T>
class time_wheel
	{
public:
	typedef T data_type;
	typedef time_wheel_node<data_type> node_type;
	typedef typename node_type::time_type time_type;
	typedef typename node_type::handler_type handler_type;
	typedef typename node_type::ostream_type ostream_type;
	typedef typename data_type::template list_type<node_type> node_list;
	typedef typename node_list::iterator iterator;
	typedef typename node_list::const_iterator const_iterator;

	time_wheel()
		: _latest_time(0)
		// Create time zero, so we can test it later.
		{ _node.push_back(node_type()); }

	void add_event(time_type t, T *obj)
		{
		// Find out where this object belongs.
		iterator idx(_node.begin());
		iterator stop(_node.end());
		for (; idx != stop; idx++)
			{
			if (t == (*idx).time())
				{
				DEBUG_STATE(DEBUG_TIME_WHEEL);
				DEBUG_OUTPUT("DEBUG_TIME_WHEEL:  appending to current time: ");
				DEBUG_OUTPUT((*idx).time());
				DEBUG_OUTPUT("\n");
				(*idx).add_event(obj);
				break;
				}
			else if (t > (*idx).time())
				{
				// Greater than current time unit.
				// Could insert if less than next time unit,
				// or create new node if next time unit doesn't
				// exist.
				iterator next(idx);
				++next;
				if (next != stop)
					{
					// Between this time unit and next time
					// unit, insert.
					if (t < (*next).time())
						{
						DEBUG_STATE(DEBUG_TIME_WHEEL);
						DEBUG_OUTPUT("DEBUG_TIME_WHEEL:  inserting time unit: ");
						DEBUG_OUTPUT(t);
						DEBUG_OUTPUT(" (before time unit ");
						DEBUG_OUTPUT((*next).time());
						DEBUG_OUTPUT(")\n");
						iterator tmp(
							_node.insert(next, node_type(t)));
						(*tmp).add_event(obj);
						break;
						}
					// Greater or equal?  Go there and check!
					}
				else
					{
					DEBUG_STATE(DEBUG_TIME_WHEEL);
					DEBUG_OUTPUT("DEBUG_TIME_WHEEL:  appending to time unit: ");
					DEBUG_OUTPUT(t);
					DEBUG_OUTPUT("\n");
					// Hmm, at the end of our list, append.
					_node.push_back(node_type(t));
					iterator prev(_node.end());
					--prev;
					(*prev).add_event(obj);
					break;
					}
				}
			else
				{
				cout << "Less than current time (Error)?" << endl << endl;
				return;
				}
			}
		}
	void trigger(const time_wheel_handler<T> &h)
		{
		// Trigger our node.
		while (!_node.empty())
			{
			node_type &tmp(_node.front());

			// Update current time unit.
			if (tmp.time() > _latest_time)
				_latest_time = tmp.time();

			tmp.trigger(h);
			_node.pop_front();
			}
		}
	bool trigger_once(const time_wheel_handler<T> &h)
		{
		if (!_node.empty())
			{
			node_type &tmp(_node.front());

			// Update current time unit.
			if (tmp.time() > _latest_time)
				_latest_time = tmp.time();

			tmp.trigger(h);
			_node.pop_front();
			return true;
			}                             
		return false;
		}
	time_type current_time() const
		{ return _latest_time; }
	ostream_type &display(ostream_type &s) const
		{
		const_iterator idx(_node.begin());
		const_iterator last(_node.end());
		for (; idx != last; ++idx)
			(*idx).display(s);
		return s;
		}

private:
	time_type _latest_time;
	node_list _node;
	};

#endif // _TIME_WHL_H
