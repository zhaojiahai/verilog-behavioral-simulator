// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// hash.cc

#include "common/hash.h"

#define VALSIZE              16
#define TWELVE_PERCENT       ( (int)(VALSIZE * .125) )
#define SEVENTY_FIVE_PERCENT ( (int)(VALSIZE * .75) )
#define HIGH_BITS            ( ~((unsigned short)~0 >> TWELVE_PERCENT) )

// Static table size.
unsigned long TABLESIZE = 0;

hash_value
hash(const hash_value::str_type &s, hash_value::scope_type sc, bool init)
	{
	if (TABLESIZE == 0)
		return hash_value(INIT_ERROR_VALUE);

	hash_value hv(0, sc, init);
	unsigned short hb, work = hv._value;
	for(unsigned int i = 0; i < s.length(); ++i)
		{
		work = (work << TWELVE_PERCENT) + s[i];
		hb = work & HIGH_BITS;
		if (hb != 0)
			work = (work ^ (hb >> SEVENTY_FIVE_PERCENT)) & HIGH_BITS;
		}
	hv._value = work % TABLESIZE; // Reduce to 17 bits.
	return hv;
	}

void
rehash(hash_value &hv, hash_value::index_type i)
	{
	if (TABLESIZE == 0)
		{
		hv._value = 0;
		hv._scope = -1;
		return;
		}
	hv._value = hv._value * i;
	hv._value = hv._value % TABLESIZE;
	}

hash_value::ostream_type &
operator<<(hash_value::ostream_type &s, const hash_value &hv)
	{ s << '(' << hv._value << ',' << hv._scope << ')'; return s; }
