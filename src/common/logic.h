// Verilog Behavioral Simulator
// Copyright (C) 1996-1997,2001,2002,2011 Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// logic.h
//
// This object is the basic unit of binary representation (bit).
// It provides all the standard logic operations.  The size of the
// logic type is the same as sizeof(char).  This makes it possible
// to use memcpy/mem* functions to move data around.  By creating
// a class object, we have better type safety than if we simply
// used a typedef.  There are no overhead, since no members are
// virtual (no destructor).

#ifndef _LOGIC_H
#define _LOGIC_H

class logic
	{
	friend bool operator==(const logic, const logic);
	friend bool operator!=(const logic, const logic);
	friend bool operator<(const logic, const logic);
	friend bool operator>(const logic, const logic);
	friend bool operator<=(const logic, const logic);
	friend bool operator>=(const logic, const logic);

public:
	enum state_value { LO, HI, DC, Z, NVL };
	enum edge_type { NOEDGE, POSEDGE, NEGEDGE };

	logic(state_value l = NVL)
		: _state(l)
		{ }
	logic(const logic &l)
		: _state(l._state)
		{ }

	logic &operator=(const logic l)
		{ _state = l._state; return *this; }

	operator bool() const
		{ return (_state == HI) ? true : false; }
	operator int() const
		{ return _state; }
	operator long int() const
		{ return _state; }
	operator char() const
		{
		switch (_state)
			{
			case LO: return '0';
			case HI: return '1';
			case DC: return 'x';
			case  Z: return 'z';
			default: return '.';
			}
		}
private:
	char _state; // Use the smallest type known.

public:
	// Use private heap manager.
	enum { cache_size = 4096 };
	union cache_union
		{
		cache_union *next;
		size_t size;
		};
	static cache_union *cache[cache_size];
	void *operator new[](size_t size)
		{
		void *p;
		cache_union *cu;
		size += sizeof(cache_union);
		if (cache[size] != 0)
			{
			p = cache[size];
			cache[size] = cache[size]->next;
			}
		else
			{
			// Allocate another block of memory.
			p = malloc(size);
			}
		cu = (cache_union *)p;
		cu->size = size;
		p = (char *)p + sizeof(cache_union);
		return p;
		}
	void operator delete[](void *ptr)
		{
		cache_union *tmp;
		ptr = (char *)ptr - sizeof(cache_union);
		cache_union *cu = (cache_union *)ptr;
		tmp = cache[cu->size];
		cache[cu->size] = cu;
		cache[cu->size]->next = tmp;
		}
	};

// Misc. operators.

// Constant values and tables for logic arithmetic.
// Currently, logic operations are through table lookup.  Can this be
// changed?  I want to make the tables static const members of
// logic.  But GCC produces 'multiple defined' link errors.  I guess
// I have to move these out of the header if I want to make them static.
// But that made things complicated, so I'm keeping everything in this
// header.

const logic  LO(logic::LO);
const logic  HI(logic::HI);
const logic  DC(logic::DC);
const logic   Z(logic::Z);
const logic NVL(logic::NVL); // Not Valid logic.

const int NUMLOGIC = 5;

// Logic operations.

#if __GNUC__ == 2 && __GNUC_MINOR__ == 95

extern logic logic_INV[NUMLOGIC];
extern logic logic_AND[][NUMLOGIC];
extern logic logic_NAND[][NUMLOGIC];
extern logic logic_OR[][NUMLOGIC];
extern logic logic_NOR[][NUMLOGIC];
extern logic logic_XOR[][NUMLOGIC];
extern logic logic_XNOR[][NUMLOGIC];
extern logic logic_TERNARY[][NUMLOGIC];

extern logic logic_reduce_AND[][NUMLOGIC];
extern logic logic_reduce_OR[][NUMLOGIC];
extern logic logic_reduce_XOR[][NUMLOGIC];

// Boolean states of logic comparison.

extern logic logic_EQU[][NUMLOGIC];
extern logic logic_EQL[][NUMLOGIC];
extern logic logic_EQC[][NUMLOGIC];
extern logic logic_EQX[][NUMLOGIC];
extern logic logic_EQZ[][NUMLOGIC];

#else

const logic logic_INV[NUMLOGIC] =
	{ HI,LO,DC,DC,DC };

const logic logic_AND[][NUMLOGIC] =
	{ {LO,LO,LO,LO,DC},
	  {LO,HI,DC,DC,DC},
	  {LO,DC,DC,DC,DC},
	  {LO,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

const logic logic_NAND[][NUMLOGIC] =
	{ {HI,HI,HI,HI,DC},
	  {HI,LO,DC,DC,DC},
	  {HI,DC,DC,DC,DC},
	  {HI,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

const logic logic_OR[][NUMLOGIC] =
	{ {LO,HI,DC,DC,DC},
	  {HI,HI,HI,HI,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

const logic logic_NOR[][NUMLOGIC] =
	{ {HI,LO,DC,DC,DC},
	  {LO,LO,LO,LO,DC},
	  {DC,LO,DC,DC,DC},
	  {DC,LO,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

const logic logic_XOR[][NUMLOGIC] =
	{ {LO,HI,DC,DC,DC},
	  {HI,LO,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

const logic logic_XNOR[][NUMLOGIC] =
	{ {HI,LO,DC,DC,DC},
	  {LO,HI,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

const logic logic_TERNARY[][NUMLOGIC] =
	{ {LO,DC,DC,DC,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

const logic logic_reduce_AND[][NUMLOGIC] =
	{ {LO,LO,LO,LO,DC},
	  {LO,HI,DC,DC,DC},
	  {LO,DC,DC,DC,DC},
	  {LO,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	};

const logic logic_reduce_OR[][NUMLOGIC] =
	{ {LO,HI,DC,DC,DC},
	  {HI,HI,HI,HI,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	};

const logic logic_reduce_XOR[][NUMLOGIC] =
	{ {LO,HI,DC,DC,DC},
	  {HI,LO,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	};

// Boolean states of logic comparison.

const logic logic_EQU[][NUMLOGIC] =
	{
	// This table is used with == comparisons.
	  {HI, LO, DC, DC, DC},
	  {LO, HI, DC, DC, DC},
	  {DC, DC, DC, DC, DC},
	  {DC, DC, DC, DC, DC},
	  {DC, DC, DC, DC, DC}
	};

const logic logic_EQL[][NUMLOGIC] =
	{
	// This table is used with < comparisons.
	  { Z, HI, DC, DC, DC},
	  {LO,  Z, DC, DC, DC},
	  {DC, DC, DC, DC, DC},
	  {DC, DC, DC, DC, DC},
	  {DC, DC, DC, DC, DC}
	};

const logic logic_EQC[][NUMLOGIC] =
	{
	// This table is used with the ===/!== comparisons.  According to the
	// IEEE Std 1364-1995, all values must match to result in true.  The
	// only valid result is either 0 or 1.
	  {HI, LO, LO, LO, LO},
	  {LO, HI, LO, LO, LO},
	  {LO, LO, HI, LO, LO},
	  {LO, LO, LO, HI, LO},
	  {LO, LO, LO, LO, HI}
	};

const logic logic_EQX[][NUMLOGIC] =
	{
	// This table is used with casex situations.  According to the IEEE
	// Std 1364-1995, all don't-cares and high-impedance bits are to be
	// ignored when the comparison is evaluated.  This is equivalent to
	// an equal condition.  Thus we can return true for these bit
	// comparisons.
	  {HI, LO, HI, HI, LO},
	  {LO, HI, HI, HI, LO},
	  {HI, HI, HI, HI, LO},
	  {HI, HI, HI, HI, LO},
	  {LO, LO, LO, LO, HI}
	};

const logic logic_EQZ[][NUMLOGIC] =
	{
	// This table is used with casez situations.  According to the IEEE
	// Std 1364-1995, all high-impedance bit values are to be ignored
	// when the comparison is evaluated.  This is equivalent to an equal
	// condition.  Thus we can return true for these bit comparisons.
	  {HI, LO, LO, HI, LO},
	  {LO, HI, LO, HI, LO},
	  {LO, LO, LO, HI, LO},
	  {HI, HI, HI, HI, LO},
	  {LO, LO, LO, LO, HI}
	};

#endif

const logic::edge_type NOEDGE(logic::NOEDGE);
const logic::edge_type POSEDGE(logic::POSEDGE);
const logic::edge_type NEGEDGE(logic::NEGEDGE);

const logic::edge_type logic_DETECT[][NUMLOGIC] =
	{ { NOEDGE,POSEDGE,POSEDGE,POSEDGE, NOEDGE},
	  {NEGEDGE, NOEDGE,NEGEDGE,NEGEDGE, NOEDGE},
	  {NEGEDGE,POSEDGE, NOEDGE, NOEDGE, NOEDGE},
	  {NEGEDGE,POSEDGE, NOEDGE, NOEDGE, NOEDGE},
	  { NOEDGE, NOEDGE, NOEDGE, NOEDGE, NOEDGE}
	};

// Some logic arithmetic operations.

inline bool
operator==(const logic l, const logic r)
	{ return l._state == r._state; }

inline bool
operator!=(const logic l, const logic r)
	{ return l._state != r._state; }

inline bool
operator<(const logic l, const logic r)
	{ return l._state < r._state; }

inline bool
operator>(const logic l, const logic r)
	{ return l._state > r._state; }

inline bool
operator<=(const logic l, const logic r)
	{ return l._state <= r._state; }

inline bool
operator>=(const logic l, const logic r)
	{ return l._state >= r._state; }

inline logic
operator!(const logic l)
	{ return logic_INV[l]; }

inline logic
operator~(const logic l)
	{ return logic_INV[l]; }

inline logic::edge_type
edge_detect(const logic f, const logic t)
	{ return logic_DETECT[f][t]; }

inline logic
reduce_and(const logic c, const logic n)
	{ return logic_reduce_AND[c][n]; }

inline logic
reduce_or(const logic c, const logic n)
	{ return logic_reduce_OR[c][n]; }

inline logic
reduce_xor(const logic c, const logic n)
	{ return logic_reduce_XOR[c][n]; }

#endif // _LOGIC_H
