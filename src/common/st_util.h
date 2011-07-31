// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// st_util.h
//
// Utilities used by st_function and st_task objects.

#ifndef _STUTIL_H
#define _STUTIL_H

#include <list>
#include "common/ptr.h"
#include "common/hash.h"
#include "common/error.h"

class expr_base;
bool check_arguments(std::list<hash_value> *, std::list< basic_ptr<expr_base> > *);
void entry_iovars(std::list<hash_value> &, std::list< basic_ptr<expr_base> > &);
void exit_iovars(std::list<hash_value> &, std::list< basic_ptr<expr_base> > &);

void print_escapecode(const vbs_error::str_type &, size_t &);
bool find_format(size_t &, const vbs_error::str_type &,
				 vbs_error::str_type &, vbs_error::value_type &);
vbs_error::value_type check_format(const vbs_error::str_type &);

#endif // _STUTIL_H
