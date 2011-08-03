/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * sim.h
 *
 * Functions that do all the work are moved out of the main driver
 * function and into the sim.cc file.  This header is included by
 * vbs.cc so it can call functions to do all the work.
 */

#ifndef _SIM_H
#define _SIM_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "p_types.h"

extern void vbs_sim_init(const char *);
extern void vbs_sim_start(int, char **);
extern long int vbs_sim_run(int);

extern const char *sim_errmsg(const char *, const char *, const char *, int, int, const char *);
extern void sim_perror(char *, char *, char *, int);
extern void sim_set_state(int);
extern void sim_set_dump(int);
extern void sim_run_status(int, const char *);
extern unsigned long sim_current_time();
extern void sim_cpu_time(int);
extern void (*sim_parsed_module)(p_module m);

extern int sim_program_exist(const char *);
extern int sim_program_exec(const char *, char *, FILE *);

#ifdef __cplusplus
}
#endif

#endif /* _SIM_H */
