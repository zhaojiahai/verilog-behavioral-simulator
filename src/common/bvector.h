// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// bvector.h
//
// Bit vector class.  Needed to represent don't cares in our bit vector.
// This object is a general purpose number object.  All arithmetic and
// logic operations are done with bit_vectors.  All number representations
// in a Verilog description will be represented by a bit_vector.
//
// Things to note:
// (1)  <begin> and <end> are inclusive indices.  This means they are
// the actual beginning and ending indices.
// (2)  The basic logical unit is the logic type.  See logic.h.
// (3)  Logic bits are internally kept least significant bit to
//      most significant bit.  Thus, a conversion is needed for
//      declarations where the ls_exp is larger than the ms_exp.
// (4)  When a decimal string (char *) is passed as input to the
//      constructor, the number must be less than <dec_t> size.
//      Otherwise, we can't store the temporary number.

#ifndef _BVECTOR_H
#define _BVECTOR_H

#include <cstring>
#include <string>
#include <utility>
#include <iostream>
#include "common/logic.h"

class bit_vector
	{
public:
	// Size of a decimal number.  This should be set to the largest integer
	// size that the machine can handle.
#ifndef DECTYPE
# if defined(__GNUC__) && !defined(__STDC__)
	typedef unsigned long long int decimal_type;
# else
	typedef unsigned long int decimal_type;
# endif
#else
	typedef DECTYPE decimal_type;
#endif

	// The base type is used in conversion functions.
	enum base_type
		{
		BASE2 = 2,
		BASE8 = 8,
		BASE10 = 10,
		BASE16 = 16,
		BASESTR = -1,
		BASEDFLT = -2
		};

	// Size type is used as a magnitude quantity.
	typedef unsigned long size_type;

	// Amount is like size, except it can be negative.
	typedef signed long amount_type;

	// Position type is used for locations/indices.  We allow a negative
	// location for error detection.  I.e. out of range situations.
	typedef signed long position_type;

	// This is our basic logic type.
	typedef logic logic_type;

	// Retype some library types.
	typedef std::string str_type;
	typedef std::ostream ostream_type;

	// A partial bit vector representation.  Used for range assignments
	// or references.
	class const_sub_bit_vector; // Forward declaration...
	class sub_bit_vector
		{
		// Give bit_vector access to constructor.
		friend class bit_vector;
		friend class bit_vector::const_sub_bit_vector;
		friend ostream_type &operator<<(ostream_type &, const sub_bit_vector &);

		bit_vector &_BV; // The original bit vector we are part of.
		position_type _begin; // Starting position.
		position_type _end; // Stopping position.
		size_type _size; // Size of this sub-bit vector.

		// Only bit_vector can create a sub_bit_vector object.
		sub_bit_vector(bit_vector &, const position_type, const position_type);
	public:
		sub_bit_vector(const sub_bit_vector &);
		sub_bit_vector &operator=(const sub_bit_vector &);
		sub_bit_vector &operator=(const const_sub_bit_vector &);
		sub_bit_vector &operator=(const bit_vector &);
		size_type size() const
			{ return _size; }
		ostream_type &info(ostream_type &) const;

		// We don't need any other method, yet!
		};
	class const_sub_bit_vector
		{
		// Give bit_vector access to constructor.
		friend class bit_vector;
		friend class bit_vector::sub_bit_vector;
		friend ostream_type &operator<<(ostream_type &, const const_sub_bit_vector &);

		const bit_vector &_BV; // The original bit vector we are part of.
		position_type _begin; // Starting position.
		position_type _end; // Stopping position.
		size_type _size; // Size of this sub-bit vector.

		// Only bit_vector can create a sub_bit_vector object.
		const_sub_bit_vector(const bit_vector &, const position_type, const position_type);
	public:
		const_sub_bit_vector(const const_sub_bit_vector &);
		size_type size() const
			{ return _size; }
		ostream_type &info(ostream_type &) const;

		// We don't need any other method, yet!
		};

	// These friends are prototyped here because they need the above
	// types to be declared.
	friend class bit_vector::sub_bit_vector;
	friend class bit_vector::const_sub_bit_vector;
	friend logic_type compare_forward(const bit_vector &, const bit_vector &,
		logic_type, logic_type, const logic [][NUMLOGIC]);
	friend logic_type compare_backward(const bit_vector &, const bit_vector &,
		logic_type, logic_type, const logic [][NUMLOGIC]);
	friend logic_type operator==(const bit_vector &l, const bit_vector &r)
		{ return compare_forward(l, r, HI, HI, logic_EQU); }
	friend logic_type operator!=(const bit_vector &l, const bit_vector &r)
		{ return !(l == r); }
	friend logic_type operator<(const bit_vector &l, const bit_vector &r)
		{ return compare_backward(l, r, LO, Z, logic_LES); }
	friend logic_type operator>(const bit_vector &l, const bit_vector &r)
		{ return r < l; }
	friend logic_type operator<=(const bit_vector &l, const bit_vector &r)
		{ return !(r < l); }
	friend logic_type operator>=(const bit_vector &l, const bit_vector &r)
		{ return !(l < r); }
	friend logic_type compare_c(const bit_vector &l, const bit_vector &r)
		{ return compare_forward(l, r, HI, HI, logic_EQC); } // case.
	friend logic_type compare_x(const bit_vector &l, const bit_vector &r)
		{ return compare_forward(l, r, HI, HI, logic_EQX); } // casex.
	friend logic_type compare_z(const bit_vector &l, const bit_vector &r)
		{ return compare_forward(l, r, HI, HI, logic_EQZ); } // casez.
	friend logic_type reduce_and(const bit_vector &);
	friend logic_type reduce_or(const bit_vector &);
	friend logic_type reduce_xor(const bit_vector &);
	friend logic_type reduce_nand(const bit_vector &bv)
		{ return ~reduce_and(bv); }
	friend logic_type reduce_nor(const bit_vector &bv)
		{ return ~reduce_or(bv); }
	friend logic_type reduce_xnor(const bit_vector &bv)
		{ return ~reduce_xor(bv); }
	friend logic_type operator!(const bit_vector &);
	friend bit_vector &unary_neg(bit_vector &, const bit_vector &);
	friend bit_vector &unary_inv(bit_vector &, const bit_vector &);
	friend bit_vector &binary_add(bit_vector &, const bit_vector &, const bit_vector &);
	friend bit_vector &binary_sub(bit_vector &, const bit_vector &, const bit_vector &);
	friend bit_vector &binary_mul(bit_vector &, const bit_vector &, const bit_vector &);
	friend bit_vector &binary_div(bit_vector &, const bit_vector &, const bit_vector &);
	friend bit_vector &binary_mod(bit_vector &, const bit_vector &, const bit_vector &);
	friend bit_vector &binary_lshf(bit_vector &, const bit_vector &, const bit_vector &);
	friend bit_vector &binary_rshf(bit_vector &, const bit_vector &, const bit_vector &);
	friend bit_vector &logic_op(bit_vector &, const bit_vector &, const bit_vector &, const logic [][NUMLOGIC]);
	friend bit_vector &logic_and(bit_vector &bv, const bit_vector &l, const bit_vector &r)
		{ return logic_op(bv, l, r, logic_AND); }
	friend bit_vector &logic_nand(bit_vector &bv, const bit_vector &l, const bit_vector &r)
		{ return logic_op(bv, l, r, logic_NAND); }
	friend bit_vector &logic_or(bit_vector &bv, const bit_vector &l, const bit_vector &r)
		{ return logic_op(bv, l, r, logic_OR); }
	friend bit_vector &logic_nor(bit_vector &bv, const bit_vector &l, const bit_vector &r)
		{ return logic_op(bv, l, r, logic_NOR); }
	friend bit_vector &logic_xor(bit_vector &bv, const bit_vector &l, const bit_vector &r)
		{ return logic_op(bv, l, r, logic_XOR); }
	friend bit_vector &logic_xnor(bit_vector &bv, const bit_vector &l, const bit_vector &r)
		{ return logic_op(bv, l, r, logic_XNOR); }
	friend bit_vector &ternary_cmp(bit_vector &bv, const bit_vector &l, const bit_vector &r)
		{ return logic_op(bv, l, r, logic_TERNARY); }
	friend ostream_type &operator<<(ostream_type &, const bit_vector &);
	friend ostream_type &operator<<(ostream_type &, const sub_bit_vector &);
	friend ostream_type &operator<<(ostream_type &, const const_sub_bit_vector &);

	// Invalid position.
	static position_type npos;

	bit_vector(logic_type = NVL);
	bit_vector(decimal_type);
	bit_vector(position_type, position_type, base_type = BASE10, bool = false, logic_type = DC);
	bit_vector(const str_type &, base_type = BASE2, size_type = 0, bool = false);
	bit_vector(const bit_vector &);
	virtual ~bit_vector();

	// Implicit converters.
	operator bool() const;
	operator unsigned long() const;

	// Public operators.
	bit_vector &operator=(const bit_vector &);
	bit_vector &operator=(const sub_bit_vector &);
	bit_vector &operator=(const const_sub_bit_vector &);
	logic_type &operator()(position_type);
	const logic_type operator()(position_type) const;
	sub_bit_vector operator()(position_type, position_type);
	const_sub_bit_vector operator()(position_type, position_type) const;

protected:
	// Multiply/divide optimization.
	bit_vector &operator+=(const bit_vector &);
	bit_vector &operator-=(const bit_vector &);
	bit_vector &operator<<=(unsigned long);
	bit_vector &operator>>=(unsigned long);

	// Used by to_string to do the work for decimal case.
	const str_type binary_2_string(size_type = 0) const;
	const str_type binary_2_decimal(size_type = 0) const;

	// Other utility functions.
	int get_bits_as_int(position_type &, amount_type) const;
	bool assign_bits(position_type &,
		logic_type, logic_type, logic_type, logic_type = NVL);
	void sign_extend(position_type);
	void assign_from_bin(const str_type &);
	void assign_from_oct(const str_type &);
	void assign_from_dec(const str_type &);
	void assign_from_hex(const str_type &);

private:
	// Verilog allows both:
	//	reg [16:0] a;
	//	reg [0:15] b;
	// Therefore, we must keep track of which was used.  Msb is lower
	// for the second case.  Note:  this is only used in range access.
	// I.e. internal bit storage is from least to most significant bit.
	// So arithmetic and logic operations are not affected.  This is
	// mostly used for error checking.
	bool _msb_lower;
	bool _tristate;
	bool _signed;
	base_type _base;
	position_type _begin;
	position_type _end;
	size_type _size;
	logic_type *_bits;

public:
	bool msb_lower() const
		{ return _msb_lower; }
	position_type start_pos() const
		{ return _begin; }
	position_type stop_pos() const
		{ return _end; }
	size_type size() const
		{ return _size; }

	// Tri-state support.
	bool is_tristate() const
		{ return _tristate; }
	void clr_tristate()
		{ _tristate = false; }
	void set_tristate()
		{
		_tristate = true;
		for (position_type i = _begin; i <= _end; ++i)
			{
			if (_bits[i] != Z)
				{
				_tristate = false;
				break;
				}
			}
		}

	// Public helper functions.
	const str_type to_string(base_type = BASEDFLT, size_type = 0) const;
	ostream_type &info(ostream_type &) const;
	};

inline
bit_vector::sub_bit_vector::sub_bit_vector(bit_vector &bv,
		const position_type ms, const position_type ls)
	: _BV(bv)
	{
	// According to the Verilog LRM, <ms> and <ls> must be non-negative
	// and <ms> must reference a more significant bit than <ls>
	if (bv._msb_lower == false && ms >= ls)
		{
		_begin = ls;
		_end = ms;
		}
	else if (bv._msb_lower == true && ms <= ls)
		{
		// When <msb_lower> is true, it means this register was
		// declared with [0:7].  Thus, when range selecting, we
		// must convert to the internal bit_vector representation.
		// This only affects range selection, and not logic or
		// arithmetic operations.
		position_type tmp = bv._end + bv._begin;
		_begin = tmp - ls;
		_end = tmp - ms;
		}
	_size = (_end - _begin) + 1;
	}

inline
bit_vector::sub_bit_vector::sub_bit_vector(const sub_bit_vector &sbv)
	: _BV(sbv._BV), _begin(sbv._begin), _end(sbv._end), _size(sbv._size)
	{}

inline bit_vector::sub_bit_vector &
bit_vector::sub_bit_vector::operator=(const sub_bit_vector &sbv)
	{
	// Try to copy over what we need, only.
	position_type i = _begin;
	position_type j = sbv._begin;
	for (; i <= _end && i <= _BV._end; ++i, ++j)
		{
		// Did we go past the end of <sbv>?  I.e <sbv._size> < <_size>
		if (j > sbv._end)
			break;
		// If we are unused, don't modify it.
		else if (_BV._bits[i] == NVL)
			continue;
		// Do not copy over unused bits.  Make them DCs in our
		// copy.  Is this the correct behavior?
		if (sbv._BV._bits[j] == NVL)
			_BV._bits[i] = DC;
		else
			_BV._bits[i] = sbv._BV._bits[j];
		}
	// Did we get everything we asked for?
	if (i <= _end && i <= _BV._end)
		memset(_BV._bits + i, static_cast<int>(LO), _end - i + 1);
	return *this;
	}

inline bit_vector::sub_bit_vector &
bit_vector::sub_bit_vector::operator=(const const_sub_bit_vector &sbv)
	{
	// Try to copy over what we need, only.
	position_type i = _begin;
	position_type j = sbv._begin;
	for (; i <= _end && i <= _BV._end; ++i, ++j)
		{
		// Did we go past the end of <sbv>?  I.e <sbv._size> < <_size>
		if (j > sbv._end)
			break;
		// If we are unused, don't modify it.
		else if (_BV._bits[i] == NVL)
			continue;
		// Do not copy over unused bits.  Make them DCs in our
		// copy.  Is this the correct behavior?
		if (sbv._BV._bits[j] == NVL)
			_BV._bits[i] = DC;
		else
			_BV._bits[i] = sbv._BV._bits[j];
		}
	// Did we get everything we asked for?
	if (i <= _end && i <= _BV._end)
		memset(_BV._bits + i, static_cast<int>(LO), _end - i + 1);
	return *this;
	}

inline bit_vector::sub_bit_vector &
bit_vector::sub_bit_vector::operator=(const bit_vector &bv)
	{
	// Exactly like the above.
	position_type i = _begin;
	position_type j = bv._begin;
	for (; i <= _end && i <= _BV._end; ++i, ++j)
		{
		if (j > bv._end)
			break;
		else if (_BV._bits[i] == NVL)
			continue;
		if (bv._bits[j] == NVL)
			_BV._bits[i] = DC;
		else
			_BV._bits[i] = bv._bits[j];
		}
	if (i <= _end && i <= _BV._end)
		memset(_BV._bits + i, static_cast<int>(LO), _end - i + 1);
	return *this;
	}

inline bit_vector::ostream_type &
bit_vector::sub_bit_vector::info(bit_vector::ostream_type &s) const
	{
	if (_BV._msb_lower)
		s << "BV[" << _begin << ":" << _end << "](" << _size << ")";
	else
		s << "BV[" << _end << ":" << _begin << "](" << _size << ")";
	return s;
	}

inline bit_vector::ostream_type &
operator<<(bit_vector::ostream_type &s, const bit_vector::sub_bit_vector &sbv)
	{
	// Since bits are kept least significant to most significant.
	// We do not need to reverse the order.
	bit_vector::position_type i = sbv._end;
	for (; i >= sbv._begin; --i)
		s << static_cast<char>(sbv._BV._bits[i]);
	return s;
	}

inline
bit_vector::const_sub_bit_vector::const_sub_bit_vector(const bit_vector &bv,
		const position_type ms, const position_type ls)
	: _BV(bv)
	{
	// According to the Verilog LRM, <ms> and <ls> must be non-negative
	// and <ms> must reference a more significant bit than <ls>
	if (bv._msb_lower == false && ms >= ls)
		{
		_begin = ls;
		_end = ms;
		}
	else if (bv._msb_lower == true && ms <= ls)
		{
		// When <msb_lower> is true, it means this register was
		// declared with [0:7].  Thus, when range selecting, we
		// must convert to the internal bit_vector representation.
		// This only affects range selection, and not logic or
		// arithmetic operations.
		position_type tmp = bv._end + bv._begin;
		_begin = tmp - ls;
		_end = tmp - ms;
		}
	_size = (_end - _begin) + 1;
	}

inline
bit_vector::const_sub_bit_vector::const_sub_bit_vector(const const_sub_bit_vector &sbv)
	: _BV(sbv._BV), _begin(sbv._begin), _end(sbv._end), _size(sbv._size)
	{}

inline bit_vector::ostream_type &
bit_vector::const_sub_bit_vector::info(bit_vector::ostream_type &s) const
	{
	if (_BV._msb_lower)
		s << "BV[" << _begin << ":" << _end << "](" << _size << ")";
	else
		s << "BV[" << _end << ":" << _begin << "](" << _size << ")";
	return s;
	}

inline bit_vector::ostream_type &
operator<<(bit_vector::ostream_type &s, const bit_vector::const_sub_bit_vector &sbv)
	{
	// Since bits are kept least significant to most significant.
	// We do not need to reverse the order.
	bit_vector::position_type i = sbv._end;
	for (; i >= sbv._begin; --i)
		s << static_cast<char>(sbv._BV._bits[i]);
	return s;
	}

inline
bit_vector::bit_vector(logic_type b)
	: _msb_lower(false), _tristate(false), _signed(false), _base(BASE2)
	{
	// The default constructor.  If no default logic value is given,
	// then we assume the caller wants a invalid bit vector.
	if (b == NVL)
		{
		_begin = -1;
		_end = -2;
		_size = 0;
		_bits = 0;
		}
	else
		{
		_begin = _end = 0;
		_size = 1;
		_bits = new logic_type[1];
		_bits[0] = b;
		}
	}

inline
bit_vector::bit_vector(position_type ms, position_type ls, base_type b, bool neg, logic_type val)
	: _tristate(false), _signed(neg), _base(b)
	{
	// Create a bit vector with the specified size.
	// According to the Verilog LRM, <ls> can be greater than
	// <ms>.  Thus, we must be careful about the <begin>/<end>.
	if (ms < ls)
		{
		_msb_lower = true;
		_begin = ms;
		_end = ls;
		}
	else
		{
		_msb_lower = false;
		_begin = ls;
		_end = ms;
		}
	_size = (_end - _begin) + 1;
	// If we begin offset from zero, we need to setup the unused bits.
	_bits = new logic_type[_end + 1]; // <_begin> + <_size>!
	// Initialize array to <val>, default is don't-care.
	memset(_bits, static_cast<int>(NVL), _begin);
	memset(_bits + _begin, static_cast<int>(val), _size);
	}

inline
bit_vector::bit_vector(const bit_vector &bv)
	: _msb_lower(bv._msb_lower), _tristate(false), _signed(bv._signed), _base(bv._base),
	_begin(bv._begin), _end(bv._end), _size(bv._size)
	{
	// Copy constructor, make an exact copy.
	if (_size > 0)
		{
		// For
		//	reg [8:1] a;
		// _size does not show the actual length.
		_bits = new logic_type[_end + 1];
		memcpy(_bits, bv._bits, _end + 1);
		}
	else
		{
		// Nothing to copy.
		_bits = 0;
		}
	}

inline
bit_vector::~bit_vector()
	{
	// Free allocated memory.
	delete [] _bits;
	}

inline bit_vector &
bit_vector::operator=(const bit_vector &bv)
	{
	// Assignment is not like copy constructor.  Assignment only wants
	// enough data to fit in our array.  If the right-hand-side is
	// larger, we will truncate it.  Else if it's smaller, we prepend
	// with zeros.  In the special case that we are invalid, then copy
	// everything.
	if (bv._size == 0)
		{
		// Has no effect.
		}
	else if (_size == 0)
		{
		// If we were nothing before, copy everything over now.
		_msb_lower = bv._msb_lower;
		_begin = bv._begin;
		_end = bv._end;
		_size = bv._size;
		_bits = new logic_type[_end + 1];
		memcpy(_bits, bv._bits, _end + 1);
		}
	else
		{
		logic_type *src = bv._bits + bv._begin;
		logic_type *dst = _bits + _begin;
		size_type i = bv._size;
		if (i < _size)
			{
			memcpy(dst, src, i);
			if (dst[i - 1] == DC)
				memset(dst + i, static_cast<int>(DC), _size - i);
			else
				memset(dst + i, static_cast<int>(LO), _size - i);
			}
		else
			{
			// Copy only what we need.
			memcpy(dst, src, _size);
			}
		}
	return *this;
	}

inline bit_vector &
bit_vector::operator=(const sub_bit_vector &sbv)
	{
	// Same as above.
	if (sbv._size == 0)
		{
		// Has no effect.
		}
	else if (_size == 0)
		{
		_msb_lower = sbv._BV._msb_lower;
		_begin = sbv._begin;
		_end = sbv._end;
		_size = sbv._size;
		_bits = new logic_type[_end + 1];
		memcpy(_bits, sbv._BV._bits, _end + 1);
		}
	else
		{
		logic_type *src = sbv._BV._bits + sbv._begin;
		logic_type *dst = _bits + _begin;
		size_type i = sbv._size;
		if (sbv._begin > sbv._BV._end || sbv._end < sbv._BV._begin)
			{
			// Access beyond the end.
			memset(dst, static_cast<int>(DC), _size);
			}
		else if (i < _size)
			{
			memcpy(dst, src, i);
			if (dst[i - 1] == DC)
				memset(dst + i, static_cast<int>(DC), _size - i);
			else
				memset(dst + i, static_cast<int>(LO), _size - i);
			}
		else
			{
			// Copy only what we need.
			memcpy(dst, src, _size);
			}
		}
	return *this;
	}

inline bit_vector &
bit_vector::operator=(const const_sub_bit_vector &sbv)
	{
	// Same as above.
	if (sbv._size == 0)
		{
		// Has no effect.
		}
	else if (_size == 0)
		{
		_msb_lower = sbv._BV._msb_lower;
		_begin = sbv._begin;
		_end = sbv._end;
		_size = sbv._size;
		_bits = new logic_type[_end + 1];
		memcpy(_bits, sbv._BV._bits, _end + 1);
		}
	else
		{
		logic_type *src = sbv._BV._bits + sbv._begin;
		logic_type *dst = _bits + _begin;
		size_type i = sbv._size;
		if (sbv._begin > sbv._BV._end || sbv._end < sbv._BV._begin)
			{
			// Access beyond the end.
			memset(dst, static_cast<int>(DC), _size);
			}
		else if (i < _size)
			{
			memcpy(dst, src, i);
			if (dst[i - 1] == DC)
				memset(dst + i, static_cast<int>(DC), _size - i);
			else
				memset(dst + i, static_cast<int>(LO), _size - i);
			}
		else
			{
			// Copy only what we need.
			memcpy(dst, src, _size);
			}
		}
	return *this;
	}

inline bit_vector::logic_type &
bit_vector::operator()(position_type idx)
	{
	// This operator is used by external functions, thus it has test
	// conditions to make sure there are no out of range accesses.
	static logic_type errbit = DC;
	// If <idx> is out of range.  We must return a DC.
	if (idx < _begin || idx > _end)
		return (errbit = DC);
	if (_msb_lower == true)
		return _bits[_end - idx];
	return _bits[idx];
	}

inline const bit_vector::logic_type
bit_vector::operator()(position_type idx) const
	{
	// This operator is used by external functions, thus it has test
	// conditions to make sure there are no out of range accesses.
	// If <idx> is out of range.  We must return a DC.
	if (idx < _begin || idx > _end)
		return DC;
	if (_msb_lower == true)
		return _bits[_end - idx];
	return _bits[idx];
	}

inline bit_vector::sub_bit_vector
bit_vector::operator()(position_type ms, position_type ls)
	{
	// Even though <ms> and <ls> must be non-negative and <ms> greater
	// than <ls>, we must return something.  So we let the constructor
	// do the testing and return whatever the constructor constructed.
	return sub_bit_vector(*this, ms, ls);
	}

inline bit_vector::const_sub_bit_vector
bit_vector::operator()(position_type ms, position_type ls) const
	{
	// Even though <ms> and <ls> must be non-negative and <ms> greater
	// than <ls>, we must return something.  So we let the constructor
	// do the testing and return whatever the constructor constructed.
	return const_sub_bit_vector(*this, ms, ls);
	}

#endif // _BVECTOR_H
