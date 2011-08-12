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

/* Also used by main.cc... */
int parms_cnt;
char *parms[16];

int
p_start(FILE *fp, const char *fn, int ln)
	{
	yyin = fp;
	cur_lineno = ln;
	if (cur_filename != 0)
		free(cur_filename);
	cur_filename = vbs_strdup(fn);
	yyreset(); /* Reset lexer. */
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
#include <signal.h>
#include <errno.h>
#include <stdio.h> /* fileno */

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
	struct sigaction ignore, sintr, squit;
	sigset_t cmask, smask;
	int retval = 0;
	pid_t cpid;

	/* Ignore SIGINT/SIGQUIT, block SIGCHLD. */
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	if (sigaction(SIGINT, &ignore, &sintr) < 0)
		return -1;
	if (sigaction(SIGQUIT, &ignore, &squit) < 0)
		return -1;
	sigemptyset(&cmask);
	sigaddset(&cmask, SIGCHLD);
	if (sigprocmask(SIG_BLOCK, &cmask, &smask) < 0)
		return -1;

	cpid = fork();
	if (cpid < 0)
		return -1;
	else if (cpid == 0)
		{
		/* In the child.  Restore signals and masks. */
		sigaction(SIGINT, &sintr, 0);
		sigaction(SIGQUIT, &squit, 0);
		sigprocmask(SIG_SETMASK, &smask, 0);

		retval = dup2(fileno(out), STDOUT_FILENO);
		if (retval < 0)
			return retval;
		/* Close on exec. */
		retval = fcntl(fileno(out), F_SETFD, 1);
		if (retval < 0)
			return retval;
		parms_cnt = 0;
		parms[parms_cnt++] = (char *)pn; /* Program name. */
		parms[parms_cnt++] = "-E"; /* C style preprocessor. */
		parms[parms_cnt++] = "-L"; /* Generate `line directive. */
		parms[parms_cnt++] = "-q"; /* Silence status message. */
		parms[parms_cnt++] = vf;
		parms[parms_cnt++] = '\0';
		retval = execv(pn, parms);
		if (retval < 0)
			exit(retval);
		/* Should not get here... */
		}

	/* Wait for child to finish, then check status. */
	retval = 0;
	while (waitpid(cpid, &retval, 0) < 0)
		{
		if (errno != EINTR)
			{
			retval = -1;
			break;
			}
		}

	/* Restore signals and masks. */
	if (sigaction(SIGINT, &sintr, 0) < 0)
		return -1;
	if (sigaction(SIGQUIT, &squit, 0) < 0)
		return -1;
	if (sigprocmask(SIG_SETMASK, &smask, 0) < 0)
		return -1;

	return retval;
	}

#endif // VERILOGVPP_PROG
