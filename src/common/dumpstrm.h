// Verilog Behavioral Simulator
// Copyright (C) 2001,2011,2011 George Varughese
//
// With modifications from:
// Jimen Ching <jimen.ching@gmail.com>
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// dumpstrm.h

#ifndef __DUMP_STREAM_H
#define __DUMP_STREAM_H

#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#if defined (HAVE_ZLIB_H)
#include <zlib.h>
#endif
#include <string>
#include <fstream>
#include <sstream>

class dump_stream
	{
protected:
	bool _compress;
	typedef std::string str_type;
	typedef std::ofstream ofstream_type;
	typedef std::stringstream strstream_type;

public:
	virtual ~dump_stream()
		{}

	bool compress() { return _compress; }
	void compress(bool yn) { _compress = yn; }
	virtual bool open(const str_type &) = 0;
	virtual void close() = 0;
	virtual void put(strstream_type &) = 0;

	friend dump_stream &operator<<(dump_stream &, strstream_type &);
	};

class dump_stream_nc : public dump_stream
	{
public:
	dump_stream_nc()
		{ compress(false); }
	virtual ~dump_stream_nc()
		{}

	bool open(const str_type &);
	void close();
	void put(strstream_type &);

private:
	ofstream_type fout;
	};

class dump_stream_c : public dump_stream
	{
public:
	dump_stream_c(int lvl)
#if defined(HAVE_ZLIB_H)
		: level(lvl)
#endif
		{ compress(true); }
	virtual ~dump_stream_c()
		{}

	bool open(const str_type &);
	void close();
	void put(strstream_type &);

private:
#if defined(HAVE_ZLIB_H)
	gzFile gout;
	int level;
#endif
	};

#endif // __DUMP_STREAM_H
