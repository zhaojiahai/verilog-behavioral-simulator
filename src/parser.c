/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2000-2002,2011 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * parser.c
 */

#include <stdio.h> /* FILE * */
#include <string.h> /* strlen() */
#include <stdlib.h> /* malloc() */
#include "parser.h"

int
p_start(FILE *fp, const char *fn, int ln)
	{
	yyin = fp;
	cur_lineno = ln;
	if (cur_filename != 0)
		free(cur_filename);
	cur_filename = vbs_strdup(fn);
	return yyparse();
	}

char *
vbs_strdup(const char *str)
	{
	char *p;
	int len;
	len = strlen(str);
	p = (char *)malloc(len + 1);
	strcpy(p, str);
	return p;
	}

#if defined(VERILOGVPP_PROG)

/*
 * Functions that need the POSIX interface.  Placed in this file because
 * we do not want to compile other files with POSIX enabled.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int parms_cnt = 1;
char *parms[128];

int
sim_program_exist(const char *fn)
	{
	struct stat sbuf;

	if (stat(fn, &sbuf) < 0)
		return 0;
	/* Check to see if this program is usable. */
	if ((sbuf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
	 && S_ISREG(sbuf.st_mode))
		return 1;
	return 0;
	}

int
sim_program_exec(const char *pn, char *vf, FILE *out)
	{
	int wait_pid, prog_status;
	pid_t cpid;
	cpid = fork();
	if (cpid < 0)
		return 0;
	else if (cpid == 0)
		{
		/* In the child. */
		prog_status = dup2(fileno(out), STDOUT_FILENO);
		if (prog_status < 0)
			exit(0);
		/* Close on exec. */
		prog_status = fcntl(fileno(out), F_SETFD, 1);
		if (prog_status < 0)
			exit(0);
		parms[0] = (char *)pn; /* Program name. */
		parms[parms_cnt++] = "-E"; /* C style preprocessor. */
		parms[parms_cnt++] = "-L"; /* Generate `line directive. */
		parms[parms_cnt++] = "-q"; /* Silence status message. */
		parms[parms_cnt++] = vf;
		prog_status = execv(pn, parms);
		if (prog_status < 0)
			exit(0);
		/* Should not get here... */
		}

	/* Wait for child to finish, then check status. */
	wait_pid = waitpid(cpid, &prog_status, 0);
	if (wait_pid < 0)
		return 0;
	else if (wait_pid != cpid || prog_status != 0)
		return 0;
	return 1;
	}

#endif // VERILOGVPP_PROG
