// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001,2002,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// sim.cc

#include <cstdio>
#include <ctime>
#include <cstdlib> // getenv
#include <csetjmp>
#include <iomanip>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include "common/ptr.h"
#include "common/dstack.h"
#include "common/error.h"
#include "common/event.h"
#include "common/time_whl.h"
#include "common/sym_tab.h"
#include "common/scp_tab.h"
#include "common/dumpinfo.h"
#include "common/st_mod.h"
#include "common/st_setup.h"
#include "expr/number.h"
#include "expr/eeval.h"
#include "misc/module.h"
#include "misc/port.h"
#include "misc/msetup.h"
#include "misc/dectrig.h"
#include "misc/mrdwr.h"
#include "moditm/modinst.h"
#include "stmt/stmtbase.h"
#include "stmt/finish.h"
#include "stmt/display.h"
#include "stmt/write.h"
#include "stmt/monitor.h"
#include "stmt/readmem.h"
#include "stmt/strobe.h"
#include "stmt/dumptsk.h"
#include "stmt/xtime.h"
#include "stmt/stime.h"
#include "stmt/random.h"
#include "stmt/loopstmt.h"
#include "stmt/taskenbl.h"
#include "stmt/seqblk.h"
#include "stmt/ssetup.h"
#include "stmt/strigger.h"
#include "p_misc.h"
#include "parser.h"
#include "vbs.h"
#include "sim.h"

#include "common/debug.h"

#ifndef SEPARATOR_CHAR
#define SEPARATOR_CHAR '/'		// Directory slash (in character form).
#endif // SEPARATOR_CHAR

#ifndef VERILOGLCL_DIR
#define VERILOGLCL_DIR "./"		// Local directory.
#endif // VERILOGLCL_DIR

#ifndef VERILOGTMP_DIR
#define VERILOGTMP_DIR "./"		// Temporary directory.
#endif // VERILOGTMP_DIR

#ifndef VERILOGSRC_EXT
#define VERILOGSRC_EXT "v"		// Source extension.
#endif // VERILOGSRC_EXT

#ifndef VERILOGPPF_EXT
#define VERILOGPPF_EXT "vp"		// Preprocessed file extension.
#endif // VERILOGPPF_EXT

#ifndef VERILOGOBJ_EXT
#define VERILOGOBJ_EXT "vo"		// Module object extension.
#endif // VERILOGOBJ_EXT

#ifndef VERILOGTMP_EXT
#define VERILOGTMP_EXT "vt"		// Temporary file extension.
#endif // VERILOGTMP_EXT

using std::string;
using std::list;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::setfill;
using std::setw;

vbs_error::value_type parse_error_code;

// Single exit point handling.
jmp_buf vbs_sim_finish;

// Operation selector.
void (*sim_parsed_module)(p_module m);

// Add toplevel instance to symbol table.
void setup_toplevel(st_module *, Stack<int> &);

void trigger_postproc(stmt_base *st)
	{
	time_wheel<stmt_base> &timewheel = vbs_engine::timewheel();
	if (st->_always == true)
		{
		if (st->_ec == 0)
			{
			if (st->_dec != 0)
				st->_dec->_delayed = false; // Reset...
			timewheel.add_event(timewheel.current_time(), st);
			}
		}
	else
		{
		seq_block_stmt *sqblk = st->get_seq_block();
		if (sqblk != 0 && sqblk->_stk.size() > 0)
			{
			// End of a nesting block, re-execute statement.  Must advance
			// current statement pointer so we move forward.
			sqblk = sqblk->_stk.top();
			loop_stmt *lp = sqblk->_curstmt->get()->get_loop();
			task_enable_stmt *tsk = sqblk->_curstmt->get()->get_task_enable();
			bool delayed = ((lp != 0 && lp->delayed() == true)
						 || (tsk != 0 && tsk->delayed() == true));
			if (delayed == false)
				++(sqblk->_curstmt);
			timewheel.add_event(timewheel.current_time(), st);
			}
		}
	}

// Function object to handle events in the event queue.
template<class T>
struct eqhandler : public event_queue_handler<T>
	{
	void operator()(T *st) const
		{
		DEBUG_STATE(DEBUG_EVENT);
		DEBUG_OUTPUT("DEBUG_EVENT:  Executing next event...\n");
		bool end_reached = st->trigger(trigger_stmt(st));
		if (end_reached == true)
			trigger_postproc(st);
		}
	};

// Function object to handle events in the time wheel.
template<class T>
struct twhandler : public time_wheel_handler<T>
	{
	typedef typename time_wheel_handler<T>::time_type time_type;
	typedef typename time_wheel_handler<T>::event_list_type list_type;

	void operator()(time_type t, list_type &tlst) const
		{
		event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
		dump_info &dumpinfo = vbs_engine::dumpinfo();
		DEBUG_STATE(DEBUG_TIME_WHEEL);
		DEBUG_OUTPUT("DEBUG_TIME_WHELL:  Starting event time ");
		DEBUG_OUTPUT(t);
		DEBUG_OUTPUT("...\n\n");
		// Trigger the operations in this time event.
		bool end_reached;
		while (!tlst.empty())
			{
			// Statements return 'true' for two reasons;  1.
			// It has finished exec'ing or 2.  It is finished
			// with one nesting level.  Thus, we can't just
			// delete this statement without knowing which
			// case it was.
			T *st = tlst.front();
			DEBUG_STATE(DEBUG_TIME_WHEEL);
			DEBUG_OUTPUT("DEBUG_TIME_WHEEL:  Executing next event...\n");
			end_reached = st->trigger(trigger_stmt(st));
			tlst.pop_front();
			if (end_reached == true)
				trigger_postproc(st);

			// Process all events on time wheel first.
			if (!tlst.empty())
				continue;

			DEBUG_STATE(DEBUG_TIME_WHEEL);
			DEBUG_OUTPUT("DEBUG_TIME_WHEEL:  Handling event queue...\n");
			eventqueue.trigger(eqhandler<T>());
			DEBUG_STATE(DEBUG_TIME_WHEEL);
			DEBUG_OUTPUT("\n");
			}

		// Output changed variables, if necessary.
		if (dumpinfo.dump_on())
			dumpinfo.dump_values();
		}
	};

#ifdef __cplusplus
extern "C" {
#endif

hash_value
find_top_level(void)
	{
	// Go through each module in the list and mark all modules which are
	// instantiated as non-top level.
	symbol_table &symboltable = vbs_engine::symboltable();
	std::list<hash_value> &modulelist = vbs_engine::modulelist();
	Stack<int> scope;
	scope.push(0);
	list<hash_value>::iterator idx(modulelist.begin());
	list<hash_value>::iterator stop(modulelist.end());
	st_module *mod;
	for (; idx != stop; ++idx)
		{
		mod = symboltable.get(*idx)->get_module();
		st_module::module_item_list *mi_lst = mod->get_module_item();
		st_module::module_item_list::iterator mi_idx(mi_lst->begin());
		st_module::module_item_list::iterator mi_stop(mi_lst->end());
		for (; mi_idx != mi_stop; ++mi_idx)
			{
			module_instantiation *mi = (*mi_idx)->get_module_instantiation();
			if (mi != 0)
				{
				hash_value hv(st_node_find(mi->name().c_str(), scope));
				if (hv._scope < 0)
					{
					vbs_err.set_data(vbs_error::SE_STDEF, mi->_lineno);
					vbs_err.out(mi->name());
					}
				st_module *m = symboltable.get(hv)->get_module();
				if (m == 0)
					{
					vbs_err.set_data(vbs_error::SE_TYPE, mi->_lineno);
					vbs_err.out(mi->name());
					}
				m->is_top_level(false);
				}
			}
		}

	// Go through the list of modules.  If more than one module is
	// considered top level return error, else return the index.
	hash_value tl;
	idx = modulelist.begin();
	for (; idx != stop; ++idx)
		{
		mod = symboltable.get(*idx)->get_module();
		if (mod->is_top_level() == false)
			continue;
		else
			{
			if (tl._scope < 0)
				tl = *idx;
			else
				return hash_value();
			}
		}
	return tl;
	}

void
tmpfile_get(const string &bn, string &new_file)
	{
	new_file = VERILOGTMP_DIR + bn + "." + VERILOGTMP_EXT;
	}

void
tmpfile_remove(const string &bn)
	{
	string tmp_file = VERILOGTMP_DIR + bn + "." + VERILOGTMP_EXT;
	if (remove(tmp_file.c_str()) != 0)
		{
		vbs_err.set_data(vbs_error::SE_FILEDELETE, -1);
		vbs_err.out(tmp_file.c_str());
		}
	}

void
tmpfile_save(const string &bn, const char *new_ext)
	{
	string orig_file = VERILOGTMP_DIR + bn + "." + VERILOGTMP_EXT;
	string new_file = VERILOGLCL_DIR + bn + "." + new_ext;
	if (rename(orig_file.c_str(), new_file.c_str()) != 0)
		{
		tmpfile_remove(bn);
		vbs_err.set_data(vbs_error::SE_FILECREATE, -1);
		vbs_err.out(new_file.c_str());
		}
	}

void
extension_type(const char *fn, string &bn, string &ext)
	{
	// Find the basename and the extension of the filename and put
	// them in bn and ext, respectively.
	bn = fn;
	string::size_type sep = bn.find_last_of(SEPARATOR_CHAR);
	if (sep != (string::size_type) -1 && sep > 0)
		{
		++sep;
		bn.erase(0, sep);
		}
	// Remove extension from bn.
	string::size_type period = bn.find_last_of('.');
	if (period != (string::size_type) -1)
		{
		const char *ptr = &bn.c_str()[period];
		ext = ++ptr;
		bn.erase(period, bn.size() - period);
		}
	}

void
write_module_to_file(p_module m)
	{
	if (parse_error_code != 0)
		return; // Do not process, might contain invalid data.

	module *mod = (module *)m;
	// Create module object filename.
	string fn(mod->_filename);
	string base_fn, ext;
	extension_type(fn.c_str(), base_fn, ext);
	fn = base_fn + "." + VERILOGOBJ_EXT;
	// For debugging purposes, use non-binary mode.
	ofstream out(fn.c_str()/*, ios::bin*/);
	mod->write(write_module(out)); // Start writing data.
	delete mod; // Free module before quitting.
	}

void
store_module_to_symbol_table(p_module m)
	{
	if (parse_error_code != 0)
		return; // Do not process, might contain invalid data.

	// After we parsed a complete module, we need to store it into
	// the symbol table for later use.
	module *mod = (module *)m;

	DEBUG_STATE(DEBUG_PARSER);
	DEBUG_OUTPUT("DEBUG_PARSER:  Finished parsing a module:\n");
	DEBUG_OUTPUT(*mod);

	// Now enter the module into the symbol table.  The scope should
	// always be global.  There are no nested module definitions.
	st_module *stmod = new st_module(mod->name());
	stmod->scope(0);

	// Save port list and module items if any.
	mod->setup(setup_module(stmod));

	// Enter node into symbol table.
	symbol_table &symboltable = vbs_engine::symboltable();
	hash_value hv(symboltable.add(stmod));
	if (hv._scope < 0)
		{
		vbs_err.set_data((vbs_error::value_type) hv._value, mod->_lineno);
		vbs_err.out(mod->name());
		}
	std::list<hash_value> &modulelist = vbs_engine::modulelist();
	modulelist.push_back(hv);
	delete mod;
	}

#if defined(VERILOGVPP_PROG)

bool
preprocessor_find(string &pn)
	{
	// Find the preprocessor program.
	static string standard_locations[] =
		{
		string("/bin/"),
		string("/usr/bin/"),
		string("/usr/local/bin/"),
		string("./")
		};

	// First look in the directory where this binary was stored.
	pn = vbs_err.get_program_name();
	string::size_type sep = pn.find_last_of(SEPARATOR_CHAR);
	if (sep != (string::size_type) -1 && sep > 0)
		{
		pn.erase(sep, pn.size());
		pn.append(VERILOGVPP_PROG);
		// Check for existence.
		if (sim_program_exist(pn.c_str()))
			return true;
		}

	// See if there is an environment variable.
	char *env = getenv("VPP");
	if (env != 0)
		{
		pn = env;
		return true;
		}

	// Check standard locations.
	for (int i = 0; i < 4; ++i)
		{
		pn = standard_locations[i];
		pn.append(VERILOGVPP_PROG);
		if (sim_program_exist(pn.c_str()))
			return true;
		}

	return false;
	}

bool
preprocessor_external(FILE *out, char *vf, const string &pn)
	{
	return sim_program_exec(pn.c_str(), vf, out);
	}

#endif // VERILOGVPP_PROG

bool
preprocessor_simple(FILE *out, char *vf)
	{
	// Null preprocessor function.  Do simple file copy.
	bool success = true;
	FILE *in = fopen(vf, "r");
	int c = fgetc(in);
	while (!feof(in))
		{
		if (fputc(c, out) == EOF)
			{
			success = false;
			break;
			}
		c = fgetc(in);
		}
	fclose(in);
	return success;
	}

FILE *
sim_preprocess(char *vf, const string &bn)
	{
	bool status = false;
	string tmp_fn, pn;

	tmpfile_get(bn, tmp_fn);
	FILE *tmp_fp = fopen(tmp_fn.c_str(), "w");
	if (tmp_fp == 0)
		return 0;

#if defined(VERILOGVPP_PROG)
	if (preprocessor_find(pn))
		status = preprocessor_external(tmp_fp, vf, pn);
	else
#endif // VERILOGVPP_PROG
		status = preprocessor_simple(tmp_fp, vf);

	fclose(tmp_fp);
	if (status == false)
		{
		tmpfile_remove(bn); // Remove tmp file before quitting.
		return 0;
		}
	return fopen(tmp_fn.c_str(), "r"); // Reopen file for reading.
	}

void
sim_compile(FILE *fp, const string &fn, bool compile_only)
	{
	// Set post compile function pointer to take care of module
	// when it is compiled.
	if (compile_only)
		sim_parsed_module = write_module_to_file;
	else
		sim_parsed_module = store_module_to_symbol_table;

	// Generate the module object.
	p_start(fp, (fn + "." + VERILOGSRC_EXT).c_str(), 1);
	}

bool
test_input_file(char *vf, string &basename, vbs_error::state_type &type)
	{
	// Set filename for error reporting.
	FILE *fp = fopen(vf, "r");
	if (fp == 0)
		{
		vbs_err.set_data(vbs_error::SE_FILEOPEN, -1);
		vbs_err.out("read mode");
		return false;
		}
	fclose(fp);

	string ext;
	extension_type(vf, basename, ext);
	if (ext == VERILOGSRC_EXT)
		type = vbs_error::SS_COMPILE;
	else if (ext == VERILOGOBJ_EXT)
		type = vbs_error::SS_READOBJ_ONLY;
	else
		{
		vbs_err.set_data(vbs_error::SE_SUPPORT, -1);
		vbs_err.out("unknown extension");
		}

	sim_run_status(1, vf);
	return true;
	}

int
read_verilog_file(char *vf)
	{
	// Get base filename, and type of file.
	string basename;
	vbs_error::state_type mode = vbs_error::SS_NONE;

	if (!test_input_file(vf, basename, mode))
		return -1;
	if (mode == vbs_error::SS_READOBJ_ONLY)
		{
		// A module object file.  Read it and continue.
		ifstream ifs(vf/*, ios::bin*/);
		module *mod = module::read(read_module(ifs));
		store_module_to_symbol_table(mod);
		}
	else
		{
		// Pre-process the file.  It might not need any pre-processing
		// at all, but we do it anyway.  The returned file pointer is
		// the temporary file.  sim_preprocess should handle the fp's
		// correctly, i.e. close one before returning the other.
		vbs_err.clear_state(vbs_error::SS_STATE_MASK);
		vbs_err.set_state(vbs_error::SS_PREPROCESS);
		FILE *fp = sim_preprocess(vf, basename);
		if (fp == 0)
			{
			vbs_err.set_data(vbs_error::SE_PROGRAM, -1);
			vbs_err.out("preprocessor");
			return -1;
			}

		// What to do next?
		if (vbs_err.test_state(vbs_error::SS_PREPROCESS_ONLY))
			{
			// Pre-process only.  Save file and goto next file.
			fclose(fp);
			tmpfile_save(basename, VERILOGPPF_EXT);
			return 0;
			}

		// Compile the file, fp should be a temporary file.
		vbs_err.clear_state(vbs_error::SS_STATE_MASK);
		vbs_err.set_state(vbs_error::SS_COMPILE);
		sim_compile(fp, basename,
					vbs_err.test_state(vbs_error::SS_COMPILE_ONLY));

		// Delete temporary file, since we are done with it
		fclose(fp);
		tmpfile_remove(basename);
		}

	return 0;
	}

void
vbs_sim_init(const char *pn)
	{
	// Setup variables for error reporting.
	vbs_err.set_program_name(pn);

	// Symbol table for simulation
	symbol_table &symboltable = vbs_engine::symboltable();
	symboltable.initialize(HASHSIZE);

	// Our global system tasks and functions.
	// System tasks and functions are always global, so we do not really
	// need to pass in any scope values.  But an argument is still needed.
	Stack<int> scope;
	systask_finish *stf = new systask_finish;
	stf->setup(setup_stmt(scope, stf));
	systask_display *stdpy = new systask_display;
	stdpy->setup(setup_stmt(scope, stdpy));
	systask_write *stw = new systask_write;
	stw->setup(setup_stmt(scope, stw));
	systask_monitor *stm = new systask_monitor;
	stm->setup(setup_stmt(scope, stm));
	systask_readmemb *strmb = new systask_readmemb;
	strmb->setup(setup_stmt(scope, strmb));
	systask_readmemh *strmh = new systask_readmemh;
	strmh->setup(setup_stmt(scope, strmh));
	systask_strobe *sts = new systask_strobe;
	sts->setup(setup_stmt(scope, sts));
	systask_dumpon *std_on = new systask_dumpon;
	std_on->setup(setup_stmt(scope, std_on));
	systask_dumpoff *std_off = new systask_dumpoff;
	std_off->setup(setup_stmt(scope, std_off));
	systask_dumpall *std_all = new systask_dumpall;
	std_all->setup(setup_stmt(scope, std_all));
	systask_dumpvars *std_vars = new systask_dumpvars;
	std_vars->setup(setup_stmt(scope, std_vars));
	systask_dumpfile *std_file = new systask_dumpfile;
	std_file->setup(setup_stmt(scope, std_file));
	sysfunc_time *sft = new sysfunc_time;
	sft->setup(setup_stmt(scope, sft));
	sysfunc_stime *sfst = new sysfunc_stime;
	sfst->setup(setup_stmt(scope, sfst));
	sysfunc_random *sfr = new sysfunc_random;
	sfr->setup(setup_stmt(scope, sfr));
	}

void
vbs_sim_setup(void)
	{
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	scope_table &scopetable = vbs_engine::scopetable();
	symbol_table &symboltable = vbs_engine::symboltable();

	// Start with global scope for setup.
	Stack<int> scope;
	scope.push(0);

	// Eliminate non-top-level modules and use whatever is left as top-level.
	st_node_base *toplevel = 0;
	hash_value hv(find_top_level());
	if (hv._scope < 0)
		{
		// Could not find a top level module, let's try use 'main'.
		hv = st_node_find("main", scope);
		if (hv._scope < 0)
			{
			vbs_err.set_data(vbs_error::SE_NTOPLVL, -1);
			vbs_err.out("main");
			}
		toplevel = symboltable.get(hv);
		if (toplevel->get_module() == 0)
			{
			vbs_err.set_data(vbs_error::SE_NTOPLVL, -1);
			vbs_err.out(toplevel->name());
			}
		}
	else
		toplevel = symboltable.get(hv);
	sim_run_status(2, toplevel->name().c_str());
	cout << endl;

	setup_toplevel(toplevel->get_module(), scope);
	scopetable.top_level(toplevel->name());
	dumpinfo.top_level(toplevel->name());
	setup_stmt::second_pass();
	}

bool
vbs_sim_step()
	{
	time_wheel<stmt_base> &timewheel = vbs_engine::timewheel();
	return timewheel.trigger_once(twhandler<stmt_base>());
	}

long int
vbs_sim_run(int end_time)
	{
	time_wheel<stmt_base> &timewheel = vbs_engine::timewheel();
	bool step = true;
	switch (end_time)
		{
		case -1: // Fast...
			sim_cpu_time(1);
			timewheel.trigger(twhandler<stmt_base>());
			sim_cpu_time(2);
			step = false; // Nothing left to do.
			break;

		case 0: // Single step...
			step = timewheel.trigger_once(twhandler<stmt_base>());
			break;

		default: // Run until end_time...
			while ((int)timewheel.current_time() < end_time)
				{
				step = timewheel.trigger_once(twhandler<stmt_base>());
				if (!step)
					break;
				}
			break;
		}

	// If simulation ended, output status.
	if (!step)
		{
		sim_run_status(-1, "event starvation");
		sim_cpu_time(0);
		cout << endl;
		}

	return timewheel.current_time();
	}

void
vbs_sim_start(int amt, char **lst)
	{
	// First, parse all files.  Do not stop on error...
	for (int i = 0; i < amt; ++i)
		{
		vbs_err.push_filename(string(lst[i]));
		read_verilog_file(lst[i]);
		vbs_err.pop_filename();
		}
	if (parse_error_code != 0)
		{
		cout << endl << "Error detected during compilation...exiting" << endl;
		exit(parse_error_code);
		}

	// Start simulation if user did not specify anything to prevent it.
	if (vbs_err.test_state(vbs_error::SS_PREPROCESS_ONLY)
	 || vbs_err.test_state(vbs_error::SS_COMPILE_ONLY))
		return;

	// Our own exception handler...
	if (setjmp(vbs_sim_finish) != 0)
		return; // $finish enabled or an error occurred.

	// Setup for simulation.
	vbs_err.clear_state(vbs_error::SS_STATE_MASK);
	vbs_err.set_state(vbs_error::SS_SETUP);
	vbs_err.push_filename(string("setup"));
	DEBUG_STATE(DEBUG_SIM_STATE);
	DEBUG_OUTPUT("DEBUG_SIM_STATE:  Setting up for simulation...\n");
	vbs_sim_setup();
	vbs_err.pop_filename();

	symbol_table &symboltable = vbs_engine::symboltable();
	DEBUG_STATE(DEBUG_SYMBOL_TABLE);
	DEBUG_OUTPUT("DEBUG_SYMBOL_TABLE:  Symbol table after setup:\n");
	DEBUG_OUTPUT(symboltable);
	DEBUG_OUTPUT("\n\n");

	// Start simulation.
	vbs_err.clear_state(vbs_error::SS_STATE_MASK);
	vbs_err.set_state(vbs_error::SS_SIMULATE);
	vbs_err.push_filename(string("simulation"));
	DEBUG_STATE(DEBUG_SIM_STATE);
	DEBUG_OUTPUT("DEBUG_SIM_STATE:  Starting simulation...\n");
	vbs_sim_run(-1);
	vbs_err.pop_filename();
	}

const char *
sim_errmsg(const char *prog, const char *m, const char *fn, int ln, int c, const char *cs)
	{
	stringstream s;

	s << endl;
	s << prog << ": ";
	s << fn << '[';
	if (ln > 0)
		s << ln;
	else
		s << '?';
	s << "] ";

	if (c != 0)
		s << "-" << c;
	else
		s << "-?";
	s << "- " << cs;

	// Display the message.
	s << " (" << m << ")." << endl << endl;

	return s.str().c_str();
	}

void
sim_perror(char *type, char *mesg, char *word, int ln)
	{
	vbs_error::value_type val;
	if (strcmp(type, "symbol") == 0)
		val = vbs_error::SE_PSYMBOL;
	else if (strcmp(type, "keyword") == 0)
		val = vbs_error::SE_PKEYWORD;
	else if (strcmp(type, "eof") == 0)
		val = vbs_error::SE_PEOF;
	else
		val = vbs_error::SE_SUPPORT;
	string buf(mesg);
	buf.append(": ");
	buf.append(word);

	vbs_err.set_data(val, ln);
	vbs_err.out(buf);

	// Save error state to prevent simulation/setup.
	if (parse_error_code == 0)
		parse_error_code = val;
	}

void
sim_set_state(int flag)
	{
	// This is a hack to set the state from the main driver so we
	// don't have to include error.h in vbs.c.
	vbs_err.clear_state(vbs_error::SS_MODE_MASK); // Set mode.
	switch (flag)
		{
		case 0x01:
			vbs_err.set_state(vbs_error::SS_COMPILE_ONLY);
			break;
		case 0x02:
			vbs_err.set_state(vbs_error::SS_PREPROCESS_ONLY);
			break;
		default:
			break;
		}
	}

void
sim_set_dump(int flag)
	{
	dump_info &dumpinfo = vbs_engine::dumpinfo();

	// This is a hack to set the dump config from the main driver so we
	// don't have to include dumpinfo.h in main.cc.
	switch (flag)
		{
		case -1:
			dumpinfo.dump_format("ascii");
			break;
		case -2:
			dumpinfo.dump_format("vcd");
			break;
		case 1: case 2: case 3: case 4:
		case 5: case 6: case 7: case 8: case 9:
			dumpinfo.dump_compress(flag);
			break;
		default:
			break;
		}
	}

void
sim_run_status(int reason, const char *message)
	{
	static bool quiet = false;

	switch (reason)
		{
		case 0:
			quiet = true;
			break;
		case 1:
			if (!quiet)
				cout << "Compiling file '" << message << "'." << endl;
			break;
		case 2:
			if (!quiet)
				cout << "Top-level module:  " << message << endl;
			break;
		case 3:
			if (!quiet)
				cout << "Generating dump file:  " << message << endl;
			break;
		default:
			if (!quiet)
				{
				time_wheel<stmt_base> &timewheel = vbs_engine::timewheel();
				cout << endl
					 << "Simulation stopped at time unit "
					 << timewheel.current_time()
					 << " due to ";
				cout << message << '.'
					 << endl;
				}
			break;
		}
	}

unsigned long
sim_current_time()
	{
	time_wheel<stmt_base> &timewheel = vbs_engine::timewheel();
	return timewheel.current_time();
	}

void
sim_cpu_time(int reason)
	{
	static bool output_clock = false;
	static time_t start_clock = 0;
	static time_t sim_clock = 0;

	switch (reason)
		{
		case 1:
			start_clock = clock();
			break;
		case 2:
			sim_clock = clock() - start_clock;
			break;
		case 3:
			output_clock = true;
			break;
		case 0:
			if (output_clock)
				{
				event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
				// Convert to milliseconds.
				time_t sim_time_ms = (sim_clock * 1000) / CLOCKS_PER_SEC;
				// Calculate seconds.
				time_t sim_time_s = sim_time_ms / 1000;
				// Calculate milliseconds.
				sim_time_ms -= sim_time_s * 1000;
				cout << "CPU time: "
					<< sim_time_s
					<< '.'
					<< setfill('0') << setw(3) << sim_time_ms
					<< " sec.  ";
				cout << "Events queued: "
					<< eventqueue.queued_events()
					<< "  ";
				cout << "Events executed: "
					<< eventqueue.triggered_events()
					<< endl;
				}
			break;
		}
	}

#ifdef __cplusplus
}
#endif // __cplusplus
