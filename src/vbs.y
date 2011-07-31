%{
/*
 * Verilog Behavioral Simulator
 * Copyright (C) 1995-1997,2001-2003 Lay Hoon Tho, Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * vbs.y
 *
 * Verilog Behavioral Simulator yacc grammar parser.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sim.h"
#include "parser.h"
#include "p_expr.h"
#include "p_stmt.h"
#include "p_misc.h"
#include "p_moditm.h"

%}

%union
	{
	int simple_keyword;
	void *not_used;
	void *not_supported;
	char *simple_string;

	/* Module declaration. */
	p_module module_declaration;
	p_port port;
	p_port_list port_list;
	p_module_item module_item;
	p_module_item_list module_item_list;

	/* Module items. */
	p_module_instantiation module_instantiation;
	p_continuous_assignment continuous_assignment;

	p_param_declaration param_declaration;
	p_io_declaration io_declaration;
	p_net_declaration net_declaration;
	p_reg_declaration reg_declaration;
	p_int_declaration int_declaration;
	p_function_declaration function_declaration;
	p_task_declaration task_declaration;

	p_decl_assign_list decl_assign_list;
	p_decl_assign decl_assign;

	/* Module instantiation. */
	p_module_instance_list module_instance_list;
	p_module_instance module_instance;
	p_port_connection_list port_connection_list;
	p_port_connection port_connection;
	p_blocking_assignment_list blocking_assignment_list;

	p_tfdecl_list tfdecl_list;
	p_tfdecl tfdecl;

	/* Statements. */
	p_statement statement;
	p_statement_list statement_list;
	p_seq_block seq_block;
	p_task_enable systask_enable;
	p_task_enable task_enable;
	p_blocking_assignment blocking_assignment;
	p_non_blocking_assignment non_blocking_assignment;
	p_lvalue lvalue;
	p_case_item case_item;
	p_case_item_list case_item_list;

	p_initial_construct initial_construct;
	p_always_construct always_construct;

	/* Delay or events. */
	p_event_expression event_expression;
	p_event_expression_list event_expression_list;
	p_delay_or_event_control delay_or_event_control;

	/* Expressions. */
	p_expression expression;
	p_expression_list expression_list;
	p_expression constant_expression;
	p_bit_select bit_select;
	p_range_identifier range_identifier;
	p_part_select part_select;
	p_range_identifier_list range_identifier_list;
	p_qouted_string qouted_string;
	p_function_call function_call;
	p_concatenation concatenation;
	p_mintypmax_expr mintypmax_expression;
	p_number number;
	};


	/*
	 * Symbols.
	 */


	/* Generic. */
%token YYPOUND
%token YYAT
%token YYMINUSGREATER
%token YYASTERICGREATER
%token YYEQUALGREATER

	/* Unary. */
%token YYEXCLAM
%token YYTILDE
%token YYAMPER
%token YYTILDEAMPER
%token YYTILDEVERTBAR
%token YYCARET
%token YYTILDECARET
%token YYCARETTILDE

	/* Binary. */
%token YYASTERIC
%token YYSLASH
%token YYPERCENT
%token YYEQUALEQUAL
%token YYEXCLAMEQUAL
%token YYEQUALEQUALEQUAL
%token YYEXCLAMEQUALEQUAL
%token YYAMPERAMPER
%token YYVERTBARVERTBAR
%token YYLESS
%token YYLESSEQUAL
%token YYGREATER
%token YYGREATEREQUAL
%token YYGREATERGREATER
%token YYLESSLESS

	/* Unary or binary. */
%token YYPLUS
%token YYMINUS
%token YYVERTBAR

	/* Ternary. */
%token YYQUESTION
%token YYCOLON


	/*
	 * Keywords.
	 */


%token YYALWAYS
%token YYAND
%token YYASSIGN
%token YYBEGIN
%token YYBUF
%token YYBUFIF0
%token YYBUFIF1
%token YYCASE
%token YYCASEX
%token YYCASEZ
%token YYCMOS
%token YYDEASSIGN
%token YYDEFAULT
%token YYDEFPARAM
%token YYDISABLE
%token YYEDGE
%token YYELSE
%token YYEND
%token YYENDCASE
%token YYENDFUNCTION
%token YYENDMODULE
%token YYENDPRIMITIVE
%token YYENDSPECIFY
%token YYENDTABLE
%token YYENDTASK
%token YYEVENT
%token YYFOR
%token YYFORCE
%token YYFOREVER
%token YYFORK
%token YYFUNCTION
%token YYHIGHZ0
%token YYHIGHZ1
%token YYIF
%token YYIFNONE
%token YYINITIAL
%token YYINOUT
%token YYINPUT
%token YYINTEGER
%token YYJOIN
%token YYLARGE
%token YYMACROMODULE
%token YYMEDIUM
%token YYMODULE
%token YYNAND
%token YYNEGEDGE
%token YYNMOS
%token YYNOR
%token YYNOT
%token YYNOTIF0
%token YYNOTIF1
%token YYOR
%token YYOUTPUT
%token YYPARAMETER
%token YYPMOS
%token YYPOSEDGE
%token YYPRIMITIVE
%token YYPULL0
%token YYPULL1
%token YYPULLDOWN
%token YYPULLUP
%token YYRCMOS
%token YYREAL
%token YYREALTIME
%token YYREG
%token YYRELEASE
%token YYREPEAT
%token YYRNMOS
%token YYRPMOS
%token YYRTRAN
%token YYRTRANIF0
%token YYRTRANIF1
%token YYSCALARED
%token YYSIGNED
%token YYSMALL
%token YYSPECIFY
%token YYSPECPARAM
%token YYSTRONG0
%token YYSTRONG1
%token YYSUPPLY0
%token YYSUPPLY1
%token YYTABLE
%token YYTASK
%token YYTIME
%token YYTRAN
%token YYTRANIF0
%token YYTRANIF1
%token YYTRI
%token YYTRI0
%token YYTRI1
%token YYTRIAND
%token YYTRIOR
%token YYTRIREG
%token YYVECTORED
%token YYWAIT
%token YYWAND
%token YYWEAK0
%token YYWEAK1
%token YYWHILE
%token YYWIRE
%token YYWOR
%token YYXNOR
%token YYXOR


	/*
	 * Others.
	 */


%token <simple_string> YYCHARSTR
%token <simple_string> YYCHARSTRDOT
%token <simple_string> YYSIZBIN /* sized bin integer */
%token <simple_string> YYSIZOCT /* sized oct integer */
%token <simple_string> YYSIZDEC /* sized dec integer */
%token <simple_string> YYSIZHEX /* sized hex integer */
%token <simple_string> YYNUMBER /* decimal integer */
%token <simple_string> YYFLOAT /* float */
%token <simple_string> YYQSTRING /* qouted string, i.e. "abc" */
%token <simple_string> YYSYSIDENT /* system task or function enable */


	/*
	 * Precedence.
	 */


%right YYCOLON
%nonassoc YYQUESTION
%left YYOR
%left YYVERTBARVERTBAR
%left YYAMPERAMPER
%left YYVERTBAR YYTILDEVERTBAR
%left YYCARET YYTILDECARET YYCARETTILDE
%left YYAMPER YYTILDEAMPER
%left YYEQUALEQUAL YYEXCLAMEQUAL YYEQUALEQUALEQUAL YYEXCLAMEQUALEQUAL
%left YYGREATEREQUAL YYLESSEQUAL YYGREATER YYLESS
%left YYLESSLESS YYGREATERGREATER
%left YYPLUS YYMINUS
%left YYASTERIC YYSLASH YYPERCENT
%right YYTILDE YYEXCLAM
%left YYUNARY_PREC
%nonassoc LT_YYELSE
%nonassoc YYELSE


	/*
	 * UDP declaration.
	 */


%type <not_supported> udp_declaration


	/*
	 * Module declaration.
	 */


%type <module_declaration> module_declaration
%type <simple_string> module_identifier
%type <port_list> list_of_ports_opt
%type <port_list> list_of_ports
%type <port_list> port_brace
%type <port> port
%type <port> port_expression
%type <port> port_expression_opt
%type <port> port_reference
%type <port> port_reference_brace
%type <simple_string> port_identifier
%type <bit_select> bit_select
%type <part_select> part_select


	/*
	 * Module items.
	 */


%type <module_item_list> module_item_brace
%type <module_item> module_item
%type <module_item> module_item_declaration
%type <not_supported> parameter_override
%type <continuous_assignment> continuous_assign
%type <not_supported> gate_instantiation
%type <not_supported> udp_instantiation
%type <module_instantiation> module_instantiation
%type <not_supported> specify_block
%type <initial_construct> initial_construct
%type <always_construct> always_construct

%type <param_declaration> parameter_declaration
%type <io_declaration> input_declaration
%type <io_declaration> output_declaration
%type <io_declaration> inout_declaration
%type <reg_declaration> reg_declaration
%type <int_declaration> time_declaration
%type <int_declaration> integer_declaration
%type <int_declaration> real_declaration
%type <int_declaration> realtime_declaration
%type <int_declaration> event_declaration
%type <net_declaration> net_declaration
%type <function_declaration> function_declaration
%type <task_declaration> task_declaration

%type <decl_assign_list> list_of_sdecl_identifiers
%type <decl_assign_list> list_of_sdecl_assignments
%type <decl_assign> sdecl_assignments
%type <range_identifier> sdecl_identifier
%type <decl_assign_list> list_of_mdecl_identifiers
%type <decl_assign_list> list_of_mdecl_assignments
%type <decl_assign> mdecl_assignments
%type <range_identifier> register_name
%type <simple_string> register_identifier
%type <simple_string> memory_identifier
%type <range_identifier_list> list_of_port_identifiers

%type <not_supported> signed_opt
%type <not_supported> v_or_s
%type <not_supported> drive_strength_opt
%type <not_supported> charge_strength_opt

%type <delay_or_event_control> delay3_opt
%type <delay_or_event_control> delay3
%type <delay_or_event_control> delay2_opt
%type <delay_or_event_control> delay2
%type <delay_or_event_control> delay_value
%type <simple_string> param_identifier

%type <part_select> range_opt
%type <part_select> range
%type <simple_keyword> net_type

%type <part_select> range_or_type_opt
%type <part_select> range_or_type

%type <tfdecl_list> function_item_declaration_brace
%type <tfdecl> function_item_declaration
%type <tfdecl_list> task_item_declaration_brace
%type <tfdecl> task_item_declaration
%type <tfdecl> block_item_declaration


	/*
	 * Module instantiastion.
	 */


%type <module_instance_list> module_instance_brace
%type <expression_list> param_value_assign_opt
%type <module_instance> module_instance
%type <simple_string> name_of_instance
%type <port_connection_list> list_of_module_connections_opt
%type <port_connection_list> list_of_module_connections
%type <port_connection_list> ordered_port_connection_brace
%type <port_connection> ordered_port_connection
%type <port_connection_list> named_port_connection_brace
%type <port_connection> named_port_connection


	/*
	 * Behavioral statements.
	 */


%type <blocking_assignment_list> list_of_type_assignments
%type <blocking_assignment> type_assignment

%type <statement> statement_or_null
%type <statement> statement
%type <statement> procedural_timing_control_statement
%type <not_supported> procedural_continuous_statement
%type <statement> conditional_statement
%type <statement> case_statement
%type <statement> loop_statement
%type <statement> wait_statement
%type <statement> disable_statement
%type <statement> event_trigger
%type <seq_block> seq_block
%type <not_supported> par_block
%type <not_supported> block_item_opt

%type <statement_list> statement_brace
%type <blocking_assignment> blocking_assign
%type <non_blocking_assignment> non_blocking_assign
%type <case_item_list> case_item_brace
%type <case_item> case_item
%type <systask_enable> system_task_enable
%type <task_enable> task_enable

%type <delay_or_event_control> delay_or_event_cntl_opt
%type <delay_or_event_control> delay_or_event_cntl
%type <delay_or_event_control> delay_control
%type <delay_or_event_control> event_control
%type <simple_string> event_identifier
%type <event_expression> event_expression
%type <event_expression_list> ored_event_expression

%type <simple_string> system_task_name
%type <simple_string> task_identifier


	/*
	 * Expressions.
	 */


%type <lvalue> reg_lvalue
%type <simple_string> reg_identifier
%type <expression_list> expression_brace
%type <expression> expression_opt
%type <expression> expression
%type <expression> primary
%type <constant_expression> const_expression
%type <number> number
%type <number> decimal_number
%type <number> binary_number
%type <number> octal_number
%type <number> hex_number
%type <number> real_number
%type <number> unsigned_number

%type <mintypmax_expression> mintypmax_expression
%type <function_call> function_call
%type <concatenation> concatenation
%type <concatenation> multiple_concatenation
%type <expression_list> reg_concatenation
%type <simple_string> function_identifier
%type <simple_string> name_of_system_function
%type <expression_list> expression_brace_opt

%type <simple_string> identifier

%%


	/*
	 * Starting point.
	 */


source_text:
		description_brace
			{ /* Not used. */ }
	;

description_brace:
		/* Optional. */
			{ /* Not used. */ }
	|	description_brace description
			{ /* Not used. */ }
	;

description:
		module_declaration
			/* Save the module so we can let yywrap handle it. */
			{ (*sim_parsed_module)($1); }
	|	udp_declaration
			{ /* Empty! */ }
	;


	/*
	 * Module declaration.
	 */


module_declaration:
		module_keyword module_identifier list_of_ports_opt ';'
				module_item_brace
		YYENDMODULE
			{ $$ = p_create_module($2, $5, $3); }
	;

module_keyword:
		/* VBS does not differentiation between the two keywords. */
		YYMODULE
			{ /* Not used. */ }
	|	YYMACROMODULE
			{ /* Not used. */ }
	;

module_identifier:
		YYCHARSTR
			{ $$ = $1; }
	;

list_of_ports_opt:
		/* Optional. */
			{ $$ = 0; }
	|	list_of_ports
			{ $$ = $1; }
	;

list_of_ports:
		'(' port_brace ')'
			{ $$ = $2; }
	;

port_brace:
		port
			{ $$ = p_create_port_list(0, $1); }
	|	port_brace ',' port
			{ $$ = p_create_port_list($1, $3); }
	;

port:
		port_expression_opt
			{ $$ = $1; }
	|	'.' port_identifier '(' port_expression_opt ')'
			{ $$ = 0; yyerror(" named port "); }
	;

port_expression_opt:
		/* Optional. */
			{ $$ = 0; }
	|	port_expression
			{ $$ = $1; }
	;

port_expression:
		port_reference
			{ $$ = $1; }
	|	'{' port_reference_brace '}'
			{ $$ = 0; yyerror(" { port reference list } "); }
	;

port_reference:
		port_identifier
			{ $$ = p_create_port($1); }
	|	port_identifier bit_select
			{ $$ = 0; yyerror(" port_identifier [cexp] "); }
	|	port_identifier part_select
			{ $$ = 0; yyerror(" port_identifier [cexp:cexp] "); }
	;

port_reference_brace:
		port_reference
			{ $$ = $1; }
	|	port_reference_brace ',' port_reference
			{ $$ = 0; }
	;

bit_select:
		'[' expression ']'
			{ $$ = p_create_bit_select($2); }
	;

part_select:
		'[' const_expression YYCOLON const_expression ']'
			{ $$ = p_create_part_select($2, $4); }
	;

port_identifier:
		YYCHARSTR
			{ $$ = $1; }
	;


	/*
	 * Module items.
	 */


module_item_brace:
		/* Optional. */
			{ $$ = p_create_module_item_list(0, 0); }
	|	module_item_brace module_item
			{ $$ = p_create_module_item_list($1, $2); }
	;

module_item:
		module_item_declaration
			{ $$ = $1; }
	|	parameter_override
			{ $$ = $1; yyerror(" parameter override "); }
	|	continuous_assign
			{ $$ = $1; }
	|	gate_instantiation
			{ $$ = $1; yyerror(" gate instantiation "); }
	|	udp_instantiation
			{ $$ = $1; yyerror(" udp instantiation "); }
	|	module_instantiation
			{ $$ = $1; }
	|	specify_block
			{ $$ = $1; yyerror(" specify block "); }
	|	initial_construct
			{ $$ = $1; }
	|	always_construct
			{ $$ = $1; }
	;

module_item_declaration:
		parameter_declaration
			{ $$ = $1; }
	|	input_declaration
			{ $$ = $1; }
	|	output_declaration
			{ $$ = $1; }
	|	inout_declaration
			{ $$ = $1; }
	|	net_declaration
			{ $$ = $1; }
	|	reg_declaration
			{ $$ = $1; }
	|	integer_declaration
			{ $$ = $1; }
	|	real_declaration
			{ $$ = $1; }
	|	time_declaration
			{ $$ = $1; }
	|	realtime_declaration
			{ $$ = $1; }
	|	event_declaration
			{ $$ = $1; }
	|	task_declaration
			{ $$ = $1; }
	|	function_declaration
			{ $$ = $1; }
	;

parameter_override:
		YYDEFPARAM list_of_sdecl_assignments ';'
			{ $$ = 0; }
	;


	/*
	 * Declarations.
	 */


list_of_sdecl_assignments:
		sdecl_assignments
			{ $$ = p_create_decl_assign_list(0, $1); }
	|	list_of_sdecl_assignments ',' sdecl_assignments
			{ $$ = p_create_decl_assign_list($1, $3); }
	;

sdecl_assignments:
		sdecl_identifier '=' expression
			{ $$ = p_create_decl_assign($1, $3); }
	;

sdecl_identifier:
		YYCHARSTR
			{ $$ = p_create_range_identifier($1, 0); }
	|	YYCHARSTRDOT
			{ $$ = p_create_range_identifier($1, 0); }
	;

list_of_sdecl_identifiers:
		sdecl_identifier
			{
			p_decl_assign ra;
			ra = p_create_decl_assign($1, 0);
			$$ = p_create_decl_assign_list(0, ra);
			}
	|	list_of_sdecl_identifiers ',' sdecl_identifier
			{
			p_decl_assign ra;
			ra = p_create_decl_assign($3, 0);
			$$ = p_create_decl_assign_list($1, ra);
			}
	;

list_of_mdecl_assignments:
		mdecl_assignments
			{ $$ = p_create_decl_assign_list(0, $1); }
	|	list_of_mdecl_assignments ',' mdecl_assignments
			{ $$ = p_create_decl_assign_list($1, $3); }
	;

mdecl_assignments:
		register_name '=' expression
			{ $$ = p_create_decl_assign($1, $3); }
	;

list_of_mdecl_identifiers:
		register_name
			{
			p_decl_assign ra;
			ra = p_create_decl_assign($1, 0);
			$$ = p_create_decl_assign_list(0, ra);
			}
	|	list_of_mdecl_identifiers ',' register_name
			{
			p_decl_assign ra;
			ra = p_create_decl_assign($3, 0);
			$$ = p_create_decl_assign_list($1, ra);
			}
	;

register_name:
		register_identifier
			{ $$ = p_create_range_identifier($1, 0); }
	|	memory_identifier part_select
			{ $$ = p_create_range_identifier($1, $2); }
	;

register_identifier:
		YYCHARSTR
			{ $$ = $1; }
	;

memory_identifier:
		YYCHARSTR
			{ $$ = $1; }
	;

parameter_declaration:
		YYPARAMETER range_opt list_of_sdecl_assignments ';'
			{ $$ = p_create_param_declaration($3, $2); }
	;

input_declaration:
		YYINPUT range_opt list_of_port_identifiers ';'
			{ $$ = p_create_io_declaration(YYINPUT, $3, $2); }
	;

output_declaration:
		YYOUTPUT range_opt list_of_port_identifiers ';'
			{ $$ = p_create_io_declaration(YYOUTPUT, $3, $2); }
	;

inout_declaration:
		YYINOUT range_opt list_of_port_identifiers ';'
			{ $$ = p_create_io_declaration(YYINOUT, $3, $2); }
	;

list_of_port_identifiers:
		port_identifier
			{
			p_range_identifier tmp;
			tmp = p_create_range_identifier($1, 0);
			$$ = p_create_range_identifier_list(0, tmp);
			}
	|	list_of_port_identifiers ',' port_identifier
			{
			p_range_identifier tmp;
			tmp = p_create_range_identifier($3, 0);
			$$ = p_create_range_identifier_list($1, tmp);
			}
	;

signed_opt:
		/* Optional. */
			{ $$ = 0; }
	|	YYSIGNED
			{ $$ = 0; yyerror(" signed declaration "); }
	;

reg_declaration:
		YYREG signed_opt range_opt list_of_mdecl_identifiers ';'
			{ $$ = p_create_reg_declaration($4, $3); }
	|	YYREG signed_opt range_opt list_of_mdecl_assignments ';'
			{ $$ = p_create_reg_declaration($4, $3); }
	;

time_declaration:
		YYTIME list_of_mdecl_identifiers ';'
			{ $$ = p_create_int_declaration($2); }
	|	YYTIME list_of_mdecl_assignments ';'
			{ $$ = p_create_int_declaration($2); }
	;

integer_declaration:
		YYINTEGER list_of_mdecl_identifiers ';'
			{ $$ = p_create_int_declaration($2); }
	|	YYINTEGER list_of_mdecl_assignments ';'
			{ $$ = p_create_int_declaration($2); }
	;

real_declaration:
		YYREAL list_of_sdecl_identifiers ';'
			{
			$$ = p_create_int_declaration($2);
			yyerror(" real declaration ");
			}
	;

realtime_declaration:
		YYREALTIME list_of_sdecl_identifiers ';'
			{
			$$ = p_create_int_declaration($2);
			yyerror(" realtime declaration ");
			}
	;

event_declaration:
		YYEVENT list_of_sdecl_identifiers ';'
			{
			$$ = p_create_int_declaration($2);
			yyerror(" event declaration ");
			}
	;

range_opt:
		/* Optional. */
			{ $$ = 0; }
	|	range
			{ $$ = $1; }
	;

range:
		part_select
			{ $$ = $1; }
	;

v_or_s:
			/* Optional. */
			{ $$ = 0; }
	|	YYVECTORED
			{ $$ = 0; yyerror(" vectored "); }
	|	YYSCALARED
			{ $$ = 0; yyerror(" scalared "); }
	;

net_declaration:
		net_type v_or_s range_opt delay3_opt
				list_of_sdecl_identifiers ';'
			{
			if ($4 != 0)
				yyerror(" delay3 ");
			$$ = p_create_net_declaration($1, $5, $3);
			}
	|	YYTRIREG v_or_s charge_strength_opt range_opt delay3_opt
				list_of_sdecl_identifiers ';'
			{
			yyerror(" trireg declaration ");
			$$ = 0;
			}
	|	net_type v_or_s range_opt delay3_opt
				list_of_sdecl_assignments ';'
			{
			if ($4 != 0)
				yyerror(" delay3 ");
			$$ = p_create_net_declaration($1, $5, $3);
			}
	|	net_type v_or_s drive_strength range_opt delay3_opt
				list_of_sdecl_assignments ';'
			{
			/* Must flatten out to avoid shift/reduce error. */
			if ($5 != 0)
				yyerror(" delay3 ");
			$$ = p_create_net_declaration($1, $6, $4);
			}
	;

net_type:
		YYWIRE
			{ $$ = YYWIRE; }
	|	YYTRI
			{ $$ = YYTRI; }
	|	YYTRI1
			{ $$ = YYTRI1; }
	|	YYSUPPLY0
			{ $$ = YYSUPPLY0; }
	|	YYWAND
			{ $$ = YYWAND; }
	|	YYTRIAND
			{ $$ = YYTRIAND; }
	|	YYTRI0
			{ $$ = YYTRI0; }
	|	YYSUPPLY1
			{ $$ = YYSUPPLY1; }
	|	YYWOR
			{ $$ = YYWOR; }
	|	YYTRIOR
			{ $$ = YYTRIOR; }
	;

drive_strength_opt:
		/* Optional. */
			{ $$ = 0; }
	|	drive_strength
			{ $$ = 0; yyerror(" drive strength "); }
	;

drive_strength:
		'(' strength0 ',' strength1 ')'
			{ }
	|	'(' strength1 ',' strength0')'
			{ }
	|	'(' strength0 ',' YYHIGHZ1 ')'
			{ }
	|	'(' strength1 ',' YYHIGHZ0 ')'
			{ }
	|	'(' YYHIGHZ1 ',' strength0 ')'
			{ }
	|	'(' YYHIGHZ0 ',' strength1 ')'
			{ }
	;

strength0:
		YYSUPPLY0
	|	YYSTRONG0
	|	YYPULL0
	|	YYWEAK0
	;

strength1:
		YYSUPPLY1
	|	YYSTRONG1
	|	YYPULL1
	|	YYWEAK1
	;

charge_strength_opt:
		/* Optional. */
			{ $$ = 0; }
	|	charge_strength
			{ $$ = 0; yyerror(" charge strength "); }
	;

charge_strength:
		'(' YYSMALL ')'
	|	'(' YYMEDIUM ')'
	|	'(' YYLARGE ')'
	;

delay3_opt:
		/* Optional. */
			{ $$ = 0; }
	|	delay3
			{ $$ = $1; }
	;

delay3:
		delay2
			{ $$ = $1; }
	|	YYPOUND '(' delay_value ',' delay_value ',' delay_value ')'
			{ $$ = $3; yyerror(" #(integer,integer,integer) "); }
	;

delay2_opt:
		/* Optional. */
			{ $$ = 0; }
	|	delay2
			{ $$ = $1; }
	;

delay2:
		YYPOUND delay_value
			{ $$ = $2; }
	|	YYPOUND '(' delay_value ')'
			{ $$ = $3; }
	|	YYPOUND '(' delay_value ',' delay_value ')'
			{ $$ = $3; yyerror(" #(integer,integer) "); }
	;

delay_value:
		unsigned_number
			{ $$ = p_create_delay_number($1); }
	|	real_number
			{ $$ = p_create_delay_number($1); yyerror(" #real "); }
	|	param_identifier
			{
			p_range_identifier tmp = p_create_range_identifier($1, 0);
			$$ = p_create_delay_identifier(tmp);
			}
	;

param_identifier:
		YYCHARSTR
			{ $$ = $1; }
	;

function_declaration:
		YYFUNCTION range_or_type_opt function_identifier ';'
				function_item_declaration_brace
				statement
		YYENDFUNCTION
			{ $$ = p_create_function_declaration($3, $6, $5, $2); }
	;

range_or_type_opt:
		/* Optional. */
			{ $$ = 0; }
	|	range_or_type
			{ $$ = $1; }
	;

range_or_type:
		range
			{ $$ = $1; }
	|	YYINTEGER
			{ $$ = 0; yyerror(" function integer return "); }
	|	YYREAL
			{ $$ = 0; yyerror(" function real return "); }
	|	YYREALTIME
			{ $$ = 0; yyerror(" function realtime return "); }
	|	YYTIME
			{ $$ = 0; yyerror(" function time return "); }
	;

function_item_declaration_brace:
		function_item_declaration
			{ $$ = p_create_tfdecl_list(0, $1); }
	|	function_item_declaration_brace function_item_declaration
			{ $$ = p_create_tfdecl_list($1, $2); }
	;

function_item_declaration:
		block_item_declaration
			{ $$ = $1; }
	|	input_declaration
			{ $$ = $1; }
	;

task_declaration:
		YYTASK task_identifier ';'
				task_item_declaration_brace
				statement_or_null
		YYENDTASK
			{ $$ = p_create_task_declaration($2, $5, $4); }
	;


task_item_declaration_brace:
		/* Optional. */
			{ $$ = p_create_tfdecl_list(0, 0); }
	|	task_item_declaration_brace task_item_declaration
			{ $$ = p_create_tfdecl_list($1, $2); }
	;

task_item_declaration:
		block_item_declaration
			{ $$ = $1; }
	|	input_declaration /* Not conforming to IEEE-1364. */
			{ $$ = $1; }
	|	output_declaration
			{ $$ = $1; }
	|	inout_declaration
			{ $$ = $1; }
	;

block_item_declaration:
		parameter_declaration
			{ $$ = $1; }
	|	reg_declaration
			{ $$ = $1; }
	|	integer_declaration
			{ $$ = $1; }
	;


	/*
	 * Primitive instances.
	 */


gate_instantiation:
		n_input_gatetype
			{ yyerror(" [x][n][and|or] "); }
		drive_strength_opt delay2_opt n_input_gate_instance_brace ';'
			{ $$ = 0; }
	|	n_output_gatetype
			{ yyerror(" buf|not "); }
		drive_strength_opt delay2_opt n_output_gate_instance_brace ';'
			{ $$ = 0; }
	|	enable_gatetype
			{ yyerror(" [buf|not]if[01] "); }
		drive_strength_opt delay3_opt enable_gate_instance_brace ';'
			{ $$ = 0; }
	|	mos_switchtype
			{ yyerror(" [r][np]mos "); }
		delay3_opt mos_switch_instance_brace ';'
			{ $$ = 0; }
	|	pass_switchtype
			{ yyerror(" tran "); }
		pass_switch_instance_brace ';'
			{ $$ = 0; }
	|	pass_en_switchtype
			{ yyerror(" [r]tranif[01] "); }
		delay3_opt pass_en_switch_instance_brace ';'
			{ $$ = 0; }
	|	cmos_switchtype
			{ yyerror(" [r]cmos "); }
		delay3_opt cmos_switch_instance_brace ';'
			{ $$ = 0; }
	|	YYPULLUP
			{ yyerror(" pullup "); }
		pullup_strength_opt pull_gate_instance_brace ';'
			{ $$ = 0; }
	|	YYPULLDOWN
			{ yyerror(" pulldown "); }
		pulldown_strength_opt pull_gate_instance_brace ';'
			{ $$ = 0; }
	;

n_input_gate_instance_brace:
		n_input_gate_instance
			{ }
	|	n_input_gate_instance_brace ',' n_input_gate_instance
			{ }
	;

n_output_gate_instance_brace:
		n_output_gate_instance
			{ }
	|	n_output_gate_instance_brace ',' n_output_gate_instance
			{ }
	;

enable_gate_instance_brace:
		enable_gate_instance
			{ }
	|	enable_gate_instance_brace ',' enable_gate_instance
			{ }
	;

mos_switch_instance_brace:
		mos_switch_instance
			{ }
	|	mos_switch_instance_brace ',' mos_switch_instance
			{ }
	;

pass_switch_instance_brace:
		pass_switch_instance
			{ }
	|	pass_switch_instance_brace ',' pass_switch_instance
			{ }
	;

pass_en_switch_instance_brace:
		pass_en_switch_instance
			{ }
	|	pass_en_switch_instance_brace ',' pass_en_switch_instance
			{ }
	;

cmos_switch_instance_brace:
		cmos_switch_instance
			{ }
	|	cmos_switch_instance_brace ',' cmos_switch_instance
			{ }
	;

pull_gate_instance_brace:
		pull_gate_instance
			{ }
	|	pull_gate_instance_brace ',' pull_gate_instance
			{ }
	;

n_input_gate_instance:
		name_of_gate_instance_opt
				'(' output_terminal ',' input_terminal_brace ')'
			{ }
	;

n_output_gate_instance:
		name_of_gate_instance_opt
				'(' output_terminal_brace ',' input_terminal ')'
			{ }
	;

enable_gate_instance:
		name_of_gate_instance_opt
				'(' output_terminal ',' input_terminal ',' enable_terminal ')'
			{ }
	;

mos_switch_instance:
		name_of_gate_instance_opt
				'(' output_terminal ',' input_terminal ',' enable_terminal ')'
			{ }
	;

pass_switch_instance:
		name_of_gate_instance_opt
				'(' inout_terminal ',' inout_terminal ')'
			{ }
	;

pass_en_switch_instance:
		name_of_gate_instance_opt
				'(' inout_terminal ',' inout_terminal ',' enable_terminal ')'
			{ }
	;

cmos_switch_instance:
		name_of_gate_instance_opt
				'(' output_terminal ',' input_terminal ','
					ncontrol_terminal ',' pcontrol_terminal ')'
			{ }
	;

pull_gate_instance:
		name_of_gate_instance_opt
				'(' output_terminal ')'
			{ }
	;

name_of_gate_instance_opt:
		/*
		 * FIXME:  shift/reduce problem.
		 * Yacc couldn't decide whether a '(' is the start of a drive
		 * strength or a terminal list.  But if we require an instance
		 * name, than we know it is a drive strength.
		 */
		/* Optional. */
		/*	{ }
	|*/	name_of_gate_instance
			{ }
	;

name_of_gate_instance:
		identifier range_opt
			{ }
	;

pullup_strength_opt:
		/* Optional. */
			{ }
	|	pullup_strength
			{ }
	;

pullup_strength:
		'(' strength0 ',' strength1 ')'
			{ }
	|	'(' strength1 ',' strength0 ')'
			{ }
	|	'(' strength1 ')'
			{ }
	;

pulldown_strength_opt:
		/* Optional. */
			{ }
	|	pulldown_strength
			{ }
	;

pulldown_strength:
		'(' strength0 ',' strength1 ')'
			{ }
	|	'(' strength1 ',' strength0 ')'
			{ }
	|	'(' strength0 ')'
			{ }
	;

scalar_expression:
		expression
			{ }
	;

input_terminal_brace:
		input_terminal
			{ }
	|	input_terminal_brace ',' input_terminal
			{ }
	;

input_terminal:
		scalar_expression
			{ }
	;

enable_terminal:
		scalar_expression
			{ }
	;

ncontrol_terminal:
		scalar_expression
			{ }
	;

pcontrol_terminal:
		scalar_expression
			{ }
	;

output_terminal_brace:
		output_terminal
			{ }
	|	output_terminal_brace ',' output_terminal
			{ }
	;

output_terminal:
		identifier
			{ }
	|	identifier bit_select
			{ }
	;

inout_terminal:
		identifier
			{ }
	|	identifier bit_select
			{ }
	;

n_input_gatetype:
		YYAND
			{ }
	|	YYNAND
			{ }
	|	YYOR
			{ }
	|	YYNOR
			{ }
	|	YYXOR
			{ }
	|	YYXNOR
			{ }
	;

n_output_gatetype:
		YYBUF
			{ }
	|	YYNOT
			{ }
	;

enable_gatetype:
		YYBUFIF0
			{ }
	|	YYBUFIF1
			{ }
	|	YYNOTIF0
			{ }
	|	YYNOTIF1
			{ }
	;

mos_switchtype:
		YYNMOS
			{ }
	|	YYPMOS
			{ }
	|	YYRNMOS
			{ }
	|	YYRPMOS
			{ }
	;

pass_switchtype:
		YYTRAN
			{ }
	|	YYRTRAN
			{ }
	;

pass_en_switchtype:
		YYTRANIF0
			{ }
	|	YYTRANIF1
			{ }
	|	YYRTRANIF0
			{ }
	|	YYRTRANIF1
			{ }
	;

cmos_switchtype:
		YYCMOS
			{ }
	|	YYRCMOS
			{ }
	;


	/*
	 * Module instantiation.
	 */


module_instantiation:
		module_identifier param_value_assign_opt module_instance_brace ';'
			{ $$ = p_create_module_instantiation($1, $3, $2); }
	;

param_value_assign_opt:
		/* Optional. */
			{ $$ = 0; }
	|	YYPOUND unsigned_number
			{ $$ = 0; yyerror(" #value not conforming "); }
	|	YYPOUND '(' expression_brace ')'
			{ $$ = $3; }
	;

module_instance_brace:
		module_instance
			{ $$ = p_create_module_instance_list(0, $1); }
	|	module_instance_brace ',' module_instance
			{ $$ = p_create_module_instance_list($1, $3); }
	;

module_instance:
		name_of_instance '(' list_of_module_connections_opt ')'
			{ $$ = p_create_module_instance($1, $3); }
	;

name_of_instance:
		YYCHARSTR
			{ $$ = $1; }
	;

list_of_module_connections_opt:
		/* Optional. */
			/*{ $$ = 0; } Discarded if entire list is blank.
	|*/	list_of_module_connections
			{ $$ = $1; }
	;

list_of_module_connections:
		ordered_port_connection_brace
			{ $$ = $1; }
	|	named_port_connection_brace
			{ $$ = $1; }
	;

ordered_port_connection_brace:
		ordered_port_connection
			{ $$ = p_create_port_connection_list(0, $1); }
	|	ordered_port_connection_brace ',' ordered_port_connection
			{ $$ = p_create_port_connection_list($1, $3); }
	;

ordered_port_connection:
		expression_opt
			{ $$ = p_create_port_connection($1); }
	;

named_port_connection_brace:
		named_port_connection
			{ $$ = p_create_port_connection_list(0, $1); }
	|	named_port_connection_brace ',' named_port_connection
			{ $$ = p_create_port_connection_list($1, $3); }
	;

named_port_connection:
		'.' port_identifier '(' expression_opt ')'
			{ $$ = p_create_port_connection($4); /* FIXME: simplification! */ }
	;


	/*
	 * UDP declaration and instantiation
	 */


udp_declaration:
		YYPRIMITIVE identifier
			{ yyerror(" udp declaration "); }
		'(' udp_port_list ')' ';'
			udp_port_declaration_brace
			udp_body
		YYENDPRIMITIVE
			{ $$ = 0; }
	;

udp_port_list:
		port_brace
			{ }
	;

udp_port_declaration_brace:
		output_declaration
			{ }
	|	input_declaration
			{ }
	|	reg_declaration
			{ }
	;

udp_body:
		combinational_body
			{ }
	|	sequential_body
			{ }
	;

combinational_body:
		YYTABLE YYENDTABLE
			{ }
	;

sequential_body:
		initial_construct YYTABLE YYENDTABLE
			{ }
	;

udp_instantiation:
		/*
		 * FIXME:  shift/reduce problem.
		 * Without drive strength, a UDP instantiation looks like a
		 * module instantiation.
		 */
		udp_identifier drive_strength delay2_opt udp_instance_brace ';'
			{ $$ = 0; }
	;

udp_identifier:
		YYCHARSTR
			{ }
	;

udp_instance_brace:
		udp_instance
			{ }
	|	udp_instance_brace ',' udp_instance
			{ }
	;

udp_instance:
		name_of_udp_instance_opt '(' output_port_connection ','
				input_port_connection_brace ')'
			{ }
	;

name_of_udp_instance_opt:
		/* Optional. */
			{ }
	|	identifier range_opt
			{ }
	;

output_port_connection:
		identifier
			{ }
	;

input_port_connection_brace:
		input_port_connection
			{ }
	|	input_port_connection_brace ',' input_port_connection
			{ }
	;

input_port_connection:
		identifier
			{ }
	;


	/*
	 * Behavioral statements.
	 */


continuous_assign:
		YYASSIGN drive_strength_opt delay3_opt list_of_type_assignments ';'
			{
			if ($2 != 0)
				yyerror(" drive strength ");
			$$ = p_create_continuous_assignment($4, $3);
			}
	;

list_of_type_assignments:
		type_assignment
			{ $$ = p_create_blocking_assignment_list(0, $1); }
	|	list_of_type_assignments ',' type_assignment
			{ $$ = p_create_blocking_assignment_list($1, $3); }
	;

type_assignment: /* net or reg */
		reg_lvalue '=' expression
			{ $$ = p_create_blocking_assignment($1, $3, 0); }
	;

initial_construct:
		YYINITIAL statement
			{ $$ = p_create_initial_construct($2); }
	;

always_construct:
		YYALWAYS statement
			{ $$ = p_create_always_construct($2); }
	;

statement:
		blocking_assign ';'
			{ $$ = $1; }
	|	non_blocking_assign ';'
			{ $$ = $1; }
	|	procedural_continuous_statement
			{ $$ = $1; yyerror(" procedural continuous assignment "); }
	|	procedural_timing_control_statement
			{ $$ = $1; }
	|	conditional_statement
			{ $$ = $1; }
	|	case_statement
			{ $$ = $1; }
	|	loop_statement
			{ $$ = $1; }
	|	wait_statement
			{ $$ = $1; yyerror(" wait statement "); }
	|	disable_statement
			{ $$ = $1; yyerror(" disable statement "); }
	|	event_trigger
			{ $$ = $1; yyerror(" event trigger "); }
	|	seq_block
			{ $$ = $1; }
	|	par_block
			{ $$ = $1; yyerror(" parallel block "); }
	|	task_enable
			{ $$ = $1; }
	|	system_task_enable
			{ $$ = $1; }
	;

statement_or_null:
		statement
			{ $$ = $1; }
	|	';'
			{ $$ = p_create_null_statement(); }
	;

blocking_assign:
		reg_lvalue '=' delay_or_event_cntl_opt expression
			{ $$ = p_create_blocking_assignment($1, $4, $3); }
	;

non_blocking_assign:
		reg_lvalue YYLESSEQUAL delay_or_event_cntl_opt expression
			{ $$ = p_create_non_blocking_assignment($1, $4, $3); }
	;

procedural_continuous_statement:
		YYASSIGN type_assignment ';'
			{ $$ = $2; }
	|	YYDEASSIGN reg_lvalue ';'
			{ $$ = $2; }
	|	YYFORCE type_assignment ';'
			{ $$ = $2; }
	|	YYRELEASE reg_lvalue ';'
			{ $$ = $2; }
	;

procedural_timing_control_statement:
		delay_or_event_cntl statement_or_null
			{ $$ = p_create_include_delay_or_event_control($2, $1); }
	;

delay_or_event_cntl_opt:
		/* Optional */
			{ $$ = 0; }
	|	delay_or_event_cntl
	;

delay_or_event_cntl:
		delay_control
	|	event_control
	;

delay_control:
		YYPOUND delay_value
			{ $$ = $2; }
	|	YYPOUND '(' mintypmax_expression ')'
			{ $$ = p_create_delay_identifier($3); }
	;

event_control:
		YYAT YYASTERIC
			{
			char *id = vbs_strdup("*");
			$$ = p_create_event_identifier(id);
			yyerror(" wild sense ");
			}
	|	YYAT '(' YYASTERIC ')'
			{
			char *id = vbs_strdup("*");
			$$ = p_create_event_identifier(id);
			yyerror(" wild sense ");
			}
	|	YYAT event_identifier
			{ $$ = p_create_event_identifier($2); }
	|	YYAT '(' ored_event_expression ')'
			{ $$ = p_create_or_event_expression($3); }
	;

event_identifier:
		YYCHARSTR
			{ $$ = $1; }
	;

ored_event_expression:
		event_expression
			{ $$ = p_create_event_expression_list(0, $1); }
	|	ored_event_expression YYOR event_expression
			{ $$ = p_create_event_expression_list($1, $3); }
	;

event_expression:
		expression
			{ $$ = p_create_change_event_expression($1); }
	|	YYPOSEDGE expression
			{ $$ = p_create_posedge_event_expression($2); }
	|	YYNEGEDGE expression
			{ $$ = p_create_negedge_event_expression($2); }
	;

conditional_statement:
		YYIF '(' expression ')' statement_or_null %prec LT_YYELSE
			{ $$ = p_create_if_else_statement($3, $5, 0); }
	|	YYIF '(' expression ')' statement_or_null YYELSE statement_or_null
			{ $$ = p_create_if_else_statement($3, $5, $7); }
	;

case_statement:
		YYCASE '(' expression ')' case_item_brace YYENDCASE
			{ $$ = p_create_case_statement(YYCASE, $3, $5); }
	|	YYCASEX '(' expression ')' case_item_brace YYENDCASE
			{ $$ = p_create_case_statement(YYCASEX, $3, $5); }
	|	YYCASEZ '(' expression ')' case_item_brace YYENDCASE
			{ $$ = p_create_case_statement(YYCASEZ, $3, $5); }
	;

case_item_brace:
		case_item
			{ $$ = p_create_case_item_list(0, $1); }
	|	case_item_brace case_item
			{ $$ = p_create_case_item_list($1, $2); }
	;

case_item:
		expression_brace YYCOLON statement_or_null
			{ $$ = p_create_case_item($1, $3); }
	|	YYDEFAULT YYCOLON statement_or_null
			{ $$ = p_create_case_item(0, $3); }
	|	YYDEFAULT statement_or_null
			{ $$ = p_create_case_item(0, $2); }
	;

loop_statement:
		YYFOREVER
			statement
			{ $$ = p_create_loop_statement(YYFOREVER, $2, 0, 0, 0); }
	|	YYREPEAT '(' expression ')'
			statement
			{ $$ = p_create_loop_statement(YYREPEAT, $5, $3, 0, 0); }
	|	YYWHILE '(' expression ')'
			statement
			{ $$ = p_create_loop_statement(YYWHILE, $5, $3, 0, 0); }
	|	YYFOR '(' blocking_assign ';' expression ';' blocking_assign ')'
			statement
			{ $$ = p_create_loop_statement(YYFOR, $9, $5, $3, $7); }
	;

wait_statement:
		YYWAIT '(' expression ')' statement_or_null
			{ $$ = $5; }
	;

event_trigger:
		YYMINUSGREATER event_identifier ';'
			{ $$ = p_create_null_statement(); }
	;

disable_statement: /* block_identifier is the same thing. */
		YYDISABLE identifier ';'
			{ $$ = p_create_null_statement(); }
	;

seq_block:
		YYBEGIN block_item_opt statement_brace YYEND
			{ $$ = p_create_seq_block($3); }
	;

par_block:
		YYFORK block_item_opt statement_brace YYJOIN
			{ $$ = p_create_seq_block($3); }
	;

block_item_opt:
		/* Optional */
			{ $$ = 0; }
	|	YYCOLON identifier
			{ $$ = 0; /* FIXME: ignored for now. */ }
	|	YYCOLON identifier block_item_declaration
			{ $$ = 0; yyerror(" block item declaration "); }
	;

statement_brace:
		/* Optional. */
			{ $$ = p_create_statement_list(0, 0); }
	|	statement_brace statement
			{ $$ = p_create_statement_list($1, $2); }
	;

task_enable:
		task_identifier expression_brace_opt ';'
			{ $$ = p_create_task_enable($1, $2); }
	;

task_identifier:
		YYCHARSTR
			{ $$ = $1; }
	|	YYCHARSTRDOT
			{ $$ = $1; }
	;

system_task_enable:
		system_task_name expression_brace_opt ';'
			{ $$ = p_create_task_enable($1, $2); }
	;

system_task_name:
		YYSYSIDENT
			{ $$ = $1; }
	;


	/*
	 * Specify
	 */


specify_block:
		YYSPECIFY specify_item_opt YYENDSPECIFY
			{ $$ = 0; }
	;

specify_item_opt:
		/* Optional. */
			{ }
	|	specify_item
			{ yyerror(" specify item "); }
	;

specify_item:
		specparam_declaration
			{ }
	|	path_declaration
			{ }
	|	system_timing_check
			{ }
	;

specparam_declaration:
		YYSPECPARAM list_of_specparam_assignments ';'
			{ }
	;

list_of_specparam_assignments:
		specparam_assignment
			{ }
	|	list_of_specparam_assignments ',' specparam_assignment
			{ }
	;

specparam_assignment:
		identifier '=' const_expression
			{ }
	;

path_declaration:
		simple_path_declaration ';'
			{ }
	|	edge_sensitive_path_declaration ';'
			{ }
	|	state_dependent_path_declaration ';'
			{ }
	;

simple_path_declaration:
		parallel_path_description '=' path_delay_value
			{ }
	|	full_path_description '=' path_delay_value
			{ }
	;

parallel_path_description:
		/*
		 * FIXME: shift/reduce problem:
		 * When yacc finds a YYPLUS/YYMINUS token, it does not know whether
		 * to shift to a specify_input_terminal_descriptor or reduce to a
		 * list_of_path_inputs.  A lookahead of one more token would have
		 * resolved this conflict.  But that is not available.  So we use
		 * the list rule instead, since that will provide the same token.
		 * We just need to make sure there is only one terminal.
		 */
		'(' list_of_path_inputs polarity_operator_opt
			YYEQUALGREATER list_of_path_outputs ')'
			{ }
	;

full_path_description:
		'(' list_of_path_inputs polarity_operator_opt
			YYASTERICGREATER list_of_path_outputs ')'
			{ }
	;

list_of_path_inputs:
		specify_input_terminal_descriptor
			{ }
	|	list_of_path_inputs ',' specify_input_terminal_descriptor
			{ }
	;

list_of_path_outputs:
		specify_output_terminal_descriptor
			{ }
	|	list_of_path_outputs ',' specify_output_terminal_descriptor
			{ }
	;

specify_input_terminal_descriptor:
		identifier
			{ }
	|	identifier bit_select
			{ }
	|	identifier part_select
			{ }
	;

specify_output_terminal_descriptor:
		identifier
			{ }
	|	identifier bit_select
			{ }
	|	identifier part_select
			{ }
	;

polarity_operator_opt:
		/* Optional. */
			{ }
	|	YYPLUS
			{ }
	|	YYMINUS
			{ }
	;

path_delay_value:
 /* FIXME: reduce/reduce problem.
  * This is just an expression.  The ')' conflicted with the expression
  * rule.  Just use the standard expression rule instead.
  *		list_of_path_delay_expressions
  *			{ }
  *	|	'(' list_of_path_delay_expressions ')'
  *			{ }
  */
		expression
			{ }
	;

edge_sensitive_path_declaration:
		parallel_edge_sensitive_path_description '=' path_delay_value
			{ }
	|	full_edge_sensitive_path_description '=' path_delay_value
			{ }
	;

parallel_edge_sensitive_path_description:
		'(' edge_identifier_opt specify_input_terminal_descriptor
			YYEQUALGREATER
				specify_output_terminal_descriptor polarity_operator_opt
			YYCOLON
				data_source_expression ')'
			{ }
	;

full_edge_sensitive_path_description:
		'(' edge_identifier_opt list_of_path_inputs
			YYASTERICGREATER
				list_of_path_outputs polarity_operator_opt
			YYCOLON
				data_source_expression ')'
			{ }
	;

data_source_expression:
		expression
			{ }
	;

edge_identifier_opt:
		/*
		 * FIXME: shift/reduce problem.
		 * Not enough lookahead tokens to differentiate between edge
		 * sensitive and non-edge sensitive 'parallel path description'.
		 * Make the 'edge identifier' non-optional.
		 */
		/* Optional. */
	/*		{ }
	|*/	YYPOSEDGE
			{ }
	|	YYNEGEDGE
			{ }
	;

state_dependent_path_declaration:
		YYIF '(' expression ')' simple_path_declaration
			{ }
	|	YYIF '(' expression ')' edge_sensitive_path_declaration
			{ }
	|	YYIFNONE '(' expression ')' simple_path_declaration
			{ }
	;

system_timing_check:
		system_task_name
			{ }
	;


	/*
	 * Expressions.
	 */


reg_lvalue:
		reg_identifier
			{
			p_range_identifier rid = p_create_range_identifier($1, 0);
			p_expression_list elst = p_create_expression_list(0, rid);
			$$ = p_create_lvalue(elst);
			}
	|	reg_identifier bit_select
			{
			p_range_identifier rid = p_create_range_identifier($1, $2);
			p_expression_list elst = p_create_expression_list(0, rid);
			$$ = p_create_lvalue(elst);
			}
	|	reg_identifier part_select
			{
			p_range_identifier rid = p_create_range_identifier($1, $2);
			p_expression_list elst = p_create_expression_list(0, rid);
			$$ = p_create_lvalue(elst);
			}
	|	reg_concatenation
			{
			$$ = p_create_lvalue($1);
			}
	;

reg_identifier:
		identifier
			{ $$ = $1; }
	;

reg_concatenation:
		'{' expression_brace '}'
			{ $$ = $2; }
	;

const_expression:
		expression
			{ $$ = $1; }
	;

mintypmax_expression:
		expression
			{ $$ = $1; }
	|	expression YYCOLON expression YYCOLON expression
			{ $$ = p_create_mintypmax_expr($1, $3, $5); }
	;

expression_opt:
		/* Optional. */
			{ $$ = 0; }
	|	expression
			{ $$ = $1; }
	;

expression_brace:
		expression
			{ $$ = p_create_expression_list(0, $1); }
	|	expression_brace ',' expression
			{ $$ = p_create_expression_list($1, $3); }
	;

expression:
		primary
			{ $$ = $1; }
	|	YYPLUS primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYPLUS, $2); }
	|	YYMINUS primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYMINUS, $2); }
	|	YYEXCLAM primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYEXCLAM, $2); }
	|	YYTILDE primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYTILDE, $2); }
	|	YYAMPER primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYAMPER, $2); }
	|	YYTILDEAMPER primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYTILDEAMPER, $2); }
	|	YYVERTBAR primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYVERTBAR, $2); }
	|	YYTILDEVERTBAR primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYTILDEVERTBAR, $2); }
	|	YYCARET primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYCARET, $2); }
	|	YYTILDECARET primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYTILDECARET, $2); }
	|	YYCARETTILDE primary %prec YYUNARY_PREC
			{ $$ = p_create_unary_op_expr(YYCARETTILDE, $2); }
	|	expression YYPLUS expression
			{ $$ = p_create_binary_op_expr(YYPLUS, $1, $3); }
	|	expression YYMINUS expression
			{ $$ = p_create_binary_op_expr(YYMINUS, $1, $3); }
	|	expression YYASTERIC expression
			{ $$ = p_create_binary_op_expr(YYASTERIC, $1, $3); }
	|	expression YYSLASH expression
			{ $$ = p_create_binary_op_expr(YYSLASH, $1, $3); }
	|	expression YYPERCENT expression
			{ $$ = p_create_binary_op_expr(YYPERCENT, $1, $3); }
	|	expression YYEQUALEQUAL expression
			{ $$ = p_create_binary_op_expr(YYEQUALEQUAL, $1, $3); }
	|	expression YYEXCLAMEQUAL expression
			{ $$ = p_create_binary_op_expr(YYEXCLAMEQUAL, $1, $3); }
	|	expression YYEQUALEQUALEQUAL expression
			{ $$ = p_create_binary_op_expr(YYEQUALEQUALEQUAL, $1, $3); }
	|	expression YYEXCLAMEQUALEQUAL expression
			{ $$ = p_create_binary_op_expr(YYEXCLAMEQUALEQUAL, $1, $3); }
	|	expression YYAMPERAMPER expression
			{ $$ = p_create_binary_op_expr(YYAMPERAMPER, $1, $3); }
	|	expression YYVERTBARVERTBAR expression
			{ $$ = p_create_binary_op_expr(YYVERTBARVERTBAR, $1, $3); }
	|	expression YYLESS expression
			{ $$ = p_create_binary_op_expr(YYLESS, $1, $3); }
	|	expression YYLESSEQUAL expression
			{ $$ = p_create_binary_op_expr(YYLESSEQUAL, $1, $3); }
	|	expression YYGREATER expression
			{ $$ = p_create_binary_op_expr(YYGREATER, $1, $3); }
	|	expression YYGREATEREQUAL expression
			{ $$ = p_create_binary_op_expr(YYGREATEREQUAL, $1, $3); }
	|	expression YYAMPER expression
			{ $$ = p_create_binary_op_expr(YYAMPER, $1, $3); }
	|	expression YYTILDEAMPER expression
			{ $$ = p_create_binary_op_expr(YYTILDEAMPER, $1, $3); }
	|	expression YYVERTBAR expression
			{ $$ = p_create_binary_op_expr(YYVERTBAR, $1, $3); }
	|	expression YYTILDEVERTBAR expression
			{ $$ = p_create_binary_op_expr(YYTILDEVERTBAR, $1, $3); }
	|	expression YYCARET expression
			{ $$ = p_create_binary_op_expr(YYCARET, $1, $3); }
	|	expression YYTILDECARET expression
			{ $$ = p_create_binary_op_expr(YYTILDECARET, $1, $3); }
	|	expression YYCARETTILDE expression
			{ $$ = p_create_binary_op_expr(YYCARETTILDE, $1, $3); }
	|	expression YYLESSLESS expression
			{ $$ = p_create_binary_op_expr(YYLESSLESS, $1, $3); }
	|	expression YYGREATERGREATER expression
			{ $$ = p_create_binary_op_expr(YYGREATERGREATER, $1, $3); }
	|	expression YYQUESTION expression YYCOLON expression
			{ $$ = p_create_ternary_op_expr($1, $3, $5); }
	|	YYQSTRING
			{ $$ = p_create_qouted_string($1); }
	;

primary:
		number
			{ $$ = $1; }
	|	identifier
			{ $$ = p_create_range_identifier($1, 0); }
	|	identifier bit_select
			{ $$ = p_create_range_identifier($1, $2); }
	|	identifier part_select
			{ $$ = p_create_range_identifier($1, $2); }
	|	concatenation
			{ $$ = $1; }
	|	multiple_concatenation
			{ $$ = $1; }
	|	function_call
			{ $$ = $1; }
	|	'(' mintypmax_expression ')'
			{ $$ = $2; }
	;

concatenation:
		'{' expression_brace '}'
			{ $$ = p_create_concatenation(0, $2); }
	;

multiple_concatenation:
		'{' expression '{' expression_brace '}' '}'
			{ $$ = p_create_concatenation($2, $4); }
	;

function_call:
		function_identifier '(' expression_brace ')'
			{ $$ = p_create_function_call($1, $3); }
	|	name_of_system_function expression_brace_opt
			{ $$ = p_create_function_call($1, $2); }
	;

expression_brace_opt:
		/* Optional. */
			{ $$ = 0; }
	|	'(' expression_brace ')'
			{ $$ = $2; }
	;

function_identifier:
		YYCHARSTR
			{ $$ = $1; }
	|	YYCHARSTRDOT
			{ $$ = $1; }
	;

name_of_system_function:
		YYSYSIDENT
			{ $$ = $1; }
	;

number:
		decimal_number
			{ $$ = $1; }
	|	octal_number
			{ $$ = $1; }
	|	binary_number
			{ $$ = $1; }
	|	hex_number
			{ $$ = $1; }
	|	real_number
			{ $$ = $1; }
	;

decimal_number:
		unsigned_number
			{ $$ = $1; }
	|	YYSIZDEC
			{ $$ = p_create_number($1); }
	;

binary_number:
		YYSIZBIN
			{ $$ = p_create_number($1); }
	;

octal_number:
		YYSIZOCT
			{ $$ = p_create_number($1); }
	;

hex_number:
		YYSIZHEX
			{ $$ = p_create_number($1); }
	;

real_number:
		YYFLOAT
			{ $$ = p_create_number($1); yyerror(" real number "); }
	;

unsigned_number:
		YYNUMBER
			{ $$ = p_create_number($1); }
	;

identifier:
		YYCHARSTR
			{ $$ = $1; }
	|	YYCHARSTRDOT
			{ $$ = $1; }
	;

%%

int
is_printable(const char *str)
	{
	int i, res, len = strlen(str);
	res = len != 0;
	for (i = 0; i < len; ++i)
		{
		if (!isprint(str[i]))
			{
			res = 0;
			break;
			}
		}
	return res;
	}

void
yyerror(char *str)
	{
	if (str[0] == ' ') /* A hack to detect unsupported constructs. */
		sim_perror("unsupported", str, "discarded", cur_lineno);
	else if (savedword != 0)
		sim_perror("keyword", str, savedword, cur_lineno);
	else if (yylval.simple_string != 0)
		{
		if (is_printable(yylval.simple_string))
			sim_perror("symbol", str, yylval.simple_string, cur_lineno);
		else if (is_printable(yytext))
			sim_perror("symbol", str, yytext, cur_lineno);
		else if (feof(yyin))
			sim_perror("eof", str, "EOF", cur_lineno);
		else
			sim_perror("unsupported", str, "abstract rule", cur_lineno);
		}
	else
		sim_perror("unsupported", str, "unknown", cur_lineno);
	}
