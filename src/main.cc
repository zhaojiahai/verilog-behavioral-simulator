// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// main.cc
//
// Main driver.  This file is kept as simple as possible.  All work should
// be done by other modules.

#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif
#include <cstring>
#include <cstdlib> // exit()
#if defined(HAVE_GETOPT_H)
#include <getopt.h>
#elif defined(HAVE_PROTO_GETOPT)
#include <unistd.h> /* getopt() is prototyped in this header. */
#else
extern "C" int opterr;
extern "C" int optind;
extern "C" int optopt;
extern "C" char *optarg;
extern "C" int getopt(int argc, char * const argv[], const char *optstr);
#endif

#include <csetjmp> // longjmp()
#include <string>
#include <sstream>
#include <iostream>
#include "expr/mintypmax.h"
#include "vbs.h"
#include "sim.h" // Starting point...
#include "version.h"

#include "common/debug.h"

using namespace std;

char help_message1[] =
"\t-a or --ascii_dump\n"
"\t\tDump using ascii format.\n"
"\t-c or --compile-only\n"
"\t\tStop after compilation, do not simulate.\n"
"\t-d keyword or --debug-enable=keyword\n"
"\t\tSpecifies the type of debug output.  Multiple use\n"
"\t\tis allowed.  The following keywords are available:\n"
"\t\t\tsim_state, parser, symbol_table, time_wheel, event\n"
"\t\t\tuser[1-4], userx\n"
"\t\tAvailable only if debug is enabled.\n";

char help_message2[] =
"\t-D or --define\n"
"\t\tDefine macro for preprocessing (only for vpp).\n"
"\t-E or --preprocess-only\n"
"\t\tStop after preprocessing, do not compile.\n"
"\t-I or --incdir\n"
"\t\tSearch directory for `include files (only for vpp).\n"
"\t-l filename or --debug-outfile=filename\n"
"\t\tSpecifies the file to write debug output.  If not\n"
"\t\tspecified and debug is enabled, output will go to\n"
"\t\tstdout.  Available only if debug is enabled.\n"
"\t-q or --quiet\n"
"\t\tDo not output statistics.\n";

char help_message3[] =
"\t-v or --vcd_dump\n"
"\t\tDump using VCD format.\n"
"\t-x min|typ|max or --delays=min|typ|max\n"
#if defined(HAVE_ZLIB_H)
"\t-z #\n"
"\t\tCompress dumpfile using zlib (compression level 1 to 9).\n"
#endif
"\t-h or --help\n"
"\t\tDisplay this message.\n";

void
info()
	{
	cout << "Welcome to Verilog Behavioral Simulator version ";
	cout << VERSION << '.' << endl << endl;
	cout << "Syntax:  progname [ filename ... ]" << endl;
	cout << help_message1 << help_message2 << help_message3 << endl;
	}

#if !defined(HAVE_GETOPT) && !defined(HAVE_GETOPT_LONG)
int opterr = 1;
int optind = 1;
int optopt;
char *optarg;

int
getopt(int argc, char *argv[], const char *optstr)
	{
	static int sp = 1;
	register int c;
	register char *cp;

	if (sp == 1)
		{
		if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
			return EOF;
		else if (strcmp(argv[optind], "--") == 0)
			{
			++optind;
			return EOF;
			}
		}
	optopt = c = argv[optind][sp];
	if (c == ':' || (cp=strchr(optstr, c)) == 0)
		{
		if (argv[optind][++sp] == '\0')
			{
			++optind;
			sp = 1;
			}
		return '?';
		}
	if (*(++cp) == ':')
		{
		if (argv[optind][sp+1] != '\0')
			optarg = &argv[optind++][sp+1];
		else if (++optind >= argc)
			{
			sp = 1;
			return '?';
			}
		else
			optarg = argv[optind++];
		sp = 1;
		}
	else
		{
		if (argv[optind][++sp] == '\0')
			{
			sp = 1;
			++optind;
			}
		optarg = NULL;
		}

	return c;
	}
#endif // !HAVE_GETOPT && !HAVE_GETOPT_LONG

int
parse_args(int argc, char *argv[])
	{
	extern int parms_cnt;
	extern char *parms[];
	int quiet_flag = 0;
	bool ascii_dump_flag = false, vcd_dump_flag = false;
	int compressed_dump_level = 0;
	char *debug_fn = 0;
	const char optstr[] = "acd:D:EI:l:qvx:z:h";

#if defined(HAVE_GETOPT_LONG)

	int option_index = 0;
	struct option long_option[] =
		{
		{"ascii-dump", 0, 0, 'a'},
		{"compile-only", 0, 0, 'c'},
		{"debug-enable", 1, 0, 'd'},
		{"define", 1, 0, 'D'},
		{"preprocess-only", 0, 0, 'E'},
		{"incdir", 1, 0, 'I'},
		{"debug-outfile", 1, 0, 'l'},
		{"quiet", 0, 0, 'q'},
		{"vcd-dump", 0, 0, 'v'},
		{"delays", 1, 0, 'x'},
		{"dump-compress", 1, 0, 'z'},
		{"help", 0, 0, 'h'},
		{0, 0, 0, 0}
		};

	// Parse the command line.
	while (1)
		{
		int c = getopt_long(argc, argv, optstr, long_option, &option_index);

#else

	// Parse the command line.
	while (1)
		{
		int c = getopt(argc, argv, optstr);

#endif

		if (c == EOF)
			break;
		switch(c)
			{
			case 'a':
				ascii_dump_flag = true;
				break;
			case 'c':
				sim_set_state(0x01); // Compile only.
				break;
			case 'd':
				if (strcmp(optarg,"sim_state") == 0)
					DEBUG_ENABLE(DEBUG_SIM_STATE);
				else if (strcmp(optarg,"parser") == 0)
					DEBUG_ENABLE(DEBUG_PARSER);
				else if (strcmp(optarg,"symbol_table") == 0)
					DEBUG_ENABLE(DEBUG_SYMBOL_TABLE);
				else if (strcmp(optarg,"time_wheel") == 0)
					DEBUG_ENABLE(DEBUG_TIME_WHEEL);
				else if (strcmp(optarg,"event") == 0)
					DEBUG_ENABLE(DEBUG_EVENT);
				else if (strcmp(optarg,"userx") == 0)
					DEBUG_ENABLE(DEBUG_USERX);
				else if (strcmp(optarg,"user1") == 0)
					DEBUG_ENABLE(DEBUG_USER1);
				else if (strcmp(optarg,"user2") == 0)
					DEBUG_ENABLE(DEBUG_USER2);
				else if (strcmp(optarg,"user3") == 0)
					DEBUG_ENABLE(DEBUG_USER3);
				else if (strcmp(optarg,"user4") == 0)
					DEBUG_ENABLE(DEBUG_USER4);
				else
					{
					cout << argv[0] << ":  Don't understand debug option ";
					cout << optarg << '.' << endl;
					return -1;
					}
				break;
			case 'D':
				parms[parms_cnt] = new char[strlen(optarg)+3];
				strcpy(parms[parms_cnt], "-D");
				strcat(parms[parms_cnt], optarg);
				++parms_cnt;
				break;
			case 'E':
				sim_set_state(0x02); // Preprocess only.
				break;
			case 'I':
				parms[parms_cnt] = new char[strlen(optarg)+10];
				strcpy(parms[parms_cnt], "+incdir+");
				strcat(parms[parms_cnt], optarg);
				strcat(parms[parms_cnt], "+");
				++parms_cnt;
				break;
			case 'l':
				debug_fn = optarg;
				break;
			case 'q':
				++quiet_flag;
				break;
			case 'v':
				vcd_dump_flag = true;
				break;
			case 'x':
				if (strcmp(optarg, "min") == 0)
					mintypmax_expr::min_delay();
				else if (strcmp(optarg, "typ") == 0)
					mintypmax_expr::typ_delay();
				else if (strcmp(optarg, "max") == 0)
					mintypmax_expr::max_delay();
				else
					{
					cout << argv[0] << ":  Don't understand delay option ";
					cout << optarg << '.' << endl;
					return -1;
					}
				break;
			case 'z':
				compressed_dump_level = atoi(optarg);
				if (compressed_dump_level < 1 || compressed_dump_level > 9)
					{
					cout << argv[0] << ":  compression level ";
					cout << optarg << " is out of range." << endl;
					return -1;
					}
				break;
			case 'h':
				info();
				return 0;
#if defined(HAVE_GETOPT_LONG)
			case 0: // Should never happen.
				cout << argv[0] << ":  Don't know how to handle long ";
				cout << "option --" << long_option[option_index].name;
				cout << '.' << endl;
				return -1;
#endif
			default:
				cout << argv[0] << ":  Don't know how to handle ";
				cout << "option '" << (char)c << "'." << endl;
				return -1;
			}
		}

	// Initialize debugging system.
	DEBUG_INITIALIZE(debug_fn);

	// If we're not asked to be quiet, enable statistics.
	if (quiet_flag == 0)
		sim_cpu_time(3);
	else if (quiet_flag > 1)
		sim_run_status(0, 0);

	// Set dump configuration.
	if (compressed_dump_level > 0)
		{
		// Compress option must be set before format.
		sim_set_dump(compressed_dump_level);
		}
	if (ascii_dump_flag)
		sim_set_dump(-1);
	else if (vcd_dump_flag)
		sim_set_dump(-2);

	return 0;
	}

int
main(int argc, char *argv[])
	{
	int retval, infiles;

	if (argc < 2)
		{
		info();
		return 0;
		}

	// Parse command line arguments.
	retval = parse_args(argc, argv);
	if (retval != 0)
		return retval;

	// Initialize system variables (globals).
	vbs_engine::initialize(); // Create our engine singleton.
	vbs_sim_init(argv[0]);

	// Here we go!
	infiles = argc - optind;
	if (infiles > 0)
		vbs_sim_start(infiles, &argv[optind]);
	else
		cout << "No input file(s)..." << endl;

	// Cleanup engine.
	vbs_engine::reset();

	return 0;
	}

void
vbs_warn(int c, const string &cm, const char *st, const string &fn, int ln, const string &m, const string &p)
	{
	cout << sim_errmsg(p.c_str(), m.c_str(), fn.c_str(), ln, c, cm.c_str()) << endl;
	}

void
vbs_fatal(int c, const string &cm, const char *st, const string &fn, int ln, const string &m, const string &p)
	{
	cout << endl << sim_errmsg(p.c_str(), m.c_str(), fn.c_str(), ln, c, cm.c_str()) << endl << endl;
	cout << "Error detected during " << st << "...exiting" << endl;
	exit(c);
	}
