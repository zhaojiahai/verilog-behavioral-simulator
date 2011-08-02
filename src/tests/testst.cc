// Verilog Behavioral Simulator
// Copyright (C) 1995,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// testst.cc

#include "common/st_node.h"
#include "common/sym_tab.h"

using namespace std;

symbol_table symboltable;

class Obj : public st_node_base
	{
public:
	Obj(const char *str, scope_type sc = -1)
		: st_node_base(str, "Obj")
		{ _scope = sc; }

	ostream &display(ostream &s) const
		{ s << _name << '(' << _scope << ')'; return s; }
	};

void
newSymbol(const char *name, st_node_base::scope_type sc,
		const char *_1st, const char *_2nd)
	{
	Obj *tmp = new Obj(name, sc);
	st_node_base::hash_type idx(symboltable.add(tmp));
	if (idx._scope < 0)
		cout << "Error entering symbol: " << name << _1st << endl;
	else
		cout << "No error entering symbol: " << name << _2nd << endl;
	}

void
findSymbol(const char *name, hash_value::scope_type sc,
		const char *_1st, const char *_2nd)
	{
	st_node_base::hash_type idx(hash(name, sc, false));
	st_node_base *tmp = symboltable.get(idx);
	if (tmp == 0)
		cout << idx << ':' << name << " not found " << _1st << endl;
	else
		{
		cout << idx << ':';
		tmp->display(cout) << _2nd << endl;
		}
	}

int
main(/*int argc, char *argv[]*/)
	{
	symboltable.initialize(65537);

	newSymbol("abc", 0, " (Not!)", " (Ok!)");
	newSymbol("bcd", 0, " (Not!)", " (Ok!)");
	newSymbol("cde", 0, " (Not!)", " (Ok!)");
	newSymbol("bcd", 0, " (Ok!)", " (Not!)");
	newSymbol("cde", 0, " (Ok!)", " (Not!)");
	newSymbol("bcd", 1, " (Not!)", " (Ok!)");

	findSymbol("abc", 0, "(Not!)", "(Ok!)");
	findSymbol("bcd", 0, "(Not!)", "(Ok!)");
	findSymbol("cde", 0, "(Not!)", "(Ok!)");
	findSymbol("def", 0, "(Ok!)", "(Not!)");

	hash_value idx(hash("abc123", 0, false));
	cout << "idx = " << idx << endl;
	rehash(idx, 3);
	cout << "idx = " << idx << endl;

	cout << "sym:" << endl << symboltable << endl;

	return 0;	
	}
