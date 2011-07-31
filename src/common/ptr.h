// Verilog Behavioral Simulator
// Copyright (C) 1996-1997 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// ptr.h
//
// Templates for smart pointers.  This file contains two types of
// smart pointers.
//
// 1.  Basic type pointers.  These pointers are general purpose smart
// pointers.
//
// 2.  Counted type pointers.  These pointers contain a common object
// type.  Each type a new pointer points to the common object, a count
// is incremented.  When the count is zero, the common object is removed.

#ifndef _PTR_H
#define _PTR_H

template<class T>
class basic_ptr
	{
public:
	basic_ptr(T *p = 0)
		{ _ptr = p; }
	basic_ptr(const basic_ptr<T> &bp)
		{ _ptr = /*const_cast<basic_ptr<T> &>*/
			// Unsafe, but needed.
			((basic_ptr<T> &)bp).release(); }
	~basic_ptr()
		{ delete _ptr; }

	basic_ptr<T> &operator=(const basic_ptr<T> &bp)
		{
		delete _ptr;
		_ptr = /*const_cast<basic_ptr<T> &>*/
			((basic_ptr<T> &)bp).release();
		return *this;
		}
	T *operator->() const
		{ return get(); }
	T &operator*() const
		{ return *_ptr; }
	operator bool() const
		{ return (_ptr != 0); }

	T *release()
		{ T *tmp = _ptr; _ptr = 0; return tmp; }
	T *get() const
		{ return _ptr; }
private:
	T *_ptr;
	};


template<class T>
class counted_ptr
	{
	typedef unsigned long amount_type;

	struct container
		{
		amount_type _count;
		basic_ptr<T> _ptr;
		container(T *o)
			: _ptr(o) { _count = 1; }
		};

public:
	counted_ptr(T *o = 0)
		{
		if (o != 0)
			_cache = new container(o);
		else
			_cache = 0;
		}
	counted_ptr(const counted_ptr<T> &cp)
		{
		_cache = cp._cache;
		if (_cache != 0)
			gaining_owner();
		}
	~counted_ptr()
		{ if (_cache != 0) losing_owner(); }

	counted_ptr<T> &operator=(const counted_ptr<T> &cp)
		{
		if (_cache != 0)
			losing_owner();
		_cache = cp._cache;
		if (_cache != 0)
			gaining_owner();
		return *this;
		}
	T *operator->() const
		{ return get(); }
	T &operator*() const
		{ return (_cache != 0) ? _cache->_ptr.operator*() : *(T *)0; }

	T *release()
		{
		T *tmp = 0;
		if (_cache != 0)
			{
			tmp = _cache->_ptr.get();
			losing_owner();
			_cache = 0;
			}
		return tmp;
		}
	T *get() const
		{ return (_cache != 0) ? _cache->_ptr.get() : 0; }
private:
	void losing_owner()
		{
		--(_cache->_count);
		if (_cache->_count == 0)
			{
			delete _cache;
			_cache = 0;
			}
		}
	void gaining_owner() const
		{ ++(_cache->_count); }

	container *_cache;
	};

#endif //_POINTER_H
