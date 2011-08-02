/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2001,2011 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * parser.h
 *
 * Misc. functions and external data for the parser.
 */

#ifndef _PARSER_H
#define _PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <stdio.h>

/*
 * Global data variables.
 */

extern FILE *yyin; /* Input for yacc parser. */
#ifdef YYTEXT_POINTER
extern char *yytext; /* Current symbol for error reporting. */
#else
extern char yytext[];
#endif
extern int cur_lineno; /* Line number of error. */
extern char *cur_filename; /* File name of error. */
extern char *savedword; /* Save keyword for error reporting. */
extern void yyerror(char *str); /* Our version. */
extern int yywrap(void); /* Our version. */
extern int yylex(void); /* Lexical analyzer function. */
extern int yyparse(void); /* Parser function. */

extern int p_start(FILE *, const char *, int);
extern char *vbs_strdup(const char *);

#ifdef __cplusplus
}
#endif

#endif /* _PARSER_H */
