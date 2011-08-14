// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// bvector.cc

#include <cerrno> // errno
#include <cstdlib> // strtoul()
#include "common/bvector.h"

#if __GNUC__ == 2 && __GNUC_MINOR__ == 95

logic logic_INV[NUMLOGIC] =
	{ HI,LO,DC,DC,DC };

logic logic_AND[][NUMLOGIC] =
	{ {LO,LO,LO,LO,DC},
	  {LO,HI,DC,DC,DC},
	  {LO,DC,DC,DC,DC},
	  {LO,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

logic logic_NAND[][NUMLOGIC] =
	{ {HI,HI,HI,HI,DC},
	  {HI,LO,DC,DC,DC},
	  {HI,DC,DC,DC,DC},
	  {HI,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

logic logic_OR[][NUMLOGIC] =
	{ {LO,HI,DC,DC,DC},
	  {HI,HI,HI,HI,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

logic logic_NOR[][NUMLOGIC] =
	{ {HI,LO,DC,DC,DC},
	  {LO,LO,LO,LO,DC},
	  {DC,LO,DC,DC,DC},
	  {DC,LO,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

logic logic_XOR[][NUMLOGIC] =
	{ {LO,HI,DC,DC,DC},
	  {HI,LO,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

logic logic_XNOR[][NUMLOGIC] =
	{ {HI,LO,DC,DC,DC},
	  {LO,HI,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

logic logic_TERNARY[][NUMLOGIC] =
	{ {LO,DC,DC,DC,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC}
	};

logic logic_reduce_AND[][NUMLOGIC] =
	{ {LO,LO,LO,LO,DC},
	  {LO,HI,DC,DC,DC},
	  {LO,DC,DC,DC,DC},
	  {LO,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	};

logic logic_reduce_OR[][NUMLOGIC] =
	{ {LO,HI,DC,DC,DC},
	  {HI,HI,HI,HI,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,HI,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	};

logic logic_reduce_XOR[][NUMLOGIC] =
	{ {LO,HI,DC,DC,DC},
	  {HI,LO,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	  {DC,DC,DC,DC,DC},
	};

// Boolean states of logic comparison.

logic logic_EQU[][NUMLOGIC] =
	{
	// This table is used with == comparisons.
	  {HI, LO, DC, DC, DC},
	  {LO, HI, DC, DC, DC},
	  {DC, DC, DC, DC, DC},
	  {DC, DC, DC, DC, DC},
	  {DC, DC, DC, DC, DC}
	};

logic logic_LES[][NUMLOGIC] =
	{
	// This table is used with < comparisons.
	  { Z, HI, DC, DC, DC},
	  {LO,  Z, DC, DC, DC},
	  {DC, DC, DC, DC, DC},
	  {DC, DC, DC, DC, DC},
	  {DC, DC, DC, DC, DC}
	};

logic logic_EQC[][NUMLOGIC] =
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

logic logic_EQX[][NUMLOGIC] =
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

logic logic_EQZ[][NUMLOGIC] =
	{
	// This table is used with casez situations.  According to the IEEE
	// Std 1364-1995, all high-impedance bit values are to be ignored
	// when the comparison is evaluated.  This is equivalent to an equal
	// condition.  Thus we can return true for these bit comparisons.
	  {HI, LO, LO, HI, LO},
	  {LO, HI, LO, HI, LO},
	  {LO, LO, HI, HI, LO},
	  {HI, HI, HI, HI, LO},
	  {LO, LO, LO, LO, HI}
	};

#endif

// Value representing "pass the end" position.
bit_vector::position_type bit_vector::npos = (position_type) -1;

// Optimized heap management.
logic::cache_union *logic::cache[logic::cache_size];


// Local functions, used only in this file.


static bit_vector::amount_type
find_hw(bit_vector::decimal_type val, bit_vector::decimal_type &wt)
	{
	// Find the highest weight of <val>.  Return the number of digits
	// in <wt>.
	wt = 1;
	bit_vector::amount_type i = 1;
	for (; (val/wt) >= 10; ++i)
		wt *= 10;
	return i;
	}


// Public member functions for bit_vector class.

bit_vector::bit_vector(decimal_type value)
	: _msb_lower(false), _tristate(false), _signed(UNSIGNED), _base(BASE10), _begin(0)
	{
	// Automatic conversion from integer.
	if (value == 0)
		{
		// A bit_vector of 0(base10), is a bit of value 0.
		_end = 0;
		_size = 1;
		_bits = new logic_type[1];
		_bits[0] = LO;
		return;
		}

	// <_bits> might be larger than necessary, but not too much of a waste.
	position_type largest = 8 * sizeof(decimal_type);
	decimal_type rem = value;
	position_type i = 0;
	_bits = new logic_type[largest];
	while (rem > 0)
		{
		// Now convert integer to binary.
		_bits[i++] = (rem & 1) ? HI : LO;
		rem >>= 1;
		}
	_end = i - 1;
	_size = (_end - _begin) + 1;
	memset(_bits + i, static_cast<int>(NVL), largest - i);
	}

bit_vector::bit_vector(const str_type &str, signed_type s, base_type base, size_type len)
	: _msb_lower(false), _tristate(false), _signed(s), _base(base), _begin(0)
	{
	// Set our bit vector from the base specified in <base>, with
	// the value contained in <str>.  <len> is optional, and set
	// to the needed size.
	// Determine our length.
	bool trunc = false;
	if (len == 0)
		{
		int underscores = 0;
		len = str.length();
		for (size_type i = 0; i < len; ++i)
			{
			if (str[i] == '_')
				++underscores;
			}
		len -= underscores;

		// Convert to bit size.
		switch(base)
			{
			case 2: break;
			case 8: len *= 3; break;
			case -2: // BASEDFLT
			case 10:
				{
				// No size specified.  We need to find the
				// smallest size that will hold the specified
				// value closest to 32bit.
				char *err = 0;
				errno = 0;
				decimal_type value = strtoul(str.c_str(), &err, 10);
				if (*err != '\0' || (value == ULONG_MAX && errno == ERANGE))
					{
					_end = 0;
					_size = 0;
					_bits = 0;
					return;
					}
				if (value > 0)
					{
					int amt = 8 * sizeof(decimal_type);
					if (amt > 32)
						{
						decimal_type tmp = value;
						int num0 = 1;
						if ((tmp >> 32) == 0) { num0 += 32; tmp <<= 32; }
						if ((tmp >> 48) == 0) { num0 += 16; tmp <<= 16; }
						if ((tmp >> 56) == 0) { num0 +=  8; tmp <<=  8; }
						if ((tmp >> 60) == 0) { num0 +=  4; tmp <<=  4; }
						if ((tmp >> 62) == 0) { num0 +=  2; tmp <<=  2; }
						num0 -= tmp >> 63;
						len = num0 < 32 ? 64 - num0 : 32;
						if (len > 32 && value > 0xffffffff)
							trunc = true;
						}
					}
				break;
				}
			case 16: len *= 4; break;
			default: break;
			}

		// Unsized constants must be at least 32bits.
		if (len < 32)
			len = 32;
		}
	_end = len - 1;
	_size = len;
	_bits = new logic_type[len];
	if (str.data() != 0)
		{
		// Now, convert to binary.
		switch(base)
			{
			case 2: assign_from_bin(str); break;
			case 8: assign_from_oct(str); break;
			case -2: // BASEDFLT
			case 10: assign_from_dec(str, trunc); break;
			case 16: assign_from_hex(str); break;
			default: break;
			}
		}
	else
		{
		// No initial value, clear to dont-care.
		memset(_bits, static_cast<int>(DC), len);
		}
	}

bit_vector::operator bool() const
	{
	position_type i = _begin;
	logic_type *data = _bits;
	bool found_true = false;
	for (; i <= _end; ++i)
		{
		if (data[i] == HI)
			found_true = true;
		else if (data[i] > HI)
			return false;
		}
	return found_true;
	}


// Protected member functions of bit_vector class.


inline bit_vector &
bit_vector::operator+=(const bit_vector &bv)
	{
	typedef bit_vector::logic_type logic_type;
	typedef bit_vector::position_type position_type;

	// Addition using ourselves as the result.  If we don't have enough
	// bits to hold the carryout bit, it is lost.
	int pad = 0;
	logic_type L, R;
	logic_type Ci = LO;
	position_type i = _begin;
	position_type li = _begin;
	position_type ri = bv._begin;
	logic_type *data = _bits;
	logic_type *bv_data = bv._bits;
	for (; i <= _end; ++i, ++li, ++ri)
		{
		L = R = NVL;
		// Have we reached the end of one of our vectors?
		if (ri > bv._end)
			{
			// If so, are there still something to add?
			if (Ci == LO)
				break;
			else
				R = LO;
			}
		else
			{
			R = bv_data[ri];
			if (R > HI)
				{
				// DC or Z found.  Set the rest of the result to DC's.
				memset(data+i, static_cast<int>(DC), (_end+1)-i);
				break;
				}
			}
		L = data[li];
		if (L > HI)
			{
			// DC or Z found.  Set the rest of the result to DC's.
			memset(data+i, static_cast<int>(DC), (_end+1)-i);
			break;
			}
		//  Table for adding two bit vectors:
		//  Ci L R | + | Sum Co
		//  ---------------
		//   0 0 0 | 0 |  0  0
		//   0 0 1 | 1 |  1  0
		//   0 1 0 | 1 |  1  0
		//   0 1 1 | 2 |  0  1
		//   1 0 0 | 1 |  1  0
		//   1 0 1 | 2 |  0  1
		//   1 1 0 | 2 |  0  1
		//   1 1 1 | 3 |  1  1
		pad = static_cast<int>(Ci) + static_cast<int>(L) + static_cast<int>(R);
		data[i] = static_cast<logic_type::state_value>(pad & 1);
		Ci = static_cast<logic_type::state_value>(pad >> 1);
		}
	return *this;
	}

inline bit_vector &
bit_vector::operator-=(const bit_vector &bv)
	{
	typedef bit_vector::logic_type logic_type;
	typedef bit_vector::position_type position_type;

	// Subtraction using ourselves as the result.  If we don't have enough
	// bits to hold the borrowin bit, it is lost.
	int pad = 0;
	logic_type L, R;
	logic_type Bi = LO;
	position_type i = _begin;
	position_type li = _begin;
	position_type ri = bv._begin;
	logic_type *data = _bits;
	logic_type *bv_data = bv._bits;
	for (; i <= _end; ++i, ++li, ++ri)
		{
		L = R = NVL;
		// Have we reached the end of one of our vectors?
		if (ri > bv._end)
			R = LO;
		else
			{
			R = bv_data[ri];
			if (R > HI)
				{
				// DC or Z found.  Set the rest of the result to DC's.
				memset(data+i, static_cast<int>(DC), (_end+1)-i);
				break;
				}
			}
		L = data[li];
		if (L > HI)
			{
			// DC or Z found.  Set the rest of the result to DC's.
			memset(data+i, static_cast<int>(DC), (_end+1)-i);
			break;
			}
		//  Table for subtracting two bit vectors:
		//  Bi L R |  - | Dif Bo
		//  ---------------
		//   0 0 0 |  0 | 0  0
		//   0 0 1 | -1 | 1  1
		//   0 1 0 |  1 | 1  0
		//   0 1 1 |  0 | 0  0
		//   1 0 0 | -1 | 1  1
		//   1 0 1 | -2 | 0  1
		//   1 1 0 |  0 | 0  0
		//   1 1 1 | -1 | 1  1
		pad = (static_cast<int>(L) - static_cast<int>(R) - static_cast<int>(Bi)) & 3;
		data[i] = static_cast<logic_type::state_value>(pad & 1);
		Bi = static_cast<logic_type::state_value>(pad >> 1);
		}
	return *this;
	}

inline bit_vector &
bit_vector::operator<<=(unsigned long amt)
	{
	if (amt == static_cast<unsigned long>(-1))
		memset(_bits+_begin, static_cast<int>(DC), _size);
	else if (amt < _size)
		{
		memmove(_bits+_begin + amt, _bits+_begin, _size - amt);
		memset(_bits+_begin, static_cast<int>(LO), amt);
		}
	else
		memset(_bits+_begin, static_cast<int>(LO), _size);
	return *this;
	}

inline bit_vector &
bit_vector::operator>>=(unsigned long amt)
	{
	if (amt == static_cast<unsigned long>(-1))
		memset(_bits+_begin, static_cast<int>(DC), _size);
	else if (amt < _size)
		{
		memmove(_bits+_begin, _bits+_begin+amt, _size - amt);
		memset(_bits+_begin+amt+1, static_cast<int>(LO), amt);
		}
	else
		memset(_bits+_begin, static_cast<int>(LO), _size);
	return *this;
	}

const bit_vector::str_type
bit_vector::binary_2_string(size_type len) const
	{
	char letter;
	int tmp;
	position_type i = _begin;
	size_type s = (_size + 7) / 8;
	str_type res(s, ' ');
	for (int idx = s - 1; i < _end; --idx)
		{
		tmp = get_bits_as_int(i, 4);
		if (tmp > 16)
			{
			res[idx] = '?';
			i += 4;
			continue;
			}
		letter = tmp;
		tmp = get_bits_as_int(i, 4);
		if (tmp > 16)
			{
			res[idx] = '?';
			continue;
			}
		letter |= tmp << 4;
		if (letter != 0)
			res[idx] = letter;
		}
	return res;
	}

const bit_vector::str_type
bit_vector::binary_2_decimal(size_type len) const
	{
	bit_vector tmp(*this);
	position_type j = tmp._begin;
	bool neg = _signed == bit_vector::SIGNED && _bits[_end] == HI;
	if (neg)
		{
		// The conversion below uses unsigned number.  So take
		// the 2's complement first, then prepend sign after.
		bit_vector one(HI);
		bit_vector inv(_end, _begin, UNSIGNED);
		unary_inv(inv, *this);
		binary_add(tmp, inv, one);
		}
	position_type numbits = 8 * sizeof(decimal_type);
	if (static_cast<size_type>(numbits) >= _size)
		numbits = _end;
	else
		numbits = (_begin + numbits) - 1;
	bool have_x, have_z, have_n;
	logic_type b;
	decimal_type wt = 0, num = 0;
	have_x = have_z = have_n = false;
	for (int i = 0; j <= numbits; ++i, ++j)
		{
		b = tmp._bits[j];
		if (b == DC)
			have_x = true;
		else if (b == Z)
			have_z = true;
		else if (b == HI)
			{
			num |= static_cast<decimal_type>(1) << i;
			have_n = true;
			}
		}
	int err = (have_n ? 0x1 : 0) | (have_x ? 0x2 : 0) | (have_z ? 0x4 : 0);
	if (err > 1)
		{
		char ch;
		switch (err)
			{
			case 0x2: ch = 'x'; break;
			case 0x4: ch = 'z'; break;
			case 0x5: ch = 'Z'; break;
			default: ch = 'X'; break;
			}
		return str_type(1, ch);
		}

	// Convert decimal to string.
	size_type size = 0;
	if (len == 0)
		size = find_hw(num, wt);
	else if (static_cast<signed long>(len) < 0)
		{
		decimal_type lnum = static_cast<decimal_type>(-1);
		size = find_hw(lnum, wt);
		}
	else
		size = len;
	if (size == 0)
		{
		size = 1;
		neg = false;
		}
	else
		size += neg;
	str_type res(size, ' ');
	position_type i = size - 1;
	decimal_type digit = 0;
	for (; i != npos; --i)
		{
		digit = num % 10; // Get lowest digit.
		num /= 10; // Remove lowest digit.
		res[i] = digit + '0'; // Convert to ASCII.
		if (num <= 0)
			break;
		}
	if (neg)
		{
		str_type::reverse_iterator b = res.rbegin();
		str_type::reverse_iterator e = res.rend();
		for (; b != e; ++b)
			{
			if (*b == ' ')
				{
				*b = '-';
				break;
				}
			}
		}
	return res;
	}

int BITS2INT[4][5] =
	{
	{ 0, 1, 100, 1000, 10000 },
	{ 0, 2, 100, 1000, 10000 },
	{ 0, 4, 100, 1000, 10000 },
	{ 0, 8, 100, 1000, 10000 },
	};

int
bit_vector::get_bits_as_int(position_type &idx, amount_type numbits) const
	{
	// Get the amount of bits specified by <numbits> and return the
	// integer equivalent.  <idx> should be in range.  Return -1 if
	// any of the bits are out of range.
	int ret = 0;

	// Make sure we have enough.
	position_type remaining = (_end + 1) - idx;
	if (remaining > numbits)
		remaining = numbits;

	switch (remaining)
		{
		case 4:
			ret += BITS2INT[3][_bits[idx+3]];
			// no break
		case 3:
			ret += BITS2INT[2][_bits[idx+2]];
			// no break
		case 2:
			ret += BITS2INT[1][_bits[idx+1]];
			// no break
		case 1:
			ret += BITS2INT[0][_bits[idx+0]];
			// no break
		}
	idx += remaining;
	return ret;
	}

bool
bit_vector::assign_bits(position_type &idx,
		logic_type A, logic_type B, logic_type C, logic_type D)
	{
	// Assign the provided bits to our bit vector.  <D> is an optional
	// parameter.
	position_type remaining = (_end + 1) - idx;
	if (D != NVL)
		{
		if (remaining > 4)
			remaining = 4;
		}
	else
		{
		if (remaining > 3)
			remaining = 3;
		}

	switch (remaining)
		{
		case 4:
			_bits[idx+3] = D;
			// no break
		case 3:
			_bits[idx+2] = C;
			// no break
		case 2:
			_bits[idx+1] = B;
			// no break
		case 1:
			_bits[idx+0] = A;
			break;
		default:
			return false;
		}
	idx += remaining;
	return true;
	}

void
bit_vector::sign_extend(position_type last)
	{
	// Sign extend our bit vector.  Last must be within range.
	size_type len = _end - last;
	if (len > 0)
		{
		logic_type hb = _bits[last]; // High bit.
		memset(_bits+last, static_cast<int>(hb), len);
		}
	}

void
bit_vector::assign_from_bin(const str_type &str)
	{
	// Set our bit vector from a binary string.
	position_type i, j = str.length() - 1;
	for (i = _begin; i <= _end && j != npos; ++i, --j)
		{
		switch(str[j])
			{
			case '0': _bits[i] = LO; break;
			case '1': _bits[i] = HI; break;
			case 'x':
			case 'X': _bits[i] = DC; break;
			case '?':
			case 'z':
			case 'Z': _bits[i] = Z; break;
			case '_': --i; break; // Skip this character.
			default: return;
			}
		}

	// Left padding, if necessary.
	for (; i <= _end; ++i)
		{
		switch(str[0])
			{
			case 'x':
			case 'X': _bits[i] = DC; break;
			case '?':
			case 'z':
			case 'Z': _bits[i] = Z; break;
			default: _bits[i] = LO; break;
			}
		}
	}

void
bit_vector::assign_from_oct(const str_type &str)
	{
	// Set our bit vector from an octal string.
	position_type i, j = str.length() - 1;
	for (i = _begin; i <= _end && j != npos; --j)
		{
		switch(str[j])
			{
			case '0': if (!assign_bits(i, LO, LO, LO)) return; break;
			case '1': if (!assign_bits(i, HI, LO, LO)) return; break;
			case '2': if (!assign_bits(i, LO, HI, LO)) return; break;
			case '3': if (!assign_bits(i, HI, HI, LO)) return; break;
			case '4': if (!assign_bits(i, LO, LO, HI)) return; break;
			case '5': if (!assign_bits(i, HI, LO, HI)) return; break;
			case '6': if (!assign_bits(i, LO, HI, HI)) return; break;
			case '7': if (!assign_bits(i, HI, HI, HI)) return; break;
			case 'x':
			case 'X': if (!assign_bits(i, DC, DC, DC)) return; break;
			case '?':
			case 'z':
			case 'Z': if (!assign_bits(i, Z, Z, Z)) return; break;
			case '_': break;
			default: return;
			}
		}

	// Left padding, if necessary.
	for (; i <= _end; ++i)
		{
		switch(str[0])
			{
			case 'x':
			case 'X': _bits[i] = DC; break;
			case '?':
			case 'z':
			case 'Z': _bits[i] = Z; break;
			default: _bits[i] = LO; break;
			}
		}
	}

void
bit_vector::assign_from_dec(const str_type &str, bool trunc)
	{
	// Optimize zero.  Errors have already been checked.
	decimal_type value = strtoul(str.c_str(), 0, 10);
	if (value == 0)
		{
		memset(_bits, static_cast<int>(LO), _size);
		_base = BASE10; // In case base wasn't specified.
		return;
		}

	// Perform conversion.
	// There are two forms of representing
	// decimal numbers 1234 or 'd1234.  In the former, we truncate
	// and sign extend.  In the later, we truncate but don't
	// sign extend.
	position_type i = _begin;
	decimal_type rem;
	if (trunc)
		{
		rem = value & 0xffffffff;
		if (_base == BASEDFLT)
			{
			decimal_type hbset = static_cast<decimal_type>(1) << 31;
			rem = (rem ^ hbset) - hbset;
			}
		}
	else
		rem = value;
	while (i <= _end)
		{
		// Now convert integer to binary.
		_bits[i++] = (rem & 1) ? HI : LO;
		rem >>= 1;
		}
	_base = BASE10; // In case base wasn't specified.
	}

void
bit_vector::assign_from_hex(const str_type &str)
	{
	// Set our bit vector from a hexadecimal string.
	position_type i, j = str.length() - 1;
	for (i = _begin; i <= _end && j != npos; --j)
		{
		switch(str[j])
			{
			case '0': if (!assign_bits(i, LO, LO, LO, LO)) return; break;
			case '1': if (!assign_bits(i, HI, LO, LO, LO)) return; break;
			case '2': if (!assign_bits(i, LO, HI, LO, LO)) return; break;
			case '3': if (!assign_bits(i, HI, HI, LO, LO)) return; break;
			case '4': if (!assign_bits(i, LO, LO, HI, LO)) return; break;
			case '5': if (!assign_bits(i, HI, LO, HI, LO)) return; break;
			case '6': if (!assign_bits(i, LO, HI, HI, LO)) return; break;
			case '7': if (!assign_bits(i, HI, HI, HI, LO)) return; break;
			case '8': if (!assign_bits(i, LO, LO, LO, HI)) return; break;
			case '9': if (!assign_bits(i, HI, LO, LO, HI)) return; break;
			case 'a':
			case 'A': if (!assign_bits(i, LO, HI, LO, HI)) return; break;
			case 'b':
			case 'B': if (!assign_bits(i, HI, HI, LO, HI)) return; break;
			case 'c':
			case 'C': if (!assign_bits(i, LO, LO, HI, HI)) return; break;
			case 'd':
			case 'D': if (!assign_bits(i, HI, LO, HI, HI)) return; break;
			case 'e':
			case 'E': if (!assign_bits(i, LO, HI, HI, HI)) return; break;
			case 'f':
			case 'F': if (!assign_bits(i, HI, HI, HI, HI)) return; break;
			case 'x':
			case 'X': if (!assign_bits(i, DC, DC, DC, DC)) return; break;
			case '?':
			case 'z':
			case 'Z': if (!assign_bits(i, Z, Z, Z, Z)) return; break;
			case '_': break;
			default: return;
			}
		}

	// Left padding, if necessary.
	for (; i <= _end; ++i)
		{
		switch(str[0])
			{
			case 'x':
			case 'X': _bits[i] = DC; break;
			case '?':
			case 'z':
			case 'Z': _bits[i] = Z; break;
			default: _bits[i] = LO; break;
			}
		}
	}


// Convenient friend functions for bit_vector class.


bit_vector::logic_type
compare_forward(const bit_vector &l, const bit_vector &r,
				bit_vector::logic_type dft, bit_vector::logic_type chk,
				const logic cmp[][NUMLOGIC], bit_vector::logic_type hb)
	{
	bit_vector::position_type li = l._begin;
	bit_vector::position_type ri = r._begin;
	bit_vector::logic_type res;
	if (l._size <= r._size)
		{
		for (; li <= l._end; ++li, ++ri)
			{
			res = cmp[l._bits[li]][r._bits[ri]];
			if (res != chk)
				return res;
			}
		for (; ri <= r._end; ++ri)
			{
			res = cmp[hb][r._bits[ri]];
			if (res != chk)
				return res;
			}
		}
	else
		{
		for (; ri <= r._end; ++li, ++ri)
			{
			res = cmp[l._bits[li]][r._bits[ri]];
			if (res != chk)
				return res;
			}
		for (; li <= l._end; ++li)
			{
			res = cmp[l._bits[li]][hb];
			if (res != chk)
				return res;
			}
		}
	return dft;
	}

bit_vector::logic_type
compare_backward(const bit_vector &l, const bit_vector &r,
				 bit_vector::logic_type dft, bit_vector::logic_type chk,
				 const logic cmp[][NUMLOGIC])
	{
	bit_vector::position_type li = l._end;
	bit_vector::position_type ri = r._end;
	bit_vector::logic_type res;

	// See if we're comparing 2's complement numbers.
	if (l._signed == bit_vector::SIGNED && l._bits[l._end] == HI)
		{
		if (r._signed == bit_vector::UNSIGNED ||
		   (r._signed == bit_vector::SIGNED && r._bits[r._end] == LO))
			{
			// Now make sure there are no DC/Z.
			while (li >= l._begin || ri >= r._begin)
				{
				if (li >= l._begin)
					{
					if (l._bits[li] > HI)
						return DC;
					else
						--li;
					}
				if (ri >= r._begin)
					{
					if (r._bits[ri] > HI)
						return DC;
					else
						--ri;
					}
				}
			return HI;
			}
		}
	else if (r._signed == bit_vector::SIGNED && r._bits[r._end] == HI)
		{
		if (l._signed == bit_vector::UNSIGNED ||
		   (l._signed == bit_vector::SIGNED && l._bits[l._end] == LO))
			{
			// Now make sure there are no DC/Z.
			while (li >= l._begin || ri >= r._begin)
				{
				if (li >= l._begin)
					{
					if (l._bits[li] > HI)
						return DC;
					else
						--li;
					}
				if (ri >= r._begin)
					{
					if (r._bits[ri] > HI)
						return DC;
					else
						--ri;
					}
				}
			return LO;
			}
		}

	if (l._size <= r._size)
		{
		li += r._size - l._size;
		for (; li > l._end; --li, --ri)
			{
			res = cmp[LO][r._bits[ri]];
			if (res != chk)
				return res;
			}
		for (; li >= l._begin; --li, --ri)
			{
			res = cmp[l._bits[li]][r._bits[ri]];
			if (res != chk)
				return res;
			}
		}
	else
		{
		ri += l._size - r._size;
		for (; ri > r._end; --li, --ri)
			{
			res = cmp[l._bits[li]][LO];
			if (res != chk)
				return res;
			}
		for (; ri >= r._begin; --li, --ri)
			{
			res = cmp[l._bits[li]][r._bits[ri]];
			if (res != chk)
				return res;
			}
		}
	return dft;
	}

bit_vector::logic_type
reduce_and(const bit_vector &bv)
	{
	bit_vector::logic_type *data = bv._bits;

	if (bv._size < 1)
		return DC;
	else if (bv._size < 2)
		return data[bv._begin] < DC ? data[bv._begin] : DC;

	bit_vector::position_type i = bv._begin;
	bit_vector::logic_type res = reduce_and(data[i], data[i+1]);
	for (i += 2; i <= bv._end; ++i)
		res = reduce_and(res, data[i]);
	return res;
	}

bit_vector::logic_type
reduce_or(const bit_vector &bv)
	{
	bit_vector::logic_type *data = bv._bits;

	if (bv._size < 1)
		return DC;
	else if (bv._size < 2)
		return data[bv._begin] < DC ? data[bv._begin] : DC;

	bit_vector::position_type i = bv._begin;
	bit_vector::logic_type res = reduce_or(data[i], data[i+1]);
	for (i += 2; i <= bv._end; ++i)
		res = reduce_or(res, data[i]);
	return res;
	}

bit_vector::logic_type
reduce_xor(const bit_vector &bv)
	{
	bit_vector::logic_type *data = bv._bits;

	if (bv._size < 1)
		return DC;
	else if (bv._size < 2)
		return data[bv._begin] < DC ? data[bv._begin] : DC;

	bit_vector::position_type i = bv._begin;
	bit_vector::logic_type res = reduce_xor(data[i], data[i+1]);
	for (i += 2; i <= bv._end; ++i)
		res = reduce_xor(res, data[i]);
	return res;
	}

bit_vector::logic_type
operator!(const bit_vector &bv)
	{
	bit_vector::position_type i = bv._begin;
	bit_vector::logic_type *data = bv._bits;
	bit_vector::logic_type retval = HI;
	for (; i <= bv._end; ++i)
		{
		if (data[i] == HI)
			retval = LO;
		else if (data[i] > HI)
			return DC;
		}
	return retval;
	}

bit_vector &
unary_neg(bit_vector &res, const bit_vector &bv)
	{
	static const bit_vector one(HI);
	unary_inv(res, bv);
	res += one;
	return res;
	}

bit_vector &
unary_inv(bit_vector &res, const bit_vector &bv)
	{
	bit_vector::position_type i = bv._begin;
	bit_vector::position_type j = res._begin;
	for (; i <= bv._end && j <= res._end; ++i, ++j)
		res._bits[j] = ~(bv._bits[i]);
	for (; j <= res._end; ++j)
		res._bits[j] = ~(LO);
	return res;
	}

bit_vector &
binary_add(bit_vector &res, const bit_vector &l, const bit_vector &r)
	{
	typedef bit_vector::logic_type logic_type;
	typedef bit_vector::position_type position_type;

	// The result of an addition supports the carryout bit.  But
	// if the user doesn't save this bit, it is destroyed upon
	// call to destructor.
	int pad = 0;
	logic_type L, R;
	logic_type Ci = LO;
	position_type i = res._begin;
	position_type li = l._begin;
	position_type ri = r._begin;
	logic_type *data = res._bits;
	logic_type *l_data = l._bits;
	logic_type *r_data = r._bits;
	for (; i <= res._end; ++i, ++li, ++ri)
		{
		L = R = NVL;
		// Have we reached the end of one of our vectors?
		if (ri > r._end)
			{
			// If so, are there still something to add?
			if (Ci == LO)
				{
				if (l._end > r._end)
					memcpy(data+i, l_data+li, (l._end+1)-li);
				else
					memset(data+i, static_cast<int>(LO), (res._end+1)-i);
				break;
				}
			else
				R = LO;
			}
		else
			{
			R = r_data[ri];
			if (R > HI)
				{
				// DC or Z found.  Set entire result to DC's.
				memset(res._bits+res._begin, static_cast<int>(DC), res._size);
				break;
				}
			}
		if (li > l._end)
			{
			if (Ci == LO)
				{
				if (r._end > l._end)
					memcpy(data+i, r_data+ri, (r._end+1)-ri);
				else
					memset(data+i, static_cast<int>(LO), (res._end+1)-i);
				break;
				}
			else
				L = LO;
			}
		else
			{
			L = l_data[li];
			if (L > HI)
				{
				// DC or Z found.  Set entire result to DC's.
				memset(res._bits+res._begin, static_cast<int>(DC), res._size);
				break;
				}
			}
		pad = static_cast<int>(Ci) + static_cast<int>(L) + static_cast<int>(R);
		data[i] = static_cast<logic_type::state_value>(pad & 1);
		Ci = static_cast<logic_type::state_value>(pad >> 1);
		}
	if (res._signed == bit_vector::UNSET)
		res._signed = l._signed == bit_vector::SIGNED ||
					  r._signed == bit_vector::SIGNED ?
					  bit_vector::SIGNED : bit_vector::UNSIGNED;
	return res;
	}

bit_vector &
binary_sub(bit_vector &res, const bit_vector &l, const bit_vector &r)
	{
	typedef bit_vector::logic_type logic_type;
	typedef bit_vector::position_type position_type;

	// The result of a subtraction supports the borrowin bit.  But
	// if the user doesn't save this bit, it is destroyed upon
	// call to destructor.
	int pad = 0;
	logic_type L, R;
	logic_type Bi = LO;
	position_type i = res._begin;
	position_type li = l._begin;
	position_type ri = r._begin;
	logic_type *data = res._bits;
	logic_type *r_data = r._bits;
	logic_type *l_data = l._bits;
	for (; i <= res._end; ++i, ++li, ++ri)
		{
		L = R = NVL;
		// Have we reached the end of one of our vectors?
		if (ri > r._end)
			R = LO;
		else
			{
			R = r_data[ri];
			if (R > HI)
				{
				// DC or Z found.  Set entire result to DC's.
				memset(res._bits+res._begin, static_cast<int>(DC), res._size);
				break;
				}
			}
		if (li > l._end)
			L = LO;
		else
			{
			L = l_data[li];
			if (L > HI)
				{
				// DC or Z found.  Set entire result to DC's.
				memset(res._bits+res._begin, static_cast<int>(DC), res._size);
				break;
				}
			}
		pad = (static_cast<int>(L) - static_cast<int>(R) - static_cast<int>(Bi)) & 3;
		data[i] = static_cast<logic_type::state_value>(pad & 1);
		Bi = static_cast<logic_type::state_value>(pad >> 1);
		}
	if (res._signed == bit_vector::UNSET)
		res._signed = l._signed == bit_vector::SIGNED ||
					  r._signed == bit_vector::SIGNED ?
					  bit_vector::SIGNED : bit_vector::UNSIGNED;
	return res;
	}

bit_vector &
binary_mul(bit_vector &p, const bit_vector &l, const bit_vector &r)
	{
	typedef bit_vector::decimal_type decimal_type;
	typedef bit_vector::position_type position_type;
	bit_vector nan(p._end, p._begin, bit_vector::UNSET, bit_vector::BASE10, DC);

	position_type i;
	bool l_signed = l._signed == bit_vector::SIGNED;
	bool r_signed = r._signed == bit_vector::SIGNED;
	bool l_neg = l_signed && l._bits[l._end] == HI;
	bool r_neg = r_signed && r._bits[r._end] == HI;

	// Optimization: for values resulting in a product that can fit
	// in a decimal_type, we use built-in arithmetic.  The
	// multiplication is done with unsigned numbers.
	bool lfail = false;
	decimal_type ld = l.to_decimal_type(&lfail);
	if (lfail && ld == 0)
		return p = nan;
	bool rfail = false;
	decimal_type rd = r.to_decimal_type(&rfail);
	if (rfail && rd == 0)
		return p = nan;
	const unsigned half_size = 8 * sizeof(decimal_type) / 2;
	decimal_type max_dec = (static_cast<decimal_type>(1) << half_size) - 1;
	decimal_type lud = l_neg ? (~ld + 1) : ld;
	decimal_type rud = r_neg ? (~rd + 1) : rd;
	if ((!lfail && lud <= max_dec) &&
		(!rfail && rud <= max_dec))
		{
		if (l_neg ^ r_neg)
			p = -(lud * rud);
		else
			p = lud * rud;
		return p;
		}

	// Multiplication using shift and add algorithm.
	bit_vector one(HI);
	bit_vector mplier, mcand(p._end, p._begin, bit_vector::UNSIGNED);
	p = LO;
	if (l_neg)
		{
		bit_vector tmp(l._end, l._begin, bit_vector::UNSIGNED);
		unary_inv(tmp, l);
		binary_add(mcand, tmp, one);
		}
	else
		mcand = l;
	if (r_neg)
		{
		bit_vector tmp(r._end, r._begin, bit_vector::UNSIGNED);
		unary_inv(tmp, r);
		binary_add(mplier, tmp, one);
		}
	else
		mplier = r;
	for (i = mplier._begin; i <= mplier._end; ++i)
		{
		if (mplier._bits[i] == HI)
			p += mcand;
		mcand <<= 1UL;
		}
	if (l_neg ^ r_neg)
		{
		bit_vector tmp(p._end, p._begin, bit_vector::SIGNED);
		unary_inv(tmp, p);
		binary_add(p, tmp, one);
		}
	if (p._signed == bit_vector::UNSET)
		p._signed = l_signed || r_signed ? bit_vector::SIGNED : bit_vector::UNSIGNED;
	return p;
	}

bit_vector &
binary_div_rem(bit_vector &q, bit_vector &rem, const bit_vector &l, const bit_vector &r)
	{
	typedef bit_vector::decimal_type decimal_type;
	typedef bit_vector::position_type position_type;
	bit_vector nan(q._end, q._begin, bit_vector::UNSET, bit_vector::BASE10, DC);

	bit_vector::position_type i, j;
	bool l_signed = l._signed == bit_vector::SIGNED;
	bool r_signed = r._signed == bit_vector::SIGNED;
	bool l_neg = l_signed && l._bits[l._end] == HI;
	bool r_neg = r_signed && r._bits[r._end] == HI;

	bool lfail = false;
	decimal_type ld = l.to_decimal_type(&lfail);
	if (lfail && ld == 0)
		{
		rem = nan;
		return q = nan;
		}
	bool rfail = false;
	decimal_type rd = r.to_decimal_type(&rfail);
	if (rfail && rd == 0)
		{
		rem = nan;
		return q = nan;
		}
	if (!lfail && !rfail)
		{
		decimal_type lud = l_neg ? (~ld + 1) : ld;
		decimal_type rud = r_neg ? (~rd + 1) : rd;
		if (rud == 0)
			{
			rem = nan;
			return q = nan;
			}
		if ((l_neg) ^ r_neg)
			q = -(lud / rud);
		else
			q = lud / rud;
		if (l_neg)
			rem = -lud % rud;
		else
			rem = lud % rud;
		return q;
		}

	// Division using shift and subtract algorithm.
	bit_vector::logic_type cmp;
	bit_vector zero(LO), one(HI);
	bit_vector dend(l._end, l._begin, bit_vector::UNSIGNED);
	bit_vector dsor(r._end, r._begin, bit_vector::UNSIGNED);
	if (r == zero)
		{
		rem = nan;
		return q = nan;
		}
	if (l_neg)
		{
		bit_vector tmp(l._end, l._begin, bit_vector::UNSIGNED);
		unary_inv(tmp, l);
		binary_add(dend, tmp, one);
		}
	else
		dend = l;
	if (r_neg)
		{
		bit_vector tmp(r._end, r._begin, bit_vector::UNSIGNED);
		unary_inv(tmp, r);
		binary_add(dsor, tmp, one);
		}
	else
		dsor = r;
	i = dend._end;
	j = q._end;
	for (; i >= dend._begin; --i)
		{
		rem <<= 1UL;
		rem._bits[0] = dend._bits[i];
		cmp = rem < dsor;
		if (cmp == HI)
			{
			q._bits[i] = LO;
			}
		else if (cmp == LO)
			{
			q._bits[i] = HI;
			rem -= dsor;
			}
		else
			{
			rem = nan;
			return q = nan;
			}
		}
	if (l_neg ^ r_neg)
		{
		bit_vector tmp(q._end, q._begin, bit_vector::SIGNED);
		unary_inv(tmp, q);
		binary_add(q, tmp, one);
		}
	if (q._signed == bit_vector::UNSET)
		q._signed = l_signed || r_signed ? bit_vector::SIGNED : bit_vector::UNSIGNED;
	// Remainder takes the sign of the dividend.
	rem._signed = l._signed;
	if (l_neg)
		{
		bit_vector tmp(q._end, q._begin, bit_vector::SIGNED);
		unary_inv(tmp, q);
		binary_add(q, tmp, one);
		}
	return q;
	}

bit_vector &
binary_div(bit_vector &q, const bit_vector &l, const bit_vector &r)
	{
	bit_vector rem(q._end, q._begin, bit_vector::UNSET, bit_vector::BASE10, LO);
	q = LO;
	binary_div_rem(q, rem, l, r);
	return q;
	}

bit_vector &
binary_mod(bit_vector &rem, const bit_vector &l, const bit_vector &r)
	{
	bit_vector q(rem._end, rem._begin, bit_vector::UNSET, bit_vector::BASE10, LO);
	rem = LO;
	binary_div_rem(q, rem, l, r);
	return rem;
	}

bit_vector &
binary_lshf(bit_vector &res, const bit_vector &l, const bit_vector &r)
	{
	// Shift <l> to the left by <r>.
	bool fail = false;
	bit_vector::decimal_type c = r.to_decimal_type(&fail);
	if (fail)
		memset(res._bits+res._begin, static_cast<int>(DC), res._size);
	else
		{
		bit_vector::position_type start = c + res._begin;
		memset(res._bits+res._begin, static_cast<int>(LO), res._size);
		if (start <= res._end)
			{
			bit_vector::size_type s = (res._end + 1) - start;
			if (s > l._size)
				s = l._size;
			memcpy(res._bits+start, l._bits+l._begin, s);
			}
		}
	return res;
	}

bit_vector &
binary_rshf(bit_vector &res, const bit_vector &l, const bit_vector &r)
	{
	// Shift <l> to the right by <r>.
	bool fail = false;
	unsigned long c = r.to_decimal_type(&fail);
	if (fail)
		memset(res._bits+res._begin, static_cast<int>(DC), res._size);
	else
		{
		bit_vector::position_type start = c + l._begin;
		memset(res._bits+res._begin, static_cast<int>(LO), res._size);
		if (start <= l._end)
			{
			bit_vector::size_type s = (l._end + 1) - start;
			if (s > res._size)
				s = res._size;
			memcpy(res._bits+res._begin, l._bits+start, s);
			}
		}
	return res;
	}

bit_vector &
logic_op(bit_vector &bv, const bit_vector &l, const bit_vector &r,
		 const logic op[][NUMLOGIC])
	{
	bit_vector::position_type i = bv._begin;
	bit_vector::position_type li = l._begin;
	bit_vector::position_type ri = r._begin;
	bit_vector::logic_type *data = bv._bits;
	bit_vector::logic_type *l_data = l._bits;
	bit_vector::logic_type *r_data = r._bits;
	if (l._size <= r._size)
		{
		for (; li <= l._end && i <= bv._end; ++li, ++ri)
			data[i++] = op[l_data[li]][r_data[ri]];
		for (; ri <= r._end && i <= bv._end; ++ri)
			data[i++] = op[LO][r_data[ri]];
		}
	else
		{
		for (; ri <= r._end && i <= bv._end; ++li, ++ri)
			data[i++] = op[l_data[li]][r_data[ri]];
		for (; li <= l._end && i <= bv._end; ++li)
			data[i++] = op[l_data[li]][LO];
		}

	// Clear remaining bits.
	while (i <= bv._end)
		data[i++] = op[LO][LO];

	return bv;
	}

bit_vector::decimal_type
bit_vector::to_decimal_type(bool *fail, size_type nb) const
	{
	// Convert to decimal for quicker arithmetic.  The result is
	// truncated to the size of decimal_type.
	bool signextend = false;
	decimal_type res = 0;
	position_type j = _begin;
	position_type numbits;
	if (nb == 0)
		nb = 8 * sizeof(res);
	if (nb > _size)
		{
		signextend = _signed == SIGNED && _bits[_end] == HI;
		numbits = _end;
		}
	else if (nb == _size)
		numbits = _end;
	else
		numbits = (_begin + nb) - 1;
	for (int i = 0; j <= numbits; ++i, ++j)
		{
		if (_bits[j] == HI)
			res |= static_cast<decimal_type>(1) << i;
		if (_bits[j] > HI)
			{
			if (fail != 0)
				*fail = true;
			return 0;
			}
		}
	logic_type hb = _bits[_end];
	for (; j <= _end; ++j)
		{
		if (_bits[j] != hb)
			{
			if (fail != 0)
				*fail = true;
			return static_cast<decimal_type>(-1);
			}
		}
	if (signextend)
		{
		decimal_type tmp = static_cast<decimal_type>(1) << (_size - 1);
		res = (res ^ tmp) - tmp;
		}
	return res;
	}


// Output functions.


const bit_vector::str_type
bit_vector::to_string(base_type base, size_type len) const
	{
	// <Rnd> rounds the size given in <len> to the next higher size of the
	// base given in <base>.  I.e.  if s = 31, and b = 3 (octal), then the
	// next higher size is 33.  Thus, <Rnd> will evaluate to 33.
#define Rnd(s,b)	((s%b)==0 ? s : s + (b-s%b))
	int tmp[17] = { -1,-1,1,-1,-1,-1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1,4 };

	// See if caller wants to use the default base.
	if (base == BASEDFLT)
		base = _base;

	// If the base is decimal (10), call the convert function for it.
	if (base == BASESTR)
		return binary_2_string(len);
	else if (base == BASE10)
		return binary_2_decimal(len);
	else if (base > BASE16 || tmp[base] < 1)
		return str_type((len > 0) ? len : 1, 'x');

	// Find out the size of the resulting string.
	size_type size;
	if (len == 0)
		{
		size_type nzlen = _size;
		for (position_type i = _end; i >= _begin; --i)
			{
			if (_bits[i] > LO)
				break;
			--nzlen;
			}
		size = Rnd(nzlen, tmp[base]) / tmp[base];
		}
	else if (static_cast<signed long>(len) < 0)
		size = Rnd(_size, tmp[base]) / tmp[base];
	else
		size = len;
	if (size <= 0)
		size = 1; // Have at least one digit.

	// For base 2, 8 and 16, the conversion process is almost the same.
	str_type res(size, ' ');
	int pad = 100;
	position_type j = _begin;
	for (position_type i = size - 1; j <= _end && i != npos; --i)
		{
		switch(base)
			{
			case BASE2: pad = get_bits_as_int(j, 1); break;
			case BASE8: pad = get_bits_as_int(j, 3); break;
			case BASE16: pad = get_bits_as_int(j, 4); break;
			default: return str_type(size, 'x');
			}
		// Convert to ASCII.
		if (pad > 9 && pad < 16)
			res[i] = (pad - 10) + 'a';
		else if (pad < 10)
			res[i] = pad + '0';
		else
			{
			if (base == BASE2 && pad >= 1000)
				res[i] = 'z';
			else
				res[i] = 'x';
			}
		}
	return res;
#undef Rnd
	}

bit_vector::ostream_type &
bit_vector::info(bit_vector::ostream_type &s) const
	{
	if (_msb_lower)
		s << "BV[" << _begin << ':' << _end << "](" << _size << ')';
	else
		s << "BV[" << _end << ':' << _begin << "](" << _size << ')';
	return s;
	}

bit_vector::ostream_type &
operator<<(bit_vector::ostream_type &s, const bit_vector &bv)
	{
	// Since our representation is from least to most significant bit.
	// We have to reverse the displaying process to make it more natural
	// for humans to read.  Humans like the msb at the left-hand side,
	// thus we must start at the end and move backwards.
	if (bv._size > 0)
		{
		bit_vector::position_type i = bv._end;
		for (; i >= bv._begin; --i)
			s << char (bv._bits[i]);
		}
	else
		s << "?01xz.?"; // Invalid bit vector.
	return s;
	}
