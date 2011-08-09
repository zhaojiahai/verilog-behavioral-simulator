%{
/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * vbs.l
 *
 * Verilog Behavioral Simulator lexical scanner.
 */

#include <string.h> /* strtok */
#include <stdlib.h> /* free, atoi */
#include "parser.h" /* vbs_strdup */
#include "p_types.h" /* types for vbs_yacc.h */
#include "vbs_yacc.h"

void copy_word(char *str);
void save_word(char *str);
void process_directive(char *text);
int cur_lineno = 1;
char *cur_filename;
char *savedword;

#define YY_NO_UNPUT /* Silence warning about unused static (from flex). */

%}

%s COMMENT
%s VENDOR
%s DIRECTIVE
%s POUND

NL						[\n]
WS						[ \t\r\b]
Digit					[0-9]
DigitU					[0-9_]
Letter					[a-zA-Z]
LetterU					[a-zA-Z_]
WordNum					[0-9a-zA-Z]
WordNumU				[0-9a-zA-Z_]
WSs						{WS}+
Number					{Digit}{DigitU}*
Float					[-+]?{Number}\.{Number}
FloatE					[-+]?{Number}(\.{Number})?[eE][-+]?{Number}
Word					{LetterU}{WordNumU}*
WordDot					{Word}(\.{Word})*
Binary					({Number}{WS}*)?\'[s]?[bB]{WS}*[01xXzZ?][01xXzZ?_]*
Octal					({Number}{WS}*)?\'[s]?[oO]{WS}*[0-7xXzZ?][0-7xXzZ?_]*
Decimal					({Number}{WS}*)?\'[s]?[dD]{WS}*{Number}
Hexdecimal				({Number}{WS}*)?\'[s]?[hH]{WS}*[0-9a-fA-FxXzZ?][0-9a-fA-FxXzZ?_]*
QoutedString			\"[^\"]*\"

%%

<INITIAL>{NL}			{ cur_lineno++; }
<INITIAL>{WSs}			{ }
<INITIAL>"//".*$		{ }
<INITIAL>"/*"			{ BEGIN COMMENT; }
<INITIAL>"(*[^)]"			{ BEGIN VENDOR; }

<COMMENT>{NL}			{ cur_lineno++; }
<COMMENT>.				{ }
<COMMENT>"*/"			{ BEGIN INITIAL; }

<VENDOR>{NL}			{ cur_lineno++; }
<VENDOR>.				{ }
<VENDOR>"*)"			{ BEGIN INITIAL; }


	/*
	 * Symbols.
	 */


	/* Generic. */
<INITIAL>"#"			{ save_word("#"); BEGIN POUND; return YYPOUND; }
<INITIAL>"@"			{ save_word("@"); return YYAT; }
<INITIAL>"->"			{ save_word("->"); return YYMINUSGREATER; }
<INITIAL>"-:"			{ save_word("-:"); return YYMINUSCOLON; }
<INITIAL>"+:"			{ save_word("+:"); return YYPLUSCOLON; }

	/* Unary. */
<INITIAL>"+"            { save_word("+"); return YYPLUS; }
<INITIAL>"-"            { save_word("-"); return YYMINUS; }
<INITIAL>"!"            { save_word("!"); return YYEXCLAM; }
<INITIAL>"~"            { save_word("~"); return YYTILDE; }
<INITIAL>"&"            { save_word("&"); return YYAMPER; }
<INITIAL>"~&"           { save_word("~&"); return YYTILDEAMPER; }
<INITIAL>"|"            { save_word("|"); return YYVERTBAR; }
<INITIAL>"~|"           { save_word("~|"); return YYTILDEVERTBAR; }
<INITIAL>"^"            { save_word("^"); return YYCARET; }
<INITIAL>"~^"           { save_word("~^"); return YYTILDECARET; }
<INITIAL>"^~"           { save_word("^~"); return YYCARETTILDE; }

	/* Binary. */
<INITIAL>"*"            { save_word("*"); return YYASTERIC; }
<INITIAL>"/"            { save_word("/"); return YYSLASH; }
<INITIAL>"%"            { save_word("%"); return YYPERCENT; }
<INITIAL>"**"			{ save_word("**"); return YYASTERICASTERIC; }
<INITIAL>"=="           { save_word("=="); return YYEQUALEQUAL; }
<INITIAL>"!="           { save_word("!="); return YYEXCLAMEQUAL; }
<INITIAL>"==="          { save_word("==="); return YYEQUALEQUALEQUAL; }
<INITIAL>"!=="          { save_word("!=="); return YYEXCLAMEQUALEQUAL; }
<INITIAL>"&&"           { save_word("&&"); return YYAMPERAMPER; }
<INITIAL>"||"           { save_word("||"); return YYVERTBARVERTBAR; }
<INITIAL>"<"            { save_word("<"); return YYLESS; }
<INITIAL>"<="           { save_word("<="); return YYLESSEQUAL; }
<INITIAL>">"            { save_word(">"); return YYGREATER; }
<INITIAL>"*>"           { save_word("<="); return YYASTERICGREATER; }
<INITIAL>">="           { save_word(">="); return YYGREATEREQUAL; }
<INITIAL>"=>"           { save_word("<="); return YYEQUALGREATER; }
<INITIAL>">>"           { save_word(">>"); return YYGREATERGREATER; }
<INITIAL>"<<"           { save_word("<<"); return YYLESSLESS; }

	/* Ternary. */
<INITIAL>"?"			{ save_word("?"); return YYQUESTION; }
<INITIAL>":"			{ save_word(":"); return YYCOLON; }


	/*
	 * Keywords.
	 */


<INITIAL>always         { save_word("always"); return YYALWAYS; }
<INITIAL>and            { save_word("and"); return YYAND; }
<INITIAL>assign         { save_word("assign"); return YYASSIGN; }
<INITIAL>begin          { save_word("begin"); return YYBEGIN; }
<INITIAL>buf            { save_word("buf"); return YYBUF; }
<INITIAL>bufif0         { save_word("bufif0"); return YYBUFIF0; }
<INITIAL>bufif1         { save_word("bufif1"); return YYBUFIF1; }
<INITIAL>case           { save_word("case"); return YYCASE; }
<INITIAL>casex          { save_word("casex"); return YYCASEX; }
<INITIAL>casez          { save_word("casez"); return YYCASEZ; }
<INITIAL>cmos           { save_word("cmos"); return YYCMOS; }
<INITIAL>deassign       { save_word("deassign"); return YYDEASSIGN; }
<INITIAL>default        { save_word("default"); return YYDEFAULT; }
<INITIAL>defparam       { save_word("defparam"); return YYDEFPARAM; }
<INITIAL>disable        { save_word("disable"); return YYDISABLE; }
<INITIAL>edge           { save_word("edge"); return YYEDGE; }
<INITIAL>else           { save_word("else"); return YYELSE; }
<INITIAL>end            { save_word("end"); return YYEND; }
<INITIAL>endcase        { save_word("endcase"); return YYENDCASE; }
<INITIAL>endfunction    { save_word("endfunction"); return YYENDFUNCTION; }
<INITIAL>endmodule      { save_word("endmodule"); return YYENDMODULE; }
<INITIAL>endprimitive   { save_word("endprimitive"); return YYENDPRIMITIVE; }
<INITIAL>endspecify     { save_word("endspecify"); return YYENDSPECIFY; }
<INITIAL>endtable       { save_word("endtable"); return YYENDTABLE; }
<INITIAL>endtask        { save_word("endtask"); return YYENDTASK; }
<INITIAL>event          { save_word("event"); return YYEVENT; }
<INITIAL>for            { save_word("for"); return YYFOR; }
<INITIAL>force          { save_word("force"); return YYFORCE; }
<INITIAL>forever        { save_word("forever"); return YYFOREVER; }
<INITIAL>fork           { save_word("fork"); return YYFORK; }
<INITIAL>function       { save_word("function"); return YYFUNCTION; }
<INITIAL>highz0         { save_word("highz0"); return YYHIGHZ0; }
<INITIAL>highz1         { save_word("highz1"); return YYHIGHZ1; }
<INITIAL>if             { save_word("if"); return YYIF; }
<INITIAL>ifnone         { save_word("ifnone"); return YYIFNONE; }
<INITIAL>initial        { save_word("initial"); return YYINITIAL; }
<INITIAL>inout          { save_word("inout"); return YYINOUT; }
<INITIAL>input          { save_word("input"); return YYINPUT; }
<INITIAL>integer        { save_word("integer"); return YYINTEGER; }
<INITIAL>join           { save_word("join"); return YYJOIN; }
<INITIAL>large          { save_word("large"); return YYLARGE; }
<INITIAL>localparam		{ save_word("localparam"); return YYLOCALPARAM; }
<INITIAL>macromodule    { save_word("macromodule"); return YYMACROMODULE; }
<INITIAL>medium         { save_word("medium"); return YYMEDIUM; }
<INITIAL>module         { save_word("module"); return YYMODULE; }
<INITIAL>nand           { save_word("nand"); return YYNAND; }
<INITIAL>negedge        { save_word("negedge"); return YYNEGEDGE; }
<INITIAL>nmos           { save_word("nmos"); return YYNMOS; }
<INITIAL>nor            { save_word("nor"); return YYNOR; }
<INITIAL>not            { save_word("not"); return YYNOT; }
<INITIAL>notif0         { save_word("notif0"); return YYNOTIF0; }
<INITIAL>notif1         { save_word("notif1"); return YYNOTIF1; }
<INITIAL>or             { save_word("or"); return YYOR; }
<INITIAL>output         { save_word("output"); return YYOUTPUT; }
<INITIAL>parameter      { save_word("parameter"); return YYPARAMETER; }
<INITIAL>pmos           { save_word("pmos"); return YYPMOS; }
<INITIAL>posedge        { save_word("posedge"); return YYPOSEDGE; }
<INITIAL>primitive      { save_word("primitive"); return YYPRIMITIVE; }
<INITIAL>pull0          { save_word("pull0"); return YYPULL0; }
<INITIAL>pull1          { save_word("pull1"); return YYPULL1; }
<INITIAL>pulldown       { save_word("pulldown"); return YYPULLDOWN; }
<INITIAL>pullup         { save_word("pullup"); return YYPULLUP; }
<INITIAL>rcmos          { save_word("rcmos"); return YYRCMOS; }
<INITIAL>real           { save_word("real"); return YYREAL; }
<INITIAL>realtime       { save_word("realtime"); return YYREALTIME; }
<INITIAL>reg            { save_word("reg"); return YYREG; }
<INITIAL>release        { save_word("release"); return YYRELEASE; }
<INITIAL>repeat         { save_word("repeat"); return YYREPEAT; }
<INITIAL>rnmos          { save_word("rnmos"); return YYRNMOS; }
<INITIAL>rpmos          { save_word("rpmos"); return YYRPMOS; }
<INITIAL>rtran          { save_word("rtran"); return YYRTRAN; }
<INITIAL>rtranif0       { save_word("rtranif0"); return YYRTRANIF0; }
<INITIAL>rtranif1       { save_word("rtranif1"); return YYRTRANIF1; }
<INITIAL>scalared       { save_word("scalared"); return YYSCALARED; }
<INITIAL>signed         { save_word("signed"); return YYSIGNED; }
<INITIAL>small          { save_word("small"); return YYSMALL; }
<INITIAL>specify        { save_word("specify"); return YYSPECIFY; }
<INITIAL>specparam      { save_word("specparam"); return YYSPECPARAM; }
<INITIAL>strong0        { save_word("strong0"); return YYSTRONG0; }
<INITIAL>strong1        { save_word("strong1"); return YYSTRONG1; }
<INITIAL>supply0        { save_word("supply0"); return YYSUPPLY0; }
<INITIAL>supply1        { save_word("supply1"); return YYSUPPLY1; }
<INITIAL>table          { save_word("table"); return YYTABLE; }
<INITIAL>task           { save_word("task"); return YYTASK; }
<INITIAL>time           { save_word("time"); return YYTIME; }
<INITIAL>tran           { save_word("tran"); return YYTRAN; }
<INITIAL>tranif0        { save_word("tranif0"); return YYTRANIF0; }
<INITIAL>tranif1        { save_word("tranif1"); return YYTRANIF1; }
<INITIAL>tri            { save_word("tri"); return YYTRI; }
<INITIAL>tri0           { save_word("tri0"); return YYTRI0; }
<INITIAL>tri1           { save_word("tri1"); return YYTRI1; }
<INITIAL>triand         { save_word("triand"); return YYTRIAND; }
<INITIAL>trior          { save_word("trior"); return YYTRIOR; }
<INITIAL>trireg         { save_word("trireg"); return YYTRIREG; }
<INITIAL>vectored       { save_word("vectored"); return YYVECTORED; }
<INITIAL>wait           { save_word("wait"); return YYWAIT; }
<INITIAL>wand           { save_word("wand"); return YYWAND; }
<INITIAL>weak0          { save_word("weak0"); return YYWEAK0; }
<INITIAL>weak1          { save_word("weak1"); return YYWEAK1; }
<INITIAL>wor            { save_word("wor"); return YYWOR; }
<INITIAL>while          { save_word("while"); return YYWHILE; }
<INITIAL>wire           { save_word("wire"); return YYWIRE; }
<INITIAL>xnor           { save_word("xnor"); return YYXNOR; }
<INITIAL>xor            { save_word("xor"); return YYXOR; }


	/*
	 * Others.
	 */


<INITIAL>\${Word}		{ copy_word(yytext); return YYSYSIDENT; }
<INITIAL>{Binary}		{ copy_word(yytext); return YYSIZBIN; }
<INITIAL>{Octal}		{ copy_word(yytext); return YYSIZOCT; }
<INITIAL>{Decimal}		{ copy_word(yytext); return YYSIZDEC; }
<INITIAL>{Hexdecimal}	{ copy_word(yytext); return YYSIZHEX; }
<INITIAL>{Number}		{ copy_word(yytext); return YYNUMBER; }
<INITIAL>{Float}		{ copy_word(yytext); return YYFLOAT; }
<INITIAL>{FloatE}		{ copy_word(yytext); return YYFLOAT; }
<INITIAL>{Word}			{ copy_word(yytext); return YYCHARSTR; }
<INITIAL>{WordDot}		{ copy_word(yytext); return YYCHARSTRDOT; }
<INITIAL>{QoutedString}	{
							/* Check for an escaped close qoute. */
							if (yytext[yyleng-2] == '\\')
								{
								yyless(yyleng-1); /* Return last qoute. */
								yymore(); /* Append next string. */
								}
							else
								{
								copy_word(yytext);
								return YYQSTRING;
								}
						}

<INITIAL>\`celldefine	{ save_word(yytext); BEGIN DIRECTIVE; }
<INITIAL>\`default_nettype { save_word(yytext); BEGIN DIRECTIVE; }
<INITIAL>\`define		{ save_word(yytext); BEGIN DIRECTIVE; }
<INITIAL>\`else			{ }
<INITIAL>\`endcelldefine { }
<INITIAL>\`endif		{ }
<INITIAL>\`ifdef		{ save_word(yytext); BEGIN DIRECTIVE; }
<INITIAL>\`include		{ save_word(yytext); BEGIN DIRECTIVE; }
<INITIAL>\`nounconnected_drive { }
<INITIAL>\`resetall		{ }
<INITIAL>\`timescale	{ save_word(yytext); BEGIN DIRECTIVE; }
<INITIAL>\`unconnected_drive { }
<INITIAL>\`undef		{ BEGIN DIRECTIVE; }
<INITIAL>\`line			{ save_word(yytext); BEGIN DIRECTIVE; }
<INITIAL>\`{Word}		{ copy_word(yytext); return  YYCHARSTR; }

<INITIAL>.				{ return yytext[0]; }

<DIRECTIVE>{NL}			{ process_directive(yytext); cur_lineno++; }
<DIRECTIVE>"/*"			{ yyless(yyleng-2); process_directive(yytext); }
<DIRECTIVE>"//"			{ yyless(yyleng-2); process_directive(yytext); }
<DIRECTIVE>.			{ yymore(); }

<POUND>{WSs}			{ }
<POUND>{NL}				{ cur_lineno++; }
<POUND>{Number}			{ copy_word(yytext); BEGIN INITIAL; return YYNUMBER; }
<POUND>{Float}			{ copy_word(yytext); BEGIN INITIAL; return YYFLOAT; }
<POUND>{FloatE}			{ copy_word(yytext); BEGIN INITIAL; return YYFLOAT; }
<POUND>{Word}			{ copy_word(yytext); BEGIN INITIAL; return YYCHARSTR; }
<POUND>.				{ BEGIN INITIAL; return yytext[0]; }

%%

int
yywrap(void)
	{
	/* We only handle one file at a time.  So clean up current globals. */
	free(savedword);
	savedword = 0;

	/* Return no more files. */
	return 1;
	}

void
yyreset(void)
	{
#ifdef YY_FLUSH_BUFFER
	YY_FLUSH_BUFFER;
#endif
	BEGIN(INITIAL);
	}

void
copy_word(char *str)
	{
	yylval.simple_string = vbs_strdup(str);
	free(savedword);
	savedword = 0;
	}

void
save_word(char *str)
	{
	free(savedword);
	savedword = vbs_strdup(str);
	}

void
process_directive(char *text)
	{
	char *p;

	if (strcmp("`line", savedword) == 0)
		{
		while (*text == ' ' || *text == '\t')
			++text; /* Skip pass leading white spaces. */
		p = strtok(text, " \t\n");
		if (p != 0)
			{
			cur_lineno = atoi(p);
			cur_lineno -= 1; /* The line number represents the next line. */
			p = strtok(0, " \t\n");
			if (p != 0)
				{
				if (*p == '\"')
					{
					int i;
					++p;
					for (i = 0; p[i] != '\"'; ++i)
						;
					p[i] = '\0';
					}
				cur_filename = vbs_strdup(p);
				}
			}
		}

	/* Done with directive line, reset mode. */
	BEGIN INITIAL;
	}
