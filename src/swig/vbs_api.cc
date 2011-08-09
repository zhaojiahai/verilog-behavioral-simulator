// Verilog Behavioral Simulator
// Copyright (C) 2000,2001,2002,2011 George Varughese, Jimen Ching
//
// With modifications from:
//  Jimen Ching <jimen.ching@gmail.com>
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// vbs_api.cc

#include <csetjmp> // longjmp()
#include <list>
#include <sstream>
#include <iostream>
#include "common/hash.h"
#include "common/error.h"
#include "common/time_whl.h"
#include "common/sym_tab.h"
#include "common/st_eval.h"
#include "common/st_net.h"
#include "common/st_mod.h"
#include "common/st_inst.h"
#include "common/dumpinfo.h"
#include "common/scp_tab.h"
#include "vbs.h"
#include "sim.h"

using std::endl;
using std::list;
using std::string;
using std::stringstream;

typedef symbol_table::size_type size_type;
typedef symbol_table::bucket_type bucket_type;

extern "C" void vbs_sim_setup(void);
extern "C" long int vbs_sim_run(int);
extern "C" bool vbs_sim_step();
extern "C" int read_verilog_file(char *);
extern "C" void vbs_sim_init(const char *);
extern "C" char *vbs_strdup(const char *);
static int current_scope = 1;
static int last_scope = 1;
static bool sim_init = false;
string errorString;


// Functions for external use
static st_net *
get_net(char *net_name)
	{
	scope_table &scopetable = vbs_engine::scopetable();
	return scopetable.get_net(net_name);
	}

char *
vbs_display_net(char *net_name)
	{
	stringstream result;
	st_net *net = get_net(net_name);
	if (net != 0)
		result << net->evaluate(evaluate_st_node());
	else
		{
		result << "Warning: Unable to find net ";
		result << net_name;
		}
	return vbs_strdup(result.str().c_str());
	}

int
vbs_get_scope(char * net_name)
	{	
	scope_table &scopetable = vbs_engine::scopetable();
	string scp = net_name;
	string top = scopetable.top_level();
#if __GNUC__ == 2 && __GNUC_MINOR__ >= 95
	if (scp.compare(top, 0, top.size()) == 0)
#else
	if (scp.compare(0, top.size(), top) == 0)
#endif
	scp.replace(0,top.length()+1,"");
	if (scp.length() > 0)
		return scopetable.find(scp.c_str());
	return 1;
	}	

bool
vbs_set_net(char *net_name, char *val)
	{
	string v = val;
	bit_vector bv(v);
	st_net * net = get_net(net_name);
	if (net != 0)
		{
		net->assignment(bv, -1, -1, -1, -1);
		return true;
		}
	return false;
	}

char *
vbs_list_instances(char *scope)
	{
	symbol_table &symboltable = vbs_engine::symboltable();
	string instances;
	size_type max = symboltable.size();
	int scp = vbs_get_scope(scope);
	for (size_type i = 0; i < max; ++i)
		{
		bucket_type::iterator item = symboltable.begin(i);
		bucket_type::iterator last  = symboltable.end(i);
		for (; item != last; ++item)
			{
			st_instantiation *n = item->get()->get_instantiation();
			if (n && scp == n->scope())
				{
				instances += n->name();
				instances += " ";
				}
			}
		}
	return vbs_strdup(instances.c_str());
	}

bool
vbs_set_scope(char *inst)
	{	
	int scp = vbs_get_scope(inst);
	if ( scp == -1) return false;
	else 
		{
		last_scope = current_scope;
		current_scope = scp;
		}
	return true;
	}

char *
vbs_list_nets(void)
	{
	symbol_table &symboltable = vbs_engine::symboltable();
	string nets;
	size_type max = symboltable.size();
	for (size_type i = 0 ; i < max ; i ++ )
		{
		bucket_type::iterator item = symboltable.begin(i);
		bucket_type::iterator last  = symboltable.end(i);
		for ( ; item != last ; item++)
			{
			st_net *n = item->get()->get_net();
			if (n != 0 && n->scope() == current_scope)
				{
				nets += n->name();
				nets += " ";
				}
			}
		}
	return vbs_strdup(nets.c_str());
	}

void
vbs_display_symboltable(void)
	{
	symbol_table &symboltable = vbs_engine::symboltable();
	symboltable.display(cout);
	}

char *
vbs_list_modules()
	{
	symbol_table &symboltable = vbs_engine::symboltable();
	list<hash_value> &modulelist = vbs_engine::modulelist();
	string mod_list;
	list<hash_value>::iterator idx(modulelist.begin());
	list<hash_value>::iterator stop(modulelist.end());
	for (; idx != stop; ++idx)
		mod_list += symboltable.get(*idx)->get_module()->name() + " ";
	return vbs_strdup(mod_list.c_str());
	}

void
vbs_dumpon()
	{
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	dumpinfo.dump_on(true);
	}

void
vbs_dumpoff()
	{
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	dumpinfo.dump_on(false);
	}

void
vbs_dumpfile(char *f)
	{
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	string fn = f;
	dumpinfo.dump_file(fn);
	}

void
vbs_dumpvars(int l,char *f)
	{
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	string fn = f;
	dumpinfo.levels(l);
	dumpinfo.dump_vars(f);
	}

int
vbs_read(char *fn)
	{
	if (!sim_init)
		{
		vbs_sim_init("tcl-vbs");
		sim_init = true;
		}
	return read_verilog_file(fn);
	}

void
vbs_sim_start()
	{
	vbs_sim_setup();
	}

long int
vbs_sim_continue()
	{
	bool was_ok = true ;
	while (was_ok) was_ok = vbs_sim_step();
	return sim_current_time();
	}

long int
vbs_sim_fast()
	{
	vbs_sim_run(-1);
	return sim_current_time();
	}

void
vbs_sim_till(long int tm)
	{
	vbs_sim_run(tm);
	}

void
vbs_sim_for(long int tm)
	{
	long int now = sim_current_time();
	vbs_sim_run(now + tm);
	}

char *
vbs_current_scope()
	{
	scope_table &scopetable = vbs_engine::scopetable();
	return scopetable.find(current_scope);
	}

char *
vbs_push_scope(char * scope)
	{
	scope_table &scopetable = vbs_engine::scopetable();
	int scp = vbs_get_scope(scope);
	last_scope = current_scope;
	current_scope = scp;
	return scopetable.find(current_scope);
	}

char *
vbs_pop_scope()
	{
	scope_table &scopetable = vbs_engine::scopetable();
	current_scope = last_scope;
	return scopetable.find(current_scope);
	}

void
vbs_warn(int c, const string &cm, const char *st, const string &fn, int ln, const string &m, const string &p)
	{
	extern const char *sim_errmsg(const char *, const char *, const char *, int, int, const char *);
	extern jmp_buf vbs_sim_finish;
	cout << sim_errmsg(p.c_str(), m.c_str(), fn.c_str(), ln, c, cm.c_str()) << st << endl;
	longjmp(vbs_sim_finish, 1);
	}

void
vbs_fatal(int c, const string &cm, const char *st, const string &fn, int ln, const string &m, const string &p)
	{
	extern const char *sim_errmsg(const char *, const char *, const char *, int, int, const char *);
	extern jmp_buf vbs_sim_finish;
	cout << sim_errmsg(p.c_str(), m.c_str(), fn.c_str(), ln, c, cm.c_str()) << st << endl;
	longjmp(vbs_sim_finish, 1);
	}
