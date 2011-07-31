// Verilog Behavioral Simulator
// Copyright (C) 2000,2001 George Varughese
//
// With modifications from:
//  Jimen Ching <jching@flex.com>
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// vbs_swig.i
//
// This file is included into the scripting language specific interface
// file.  This allows scripting language specific code to be automatically
// generated.

extern "C" long int vbs_sim_run(int end_time);
extern "C" bool vbs_sim_step();
extern char *vbs_display_net(char *);
extern bool vbs_set_net(char * net, char *val);
extern int vbs_get_scope(char * net_name);
extern char * vbs_list_instances(char *);


extern long int vbs_current_time();
extern bool vbs_set_scope(char *inst);
extern char * vbs_list_nets(void);
extern void vbs_display_symboltable(void);
extern char * vbs_list_modules();
extern int vbs_read(char *vf);

extern void vbs_dumpon();
extern void vbs_dumpoff();
extern void vbs_dumpfile(char *f);
extern void vbs_dumpvars(int l, char *f);
extern void vbs_sim_continue();
extern void vbs_sim_fast();
extern void vbs_sim_till(long int tm);
extern void vbs_sim_for(long int tm);
extern char * vbs_current_scope();
extern char * vbs_push_scope(char *scope);
extern char * vbs_pop_scope();
extern void vbs_sim_start();
