// Verilog Behavioral Simulator
// Copyright (C) 2000-2001 George Varughese
//
// With modifications from:
// Jimen Ching <jimen.ching@gmail.com>
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// scp_tab.h

#ifndef _SCP_TAB_H
#define _SCP_TAB_H

#include <string>
#include <list>
#include <map>

#include "sym_tab.h"

class symbol_table;
class st_net;
class st_instantiation;

class scope_table 
	{
public:
	template<typename U> struct list_type : public std::list<U> { };
	typedef std::string str_type;
	typedef list_type<str_type> str_list;
	typedef st_net net_type;
	// Require std::list to be compatible with dump_base.
	typedef std::list<net_type*> net_list;
	typedef st_instantiation instantiation_type;
	typedef list_type<instantiation_type*> instantiation_list;
	typedef std::map<int, str_type, std::less<int> > hashtable_type;
	typedef symbol_table::bucket_type bucket_type;
	typedef symbol_table::size_type size_type;

	scope_table();
	scope_table(net_list *);
	scope_table(net_list *, int);

	char *find(int i); // return string corresponding to scope
	int find(const str_type &); // return the scope of the string
	net_list get_nets(int); // return the no. of nets at the level
	net_list get_nets(int, int); // return the no. of nets at the level
	net_list get_nets(const char *sc); // all signals below level sc
	net_list get_nets(const char *sc, int); // all_signals to l levels below
	net_list get_nets(instantiation_list &, int = 0); // all_signals to l levels below
	st_net *get_net(const char *); // return the st_net * of net
	char *get_scope(const char *); // return the scope of net
	st_instantiation *get_instance(str_type &);
	instantiation_list get_instances(int sc);
	instantiation_list get_instances(const char *);
	instantiation_list get_instances(char *sc, net_list *n_lst); 
	// returns both instances and nets at level sc.
	net_list *get_dump_list();
	void clear_dump_list();

	int push_level(); // return the no. of levels from top
	int pop_level(); // return the no. of levels from level
	int level(); // return current_level
	int instance_level(); // returns the last instance level
	void instance_level(int); // adds level to instance level, -1 move up.
	str_type scope_string(); // returns the string denoting current scope
	void push_scope(const str_type &); // add string from _scope_string
	void pop_scope(); // pop off top of _scope_string
	void insert(int);
	void insert(int i, const str_type &);
	void add(net_list &);
	bool net_list_added();
	void net_list_added(bool);
	const str_type &top_level();
	void top_level(const str_type &);
	void is_net(const bool);
	bool is_net();

private:
	hashtable_type _hashtab;
	net_list _dump_list; // list of nets to be dumped
	int _dump_scope; // dump_info.top_level();
	int _instance_level;
	int _current_level; // Keep track while dealing with
						// setting up scope_table.
	bool _net_list_added; // If $dumpvars ran already.
	str_list _scope_string;
	str_type _top_level;
	int dump_top_level; // top level for the current dumpvars call
	bool _is_net;

	};

#endif // _SCP_TAB_H
