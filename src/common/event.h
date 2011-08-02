// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// event.h
//
// Event queue class to support "always @" statements.
//
// 96-1-15:  Added support for STL's list container and objectized event
// and trigger types.
//
// 96-2-10:  Fixed support for level change (reduce to one bit).
//
// 96-4-18:  Added support for variable change, renamed above object.

#ifndef _EVENT_H
#define _EVENT_H

#include <list>
#include "common/debug.h"

// TRIG_TYPE is an object to replace the behavior of an enumeration.
// We need special operators for trigger type, but enumerations are
// inefficient (takes too much space).  This object should attempt to
// take the least amount of memory the system can support.

struct TRIG_TYPE
	{
	enum
		{
		TRIG_NONE    = 0x00,
		TRIG_NEGEDGE = 0x01,
		TRIG_POSEDGE = 0x02,
		TRIG_CHNG    = 0x04,
		TRIG_NONBLK  = 0x10,
		TRIG_MONITOR = 0x20,
		TRIG_STROBE  = 0x40
		};

	explicit TRIG_TYPE(int t = TRIG_NONE)
		: _type(t) {}
	TRIG_TYPE(const TRIG_TYPE &t)
		: _type(t._type) {}
	TRIG_TYPE operator=(const TRIG_TYPE &t)
		{ _type = t._type; return *this; }
	operator char() const
		{ return _type; }

	char _type;
	};

const TRIG_TYPE TRIG_NONE(TRIG_TYPE::TRIG_NONE);
const TRIG_TYPE TRIG_NEGEDGE(TRIG_TYPE::TRIG_NEGEDGE);
const TRIG_TYPE TRIG_POSEDGE(TRIG_TYPE::TRIG_POSEDGE);
const TRIG_TYPE TRIG_CHNG(TRIG_TYPE::TRIG_CHNG);
const TRIG_TYPE TRIG_NONBLK(TRIG_TYPE::TRIG_NONBLK);
const TRIG_TYPE TRIG_MONITOR(TRIG_TYPE::TRIG_MONITOR);
const TRIG_TYPE TRIG_STROBE(TRIG_TYPE::TRIG_STROBE);

inline bool
operator==(const TRIG_TYPE l, const TRIG_TYPE r)
	{ return l._type == r._type; }

inline TRIG_TYPE
operator|(const TRIG_TYPE l, const TRIG_TYPE r)
	{ return TRIG_TYPE(l._type | r._type); }

inline TRIG_TYPE
operator&(const TRIG_TYPE l, const TRIG_TYPE r)
	{ return TRIG_TYPE(l._type & r._type); }

// Handler for an event.

template<class T>
struct event_queue_handler
	{
	// The argument to this function is the object in which to
	// perform some task on.  The reference is used, so we do
	// not need to make a copy of T.
	virtual void operator()(T *) const = 0;
	};


// Base class for all event types.  Provide a common interface.

template<class T>
class event_base
	{
	typedef std::ostream ostream_type;
	typedef T content_type;

public:
	template<typename U> struct list_type : public std::list<U> { };
	typedef typename content_type::expr_type bit_type;
	typedef typename content_type::expr_type::num_type num_type;
	typedef TRIG_TYPE trig_type;

	// Event container, holds all data for simulation.
	struct event_container
		{
		event_container(bool f, content_type *t)
			: _cnt(0), _queued(false), _active(false), _force(f), _obj(t)
			{}
		int _cnt; // Number of event with common object.
		bool _queued; // Were we queued already?
		bool _active; // Were we triggered?
		bool _force; // Force re-activation after event trigger.
		content_type *_obj; // Object to trigger.
		};

	event_base(event_container *c, const num_type &ival)
		: _previous_bit(ival), _bit(0)
		{ _cache = c; _cache->_cnt++; }
	virtual ~event_base()
		{
		--(_cache->_cnt);
		if (_cache->_cnt == 0)
			delete _cache;
		}

	// The common interface.
	const num_type &previous_bit() const
		{ return _previous_bit; }
	void previous_bit(const num_type &b)
		{ _previous_bit = b; }
	const bit_type *monitor_bit() const
		{ return _bit; }
	void monitor_bit(bit_type *b)
		{ _bit = b; }
	content_type *data()
		{ return _cache->_obj; }
	void queue()
		{ _cache->_queued = true; }
	void unqueue()
		{ _cache->_queued = false; }
	bool is_queued() const
		{ return (_cache->_queued == true); }
	void activate()
		{ _cache->_active = true; }
	void deactivate()
		{ _cache->_active = false; }
	bool is_active() const
		{ return _cache->_force ? true : _cache->_active; }

	virtual trig_type type() const = 0;
	const char *type_name() const
		{
		switch (type())
			{
			case trig_type::TRIG_NEGEDGE: return "TRIG_NEGEDGE";
			case trig_type::TRIG_POSEDGE: return "TRIG_POSEDGE";
			case trig_type::TRIG_CHNG:    return "TRIG_CHNG";
			case trig_type::TRIG_NONBLK:  return "TRIG_NONBLK";
			case trig_type::TRIG_MONITOR: return "TRIG_MONITOR";
			case trig_type::TRIG_STROBE:  return "TRIG_STROBE";
			}
		return "TRIG_???";
		}

private:
	num_type _previous_bit;
	bit_type *_bit; // Expression being monitored.  Evaluates to one bit.
	event_container *_cache; // Common holding area.
	};

template<class T>
class negedge_event : public event_base<T>
	{
	typedef typename event_base<T>::num_type num_type;
	typedef typename event_base<T>::trig_type trig_type;
	typedef typename event_base<T>::event_container container_type;

public:
	negedge_event(container_type *c, const num_type &ival)
		: event_base<T>(c, ival) {}

	trig_type type() const
		{ return TRIG_NEGEDGE; }
	};

template<class T>
class posedge_event : public event_base<T>
	{
	typedef typename event_base<T>::num_type num_type;
	typedef typename event_base<T>::trig_type trig_type;
	typedef typename event_base<T>::event_container container_type;

public:
	posedge_event(container_type *c, const num_type &ival)
		: event_base<T>(c, ival) {}

	trig_type type() const
		{ return TRIG_POSEDGE; }
	};

template<class T>
class change_event : public event_base<T>
	{
	typedef typename event_base<T>::num_type num_type;
	typedef typename event_base<T>::trig_type trig_type;
	typedef typename event_base<T>::event_container container_type;

public:
	change_event(container_type *c, const num_type &ival)
		: event_base<T>(c, ival) {}

	trig_type type() const
		{ return TRIG_CHNG; }
	};

template<class T>
class nonblock_event : public event_base<T>
	{
	typedef typename event_base<T>::num_type num_type;
	typedef typename event_base<T>::trig_type trig_type;
	typedef typename event_base<T>::event_container container_type;

public:
	nonblock_event(container_type *c, const num_type &ival)
		: event_base<T>(c, ival) {}

	trig_type type() const
		{ return TRIG_NONBLK; }
	};

template<class T>
class monitor_event : public event_base<T>
	{
	typedef typename event_base<T>::num_type num_type;
	typedef typename event_base<T>::trig_type trig_type;
	typedef typename event_base<T>::event_container container_type;

public:
	monitor_event(container_type *c, const num_type &ival)
		: event_base<T>(c, ival) {}

	trig_type type() const
		{ return TRIG_MONITOR; }
	};

template<class T>
class strobe_event : public event_base<T>
	{
	typedef typename event_base<T>::num_type num_type;
	typedef typename event_base<T>::trig_type trig_type;
	typedef typename event_base<T>::event_container container_type;

public:
	strobe_event(container_type *c, const num_type &ival)
		: event_base<T>(c, ival) {}

	trig_type type() const
		{ return TRIG_STROBE; }
	};


// Event queue used for the simulation timing.  The queue consists
// of a list of events (defined above).  A member function is provided
// to trigger all events in the list.  Usually, we want to trigger
// all events queued by the previous time unit.  Maybe I should
// provide simpler primitives for the user to trigger the events
// individually?


template<class T>
class event_queue
	{
public:
	typedef int amount_type;
	typedef event_base<T> event_type;
	typedef event_type *event_ptr;
	typedef typename event_type::trig_type trig_type;
	typedef typename event_type::template list_type<event_ptr> event_list;

	event_queue()
		: _queued_active(0), _queued_inactive(0),
		  _queued_monitor(0), _queued_strobe(0), _triggered_events(0) {}
	void add_event(event_ptr ev)
		{
		// We are about to put the event into our list.
		// We must change the queued status so we do not
		// try to queue it more than once.
		ev->queue();
		// Now append the event into the list.  We need to determine
		// which of the inactive event list this event belongs.  The
		// following events are supported:
		//		assignments (non-blocking and continuous)
		//		strobe
		//		monitor
		if ((ev->type() & TRIG_NONBLK) != TRIG_NONE)
			{
			DEBUG_STATE(DEBUG_EVENT);
			DEBUG_OUTPUT("DEBUG_EVENT:  Queuing TRIG_NONBLK event.\n");
			_inactive.push_back(ev);
			++_queued_inactive;
			}
		else if ((ev->type() & TRIG_MONITOR) != TRIG_NONE)
			{
			DEBUG_STATE(DEBUG_EVENT);
			DEBUG_OUTPUT("DEBUG_EVENT:  Queuing TRIG_MONITOR event.\n");
			_monitor.push_back(ev);
			++_queued_monitor;
			}
		else if ((ev->type() & TRIG_STROBE) != TRIG_NONE)
			{
			DEBUG_STATE(DEBUG_EVENT);
			DEBUG_OUTPUT("DEBUG_EVENT:  Queuing TRIG_STROBE event.\n");
			_strobe.push_back(ev);
			++_queued_strobe;
			}
		else
			{
			DEBUG_STATE(DEBUG_EVENT);
			DEBUG_OUTPUT("DEBUG_EVENT:  Queuing ");
			DEBUG_OUTPUT(ev->type_name());
			DEBUG_OUTPUT(" event.\n");
			_active.push_back(ev);
			++_queued_active;
			}
		}
	void move_events(event_list &to, event_list &from)
		{
		while (!from.empty())
			{
			// Get event to move.
			event_ptr ev = from.front();
			// Reset flag so we can queue this event when we trigger
			// it below.  This allows recursive events in the same
			// simulation time.
			ev->unqueue();
			to.push_back(ev);
			from.pop_front();
			}
		}
	void handle_events(event_list &ev_list, const event_queue_handler<T> &h)
		{
		while (!ev_list.empty())
			{
			// Get event to trigger.
			event_ptr ev = ev_list.front();
			// Trigger it.
			h(ev->data());
			++_triggered_events;
			// Remove node from list.
			ev_list.pop_front();
			}
		}
	void trigger(const event_queue_handler<T> &handler)
		{
		// Trigger events, but do not delete the event object.
		// It is not a copy, the actual event is stored in the
		// symbol table and should not be deleted.
		event_list temp;
		while (!_active.empty() || !_inactive.empty() || !_monitor.empty())
			{
			// Move events out of the queue.  This allows the trigger
			// action to queue more events.

			// The event processing is ordered in such a way that monitor
			// event processing are interleaved between active and inactive
			// events.
			if (!_active.empty())
				{
				DEBUG_STATE(DEBUG_EVENT);
				DEBUG_OUTPUT("DEBUG_EVENT:  Triggering sensitivity event.\n");
				move_events(temp, _active);
				handle_events(temp, handler);
				}
			else if (!_inactive.empty())
				{
				DEBUG_STATE(DEBUG_EVENT);
				DEBUG_OUTPUT("DEBUG_EVENT:  Triggering nonblock event.\n");
				move_events(temp, _inactive);
				handle_events(temp, handler);
				}

			DEBUG_STATE(DEBUG_EVENT);
			DEBUG_OUTPUT("DEBUG_EVENT:  Triggering monitor event.\n");
			move_events(temp, _monitor);
			handle_events(temp, handler);
			}

		// Strobe events are triggered right before time is advanced,
		// this is it...
		DEBUG_STATE(DEBUG_EVENT);
		DEBUG_OUTPUT("DEBUG_EVENT:  Triggering strobe event.\n");
		move_events(temp, _strobe);
		handle_events(temp, handler);
		}
	amount_type queued_events() const
		{ return _queued_active + _queued_inactive
				+ _queued_monitor + _queued_strobe; }
	amount_type triggered_events() const
		{ return _triggered_events; }

private:
	// The following is VBS's interface to the IEEE Std 1364-1995
	// description of the scheduling semantics.
	amount_type _queued_active;
	amount_type _queued_inactive;
	amount_type _queued_monitor;
	amount_type _queued_strobe;
	amount_type _triggered_events;
	event_list _active;
	event_list _inactive;
	event_list _monitor;
	event_list _strobe;
	};

#endif // _EVENT_H
