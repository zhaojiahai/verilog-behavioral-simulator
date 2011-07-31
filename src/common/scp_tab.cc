// Verilog Behavioral Simulator
// Copyright (C) 2000-2001 George Varughese
//
// With modifications from:
// Jimen Ching <jching@flex.com>  
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// scp_tab.cc

#include <cstring>
#include "sym_tab.h"
#include "st_inst.h"
#include "st_net.h"
#include "scp_tab.h"

extern "C" char *vbs_strdup(const char *);
extern symbol_table symboltable;

scope_table::scope_table()
	{
	_current_level = 0; // Top!
//	_hashtab[1] = "top"; // Top level!
	}

scope_table::scope_table(net_list *n)
	{
	scope_table(n, 0);
	}

scope_table::scope_table(net_list *n, int sc)
	{
	_dump_scope = sc;
	_instance_level = -1;
	_current_level = 0; // Top!
	}

void
scope_table::is_net(const bool yn)
	{ _is_net = yn; }

bool
scope_table::is_net()
	{ return _is_net; }

char *
scope_table::find(int i)
	{
	hashtable_type::iterator itp = _hashtab.find(i);
	if (itp != _hashtab.end())
		return vbs_strdup((*itp).second.c_str());
	return "";
	}

int    
scope_table::find(const str_type &n)
	{
	// This is costly!
	hashtable_type::iterator ibegin = _hashtab.begin();
	hashtable_type::iterator istop = _hashtab.end();
	for (; ibegin != istop; ++ibegin)
		{
		if (ibegin->second == n)
			return ibegin->first;
		}
	return -1;
	}

scope_table::net_list
scope_table::get_nets(int sc) 
	{
	// Returns nets at scope sc
	net_list n_list;
	size_type max = symboltable.size();
	for (size_type i = 0; i < max; ++i)
		{
		bucket_type::iterator item = symboltable.begin(i);
		bucket_type::iterator last  = symboltable.end(i);
		for (; item != last; ++item)
			{
			st_net *n = item->get()->get_net();
			if (n != 0)
				{
				if (n->scope() == sc)
					n_list.insert(n_list.end(), n);
				}
			}
		}
	return n_list;
	}

st_net *
scope_table::get_net(const char *st)
	{
	int sc;
	char *scp = get_scope(st);
	str_type net_name(st);
	int dot = net_name.find(".");
	if (dot == -1)
		sc = 1; // top
	else
		{
		if (scp == 0)
			sc = 1; // try top
		else
			sc = find(scp);
		if (sc == 0)
			sc = 1; // top level
		}
	size_type max = symboltable.size();
	unsigned int last_dot_position = net_name.rfind(".");
	if (last_dot_position < net_name.length())
		net_name = net_name.substr(last_dot_position+1);
	for (size_type i = 0; i < max; ++i)
		{
		bucket_type::iterator item = symboltable.begin(i);
		bucket_type::iterator last  = symboltable.end(i);
		for (; item != last; ++item)
			{
			st_net *n = item->get()->get_net();
			if (n != 0)
				{
				//if (n->name() == st && n->scope() == sc)
				if (n->name() == net_name && n->scope() == sc)
					return n;
				}
			}
		}
	return 0;
	}

// Returns the scope name of full net name
// Eg:  a.n1 returns a

char *
scope_table::get_scope(const char *nin)
	{
	str_type nm = nin;
	str_type top = top_level();
#if __GNUC__ == 2 && __GNUC_MINOR__ >= 95
	if (nm.compare(top, 0, top.size()) == 0)
#else
	if (nm.compare(0, top.size(), top) == 0)
#endif
		nm.replace(0,top.length()+1,"");
	const char * n = nm.c_str();
	size_t full, sub,diff;
	char * last_dot = strrchr(n,'.');
	char * ret = 0;
	if (last_dot != 0)
		{
		full = strlen(n);
		sub  = strlen(last_dot);
		diff = full - sub;
		ret = new char [diff+1];
		strncpy(ret,n,diff);
		ret[diff] = '\0';
		}
	else
		{
		if (strlen(n) > 0)
			ret = vbs_strdup(n);
		}
	return ret;
	}
	
scope_table::net_list
scope_table::get_nets(int sc, int lvls)
	{
	net_list n_list;
	instantiation_list inst_list = get_instances(sc);

	if (inst_list.size() > 0)
		n_list = get_nets(inst_list,lvls);
	net_list tmp_n_list = get_nets(sc);
	// This level...
	n_list.insert(n_list.end(), tmp_n_list.begin(), tmp_n_list.end());
	return n_list;
	}

scope_table::net_list
scope_table::get_nets(const char *sc)
	{
	net_list lst;
	int scp = 1;
	if (strcmp(sc,top_level().c_str()))
		scp = find(sc); // Not top!
	if (scp == -1) 
		return lst;
	lst = get_nets(scp);
	return lst;
	}

scope_table::net_list
scope_table::get_nets(const char *sc, int lvls)
	{
	net_list nlst;
	str_type nm = sc;
	str_type top = top_level();
	int scp = 1;
	if (nm != top)
		{
		// scope table is relative to top
#if __GNUC__ == 2 && __GNUC_MINOR__ >= 95
		if (nm.compare(top, 0, top.size()) == 0)
#else
		if (nm.compare(0, top.size(), top) == 0)
#endif
			nm.replace(0, top.length()+1, "");
		is_net(false);
		scp = find(nm.c_str());
		}
	if (scp == -1) 
		{
		// check if its a net
		st_net * net = get_net(nm.c_str());
		if (net == 0)
			{
			cout << "Warning : " << nm << " undefined instance or net." << endl;
			} 
		else
			{
			is_net(true);
			nlst.insert(nlst.end(),net);
			}
		return nlst;
		}
	st_instantiation *cur_inst = get_instance(nm);
	if (cur_inst != 0) // get nets for cur_inst
		{
		dump_top_level = cur_inst->level();	
		nlst = get_nets(scp,lvls);
		return nlst;
		}
	else              // could be a net or top level
		{
		if (nm == top_level())
			{
			// Start from top!
			nlst = get_nets(1,lvls);
			}
		else
			{
			// FIXME: should not reach here after the second
			// setup pass...
			cout << "Warning : " << nm << " undefined instance or net." << endl;
			}
		return nlst;
		}
	}

scope_table::net_list
scope_table::get_nets(instantiation_list & inst_list, int lvls)
	{
	// Returns nets lvls levels below inst_list level.
	net_list tmp_net_list;
	instantiation_list tmp_inst_list;

	// It is inefficient to traverse the full symbol table, but...
	instantiation_list::iterator itp = inst_list.begin();
	instantiation_list::iterator stop = inst_list.end();
	for (; itp != stop; ++itp) 
		{
		st_instantiation *inst = *itp;
		int inst_scope = inst->instance_scope();
		int cur_rel_level = inst->level() - dump_top_level;
		if ((cur_rel_level < lvls) || ( lvls == 0))
			{
			net_list nt;
			instantiation_list child_inst_list;
			child_inst_list = get_instances(inst_scope);
			nt = get_nets(child_inst_list,lvls);
			tmp_net_list.insert(tmp_net_list.end(), nt.begin(), nt.end());
			nt = get_nets(inst_scope);
			tmp_net_list.insert(tmp_net_list.end(), nt.begin(), nt.end());
			}
		}
	return tmp_net_list;
	}

st_instantiation *
scope_table::get_instance(str_type &nam)
	{
	int sc = find(nam);
	size_type max = symboltable.size();
	for (size_type i = 0; i < max; ++i)
		{
		bucket_type::iterator item = symboltable.begin(i);
		bucket_type::iterator last  = symboltable.end(i);
		for (; item != last; ++item)
			{
			st_instantiation *n = item->get()->get_instantiation();
			if (n != 0)
				{
				if (n->instance_scope() == sc)
					return n;
				}
			}
		}
	return 0;
	}

scope_table::instantiation_list
scope_table::get_instances(int sc) 
	{
	// Returns instances at scope sc.
	instantiation_list inst_lst;
	size_type max = symboltable.size();
	for (size_type i = 0; i < max ; ++i)
		{
		bucket_type::iterator item = symboltable.begin(i);
		bucket_type::iterator last = symboltable.end(i);
		for (; item != last; ++item)
			{
			st_instantiation *n = item->get()->get_instantiation();
			if (n != 0)
				{
				if (n->scope() == sc)
					inst_lst.insert(inst_lst.end(), n);
				}
			}
		}
	return inst_lst;
	}

scope_table::instantiation_list
scope_table::get_instances(const char *sc) 
	{
	int i = find(sc);
	return get_instances(i);
	}

scope_table::instantiation_list
scope_table::get_instances(char *sc, net_list *n_lst)
	{
	net_list nets;
	nets = get_nets(sc);
	n_lst->insert(n_lst->end(), nets.begin(), nets.end());
	return get_instances(sc);
	}

scope_table::net_list *
scope_table::get_dump_list()
	{ return &_dump_list; }

void
scope_table::clear_dump_list()
	{ _dump_list.clear(); }
int
scope_table::push_level()
	{ return ++_current_level; }

int 
scope_table::pop_level()
	{ return --_current_level; }

int
scope_table::level()
	{ return _current_level; }

scope_table::str_type
scope_table::scope_string()
	{
	str_type sc("");
	str_list::iterator itp(_scope_string.begin());
	str_list::iterator stop(_scope_string.end());
	if (itp != stop && itp->size() > 0)
		sc = *itp;
	else
		return sc;
	for (++itp; itp != stop; ++itp)
		{
		sc += ".";
		sc +=  *itp;
		}
	return sc;
	}

void
scope_table::push_scope(const str_type &s)
	{
	_scope_string.insert(_scope_string.end(), s);
	}

void
scope_table::pop_scope()
	{
	if (_scope_string.size() > 0)
		{
		str_list::iterator stop = _scope_string.end();
		--stop;
		_scope_string.erase(stop);
		}
	}

void
scope_table::insert(const int i)
	{
	str_list::iterator itp = _scope_string.begin();
	str_list::iterator stop = _scope_string.end();
	str_type this_scope;
	this_scope = (*itp);
	++itp;
	for (; itp != stop; ++itp)
		{
		this_scope += ".";
		this_scope += (*itp);
		}
	_hashtab[i] = this_scope;
	}

void
scope_table::insert(int i, const str_type &s)
	{
	_hashtab[i] = s;
	}

void
scope_table::add(net_list &n)
	{
	net_list::iterator b(n.begin());
	net_list::iterator e(n.end());
	if (b != e)
		_dump_list.insert(_dump_list.end(), b, e);
	}

bool 
scope_table::net_list_added() 
	{ return _net_list_added; }
	
void 
scope_table::net_list_added(bool n)
	{
	_net_list_added = n;
	}

const scope_table::str_type &
scope_table::top_level()
	{
	return _top_level;
	}

void
scope_table::top_level(const str_type &t)
	{
	_hashtab[1] = t; // Top level!
	_top_level = t;
	}
