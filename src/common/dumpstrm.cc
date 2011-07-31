// Verilog Behavioral Simulator
// Copyright (C) 2001-2002 George Varughese
//
// With modifications from:
// Jimen Ching <jching@flex.com>
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dumpstrm.cc

#include <cstdio> // sprintf
#include <iostream>
#include "dumpstrm.h"

using std::cerr;
using std::endl;

// Non-compressing dump_stream.

void 
dump_stream_nc::put(strstream_type &s)
	{
	fout << s.str();
	s.clear();
	}

bool
dump_stream_nc::open(const str_type &f)
	{
	fout.open(f.c_str());
	return true;
	}

void
dump_stream_nc::close()
	{
	fout.close();
	}


// Compressing dump_stream.

void 
dump_stream_c::put(strstream_type &s)
	{
#if defined(HAVE_ZLIB_H)
	int wr_length, err;
	str_type str(s.str());
	int len = str.size();

	wr_length = gzwrite(gout, (void *)str.c_str(), len);
	if (wr_length != len)
		{
		cerr << gzerror(gout, &err) << endl
			 << wr_length << " " << len
			 << " for stream '" << str << "'" << endl;
		}
#endif
	s.clear();
	}

bool
dump_stream_c::open(const str_type &f)
	{
#if defined (HAVE_ZLIB_H)
	str_type fname = f + ".gz"; // Appending .gz to file name
	gout = gzopen(fname.c_str(),"w"); 
	if (gout == NULL)
		return false;
	if (gzsetparams(gout, level, Z_DEFAULT_STRATEGY) != Z_OK)
		return false;
#else
	cerr << "Dump compression was not built-in, "
		 << "no output generated."
		 << endl;
#endif
	return true;
	}

void
dump_stream_c::close()
	{
#if defined (HAVE_ZLIB_H)
	if (gzclose(gout) != Z_OK)
		{
		cerr << "Error: failed gzclose of vcd_dump" << endl;
		exit(-1);
		}
#endif
	}

dump_stream &
operator<<(dump_stream &s, dump_stream::strstream_type &o)
	{
	s.put(o);
	return s;
	}
