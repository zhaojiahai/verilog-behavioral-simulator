// Verilog Behavioral Simulator
// Copyright (C) 1995-1997,2001-2003,2011 Lay Hoon Tho, Jimen Ching
//
// This file is part of the Verilog Behavioral Simulator package.
// See the file COPYRIGHT for copyright and disclaimer information.
// See the file COPYING for the licensing terms and conditions.
// See the file CONTRIBUTORS for a list of contributing authors.
//
// strigger.cc

#include <cstdlib> // rand()
#include <csetjmp> // longjmp()
#include "common/error.h"
#include "common/time_whl.h"
#include "common/sym_tab.h"
#include "common/scp_tab.h"
#include "common/dumpinfo.h"
#include "common/st_func.h"
#include "common/st_task.h"
#include "common/st_net.h"
#include "common/st_trig.h"
#include "common/st_util.h"
#include "expr/qstr.h"
#include "expr/eeval.h"
#include "expr/emon.h"
#include "expr/etrigger.h"
#include "misc/dectrig.h"
#include "misc/mtrigger.h"
#include "stmt/finish.h"
#include "stmt/display.h"
#include "stmt/write.h"
#include "stmt/monitor.h"
#include "stmt/strobe.h"
#include "stmt/dumptsk.h"
#include "stmt/xtime.h"
#include "stmt/stime.h"
#include "stmt/random.h"
#include "stmt/seqblk.h"
#include "stmt/taskenbl.h"
#include "stmt/assign.h"
#include "stmt/ifelse.h"
#include "stmt/case.h"
#include "stmt/loopstmt.h"
#include "stmt/strigger.h"

#include "common/debug.h"
#include "common/tmpl_utl.h"
#include "vbs.h"
#include "sim.h"


// Case item for case statement.

bool
trigger_case_item::operator()(case_item *p) const
	{
	if (p->is_default())
		return p->_stmt->trigger(trigger_stmt(_parent));
	bool ret(true);
	case_item::expr_list::iterator itp(p->_expr->begin());
	case_item::expr_list::iterator stop(p->_expr->end());
	for (; itp != stop; ++itp)
		{
		bool test = false;
		switch (_type)
			{
			case case_item::CASE_X:
				test = compare_x(_value, (*itp)->evaluate(evaluate_expr()));
				break;
			case case_item::CASE_Z:
				test = compare_z(_value, (*itp)->evaluate(evaluate_expr()));
				break;
			default:
				test = compare_c(_value, (*itp)->evaluate(evaluate_expr()));
				break;
			}
		if (test)
			{
			ret = p->_stmt->trigger(trigger_stmt(_parent));
			_status = true;
			break;
			}
		}
	return ret;
	}


// All statements.

// The following few function definitions are system tasks and functions.
// They are not regular statements, so seperate them.

bool
trigger_stmt::operator()(systask_finish *) const
	{
	// Stop simulation clock.
	sim_cpu_time(2);

	// systask_finish is an internal statement.  It can not have a
	// delay or event control.  So, just trigger it.
	sim_run_status(-1, "$finish");

	// Printout simulation time.
	sim_cpu_time(0);
	cout << endl;

	extern jmp_buf vbs_sim_finish;
	longjmp(vbs_sim_finish, 1);

	// Unreachable code...
	return false;
	}

void
trigger_stmt::sys_task_output(task_caller_type *caller) const
	{
	// Now, grab the format string and find out what to do.
	scope_table &scopetable = vbs_engine::scopetable();
	task_caller_type::arg_list::iterator itp(caller->_argument->begin());
	task_caller_type::arg_list::iterator stop(caller->_argument->end());
	// Should not be null.  Setup should have checked.
	qouted_str *qs = (*itp)->get_qouted_str();
	const str_type &fmt(qs->get_str());
	++itp;
	for (size_t i = 1; i < fmt.size(); ++i)
		{
		if (fmt[i] == '\"')
			break;
		switch(fmt[i])
			{
			case '%':
				{
				str_type s;
				++i;	// Move to next character.
				vbs_error::value_type trash;
				find_format(i, fmt, s, trash);
				if (s == "%")
					{
					// Escape character for percent sign.
					cout << '%';
					}
				else if (s == "m")
					{
					// Hierarchical name.
					cout << scopetable.find(caller->_scope);
					}
				else if (itp != stop)
					{
					(*itp)->trigger(trigger_expr(s));
					++itp;
					}
				break;
				}
			case '\\':
				// Escape character found.
				print_escapecode(fmt, ++i);
				break;
			default:
				// Other junk gets printed.
				cout << fmt[i];
				break;
			}
		}
	}

bool
trigger_stmt::operator()(systask_display *) const
	{
	task_caller_type *caller = _parent->get_task_enable();
	// The $display task enable should at least have a format string.
	// No string means nothing to do.
	if (caller->_argument != 0)
		{
		sys_task_output(caller);

		/*
		 * The difference between $display and $write is that $display
		 * includes a newline.
		 */
		cout << endl;
		}
	return true;
	}

bool
trigger_stmt::operator()(systask_write *) const
	{
	task_caller_type *caller = _parent->get_task_enable();
	// The $write task enable should at least have a format string.
	// No string means nothing to do.
	if (caller->_argument != 0)
		sys_task_output(caller);
	return true;
	}

bool
trigger_stmt::operator()(systask_monitor *) const
	{
	task_caller_type *caller = _parent->get_task_enable();
	// The $monitor task enable should at least have a format string.
	// No string means nothing to do.
	if (caller->_argument != 0)
		{
		sys_task_output(caller);
		cout << endl; // Similar to $display.
		}
	return true;
	}

void
trigger_stmt::sys_task_readmem(task_caller_type *caller, int base) const
	{
	// Now, grab the format string and find out what to do.
	task_caller_type::arg_list::iterator itp(caller->_argument->begin());
	task_caller_type::arg_list::iterator stop(caller->_argument->end());

	// Should not be null.  Setup should have checked.
	qouted_str *qs = (*itp)->get_qouted_str();
	const str_type &fmt(qs->get_str());
	const str_type &fn(fmt.substr(1, fmt.size() - 2));

	// Get memory identifier.
	++itp;
	range_id *rid = (*itp)->get_range_id();

	// Get start/finish indices if specified.
	num_type::position_type start = -1, finish = -1;
	if (++itp != stop)
		{
		start = (unsigned long) (*itp)->evaluate(evaluate_expr());
		if (++itp != stop)
			finish = (unsigned long) (*itp)->evaluate(evaluate_expr());
		}

	// Read file into memory.
	symbol_table &symboltable = vbs_engine::symboltable();
	net_type *net = symboltable.get(rid->index())->get_net();
	net->assignment(fn.c_str(), base, start, finish);
	}

bool
trigger_stmt::operator()(systask_readmemb *) const
	{
	task_caller_type *caller = _parent->get_task_enable();
	// The $readmemb task enable should at least have a filename.
	// No string means nothing to do.
	if (caller->_argument != 0)
		sys_task_readmem(caller, 2);
	return true;
	}

bool
trigger_stmt::operator()(systask_readmemh *) const
	{
	task_caller_type *caller = _parent->get_task_enable();
	// The $readmemh task enable should at least have a filename.
	// No string means nothing to do.
	if (caller->_argument != 0)
		sys_task_readmem(caller, 16);
	return true;
	}

bool
trigger_stmt::operator()(systask_strobe *) const
	{
	task_caller_type *caller = _parent->get_task_enable();
	// The $strobe task enable should at least have a format string.
	// No string means nothing to do.
	if (caller->_argument != 0)
		{
		sys_task_output(caller);
		cout << endl; // Similar to $display.
		}
	return true;
	}

bool
trigger_stmt::operator()(systask_dumpon *) const
	{
	// Turn on dumping.
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	dumpinfo.dump_on(true);
	return true;
	}

bool
trigger_stmt::operator()(systask_dumpoff *) const
	{
	// Turn off dumping.
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	dumpinfo.dump_on(false);
	return true;
	}

bool
trigger_stmt::operator()(systask_dumpall *) const
	{
	// Turn on dump of all variables.
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	dumpinfo.dump_all();
	return true;
	}

bool
trigger_stmt::operator()(systask_dumpvars *) const
	{
	dump_info &dumpinfo = vbs_engine::dumpinfo();
	task_caller_type *caller = _parent->get_task_enable();
	// The $dumpvars 
	// No arguments means dump all signals
	// otherwise  
	//       (n,top) => n level from top, 0 = all signals below
	if (caller->_argument != 0)
		{
		task_caller_type::arg_list::iterator first(caller->_argument->begin());
		task_caller_type::arg_list::iterator stop(caller->_argument->end());
		str_type h;
		number *n = ((*first)->get_number());
		if (n != NULL)
			{
			unsigned long a = (*first)->evaluate(evaluate_expr());
			dumpinfo.levels(a);
			}
		++first;
		if (first != stop)
			{
			range_id *n = (*first)->get_range_id();
			if (n != NULL)
				dumpinfo.dump_vars(n->name());
			}
		}
	else
		{
		dumpinfo.levels(0);
		dumpinfo.dump_vars_all(); // Set dump all.
		}
	dumpinfo.dump_on(true);
	return true;
	}

bool
trigger_stmt::operator()(systask_dumpfile *) const
	{
// FIXME:  if second pass setup outputs the header, then dumpinfo.dump_file
// must be called during first pass setup.  Maybe we should eliminate second
// pass setup...
#if 0
	// Set the dumpfile.
	task_caller_type *caller = _parent->get_task_enable();
	if (caller->_argument != 0)
		{
		// Now, grab the format string and find out what to do.
		task_caller_type::arg_list::iterator itp(caller->_argument->begin());
		// Should not be null.  Setup should have checked.
		qouted_str *qs = (*itp)->get_qouted_str(); 
		dumpinfo.dump_file(qs->get_str());
		}
#endif

	return true;
	}

bool
trigger_stmt::operator()(sysfunc_time *p) const
	{
	// All we need to do is store the current time.
	symbol_table &symboltable = vbs_engine::symboltable();
	func_type *func = symboltable.get(p->_index)->get_function();
	func->_storage = sim_current_time();
	return true;
	}

bool
trigger_stmt::operator()(sysfunc_stime *p) const
	{
	// All we need to do is store the current time.
	symbol_table &symboltable = vbs_engine::symboltable();
	func_type *func = symboltable.get(p->_index)->get_function();
	func->_storage = sim_current_time();
	return true;
	}

bool
trigger_stmt::operator()(sysfunc_random *p) const
	{
	// All we need to do is store a random number.
        // At least for 32 bits.
	symbol_table &symboltable = vbs_engine::symboltable();
	func_type *func = symboltable.get(p->_index)->get_function();
	func->_storage = rand();
	return true;
	}


// The rest of the function definitions are for statement trigger.


inline int
trigger_stmt::handle_dec(stmt_type *p) const
	{
	// Take care of delay or event control before statement.
	int retval = 0;
	stmt_type::dec_type *dec = p->_dec;

	// Event controls might be stored in _ec, for always construct.
	// In which case, it was already processed.
	if (dec == 0 && !_parent->_always)
		dec = p->_ec;

	// Process control, independent of where it was stored.
	if (dec != 0)
		{
		// We are asked to persist, so indicate a state change,
		// and continue.
		if (dec->_delayed == true)
			dec->_delayed = false;
		else
			{
			dec->_delayed = true;
			dec->trigger(trigger_dec(_parent));
			retval = 1;
			}
		}

	return retval;
	}

bool
trigger_stmt::operator()(null_stmt *p) const
	{
	// This function is used as a template for all the other statement
	// trigger functions.  Since all of them are the same, no need to
	// re-comment each one.

	// Do we need to delay?
	if (handle_dec(p) != 0)
		return false;

	// We should now do whatever is necessary.  For null statements,
	// there is nothing to do, so return.
	return true;
	}

bool
trigger_stmt::operator()(seq_block_stmt *p) const
	{
	// Do we need to delay?
	if (handle_dec(p) != 0)
		return false;

	// This is the new simulation procedure.  We must handle one statement
	// at a time.  This means we do not go through the entire list
	// and append each statement to the future.  Instead, we get one
	// statement and try and execute it.  If there's a delay, we append
	// this entire statement into the future (else case in above).

	// Note:  We must push the list of statements into the stack if it's
	// not already on the stack.
	seq_block_stmt *sqblk = _parent->get_seq_block();
	if (!p->_pushed)
		{
		DEBUG_STATE(DEBUG_USERX);
		DEBUG_OUTPUT("DEBUG_USERX:  Adding list...\n");
		DEBUG_FOUTPUT(ptrprintlist<seq_block_stmt::stmt_list>(p->_stmt));
		DEBUG_OUTPUT("\n");
		// Set current statement to be used later.  This works
		// because <_parent> is guaranteed to be a <seqblk>.
		p->_curstmt = p->_stmt->begin();
		sqblk->_stk.push(p);
		p->_pushed = true;
		}

	// Let's begin.
	seq_block_stmt *nesting;
	while (true)
		{
		// First, get next statement.
		while (true)
			{
			// Do we have anything to do?
			if (sqblk->_stk.size() < 1)
				return true;
			// Grab the block to execute.
			nesting = sqblk->_stk.top();
			// Check if we came to the end.
			if (nesting->_curstmt != nesting->_stmt->end())
				{
				// We got a statement.
				DEBUG_STATE(DEBUG_USERX);
				DEBUG_OUTPUT("DEBUG_USERX:  Using list...\n");
				DEBUG_FOUTPUT(ptrprintlist<seq_block_stmt::stmt_list>(nesting->_stmt));
				DEBUG_OUTPUT("\n");
				break;
				}
			else
				{
				// No more statements in this nested level.
				DEBUG_STATE(DEBUG_USERX);
				DEBUG_OUTPUT("DEBUG_USERX:  Ended one nesting level...\n");
				// Dump this block.
				sqblk->_stk.pop();
				// Reset <pushed>, because we might be triggered again.
				nesting->_pushed = false;
				// Done with this nesting level, let caller decide what to do.
				return true;
				}
			}

		// Now trigger this statement.  Let the statement handle the delay.
		stmt_type *tmp = (*nesting->_curstmt).get();
		DEBUG_STATE(DEBUG_USERX);
		DEBUG_OUTPUT("DEBUG_USERX:  Triggering...\n");
		DEBUG_OUTPUT(*tmp);
		DEBUG_OUTPUT("\n");
		if (tmp->trigger(trigger_stmt(_parent)) == false)
			{
			// Execution has been delayed.
			DEBUG_STATE(DEBUG_USERX);
			DEBUG_OUTPUT("DEBUG_USERX:  Delayed * level...\n");
			DEBUG_OUTPUT(*tmp);
			DEBUG_OUTPUT("\n");
			return false;
			}
		++(nesting->_curstmt); // Setup for next round.
		}

	// Unreachable code...
	return false;
	}

bool
trigger_stmt::operator()(task_enable_stmt *p) const
	{
	// Do we need to delay?
	if (handle_dec(p) != 0)
		return false;

	// Setup of $monitor/$strobe has been delayed until now.
	// We don't want to monitor the changes until the task has been
	// enabled.  Otherwise, extraneous output will be generated.
	// This should only occur once, since we remove the pointer
	// afterwards.
	if (p->_event != 0)
		{
		task_enable_stmt::arg_list::iterator itp(p->_argument->begin());
		task_enable_stmt::arg_list::iterator stop(p->_argument->end());
		++itp; // Skip the format string.
		for (; itp != stop; ++itp)
			{
			if (!(*itp)->monitor(monitor_expr(p->_event)))
				{
				strstream_type buf;
				buf << *(*itp);
				vbs_err.set_data(vbs_error::SE_TYPE, (*itp)->_lineno);
				vbs_err.out(buf);
				}
			}
		delete p->_event;
		p->_event = 0;

		// Enabling $monitor/$strobe doesn't generate output.
		return true;
		}

	// Due to nesting levels, we need to return status.
	symbol_table &symboltable = vbs_engine::symboltable();
	task_type *t = symboltable.get(p->_index)->get_task();
	if (t->trigger(trigger_st_node(p, _parent)) == false)
		{
		p->_delayed = true;
		return false;
		}
	return true;
	}

bool
trigger_stmt::operator()(assignment_stmt *p) const
	{
	// Do we need to delay?
	if (handle_dec(p) != 0)
		return false;

	if (p->_value == 0)
		{
		if (p->_nonblocking)
			{
			p->_update->_lval->set_target();
			p->_update->_value = new num_type(p->_rval->evaluate(evaluate_expr()));
			if (p->_delayed_store != 0)
				{
				// We're using p->_update, so no need set this flag.  p->_value
				// is the flag now.
				//p->_delayed_store->_delayed = true;
				// We want to trigger this statement only, not delay the parent.
				p->_delayed_store->trigger(trigger_dec(p->_update));
				}
			else
				{
				if (!p->_event->is_queued())
					{
					// p->_event was setup using p->_update...
					event_queue<stmt_base> &eventqueue = vbs_engine::eventqueue();
					eventqueue.add_event(p->_event);
					}
				}
			}
		else
			{
			if (p->_delayed_store != 0)
				{
				stmt_type::dec_type *dec = p->_dec;
				if (dec == 0 && !_parent->_always)
					dec = p->_ec;

				p->_lval->set_target(); // Evaluate ahead of update.
				p->_value = new num_type(p->_rval->evaluate(evaluate_expr()));
				if (dec != 0)
					dec->_delayed = true; // Skip inter-statement delay.
				p->_delayed_store->_delayed = true;
				p->_delayed_store->trigger(trigger_dec(_parent));
				return false;
				}
			else
				{
				p->_lval->clr_target();
				const num_type &res(p->_rval->evaluate(evaluate_expr()));
				if (!res.is_tristate())
					p->_lval->trigger(trigger_lvalue(res));
				}
			}
		}
	else
		{
		if (p->_delayed_store != 0)
			p->_delayed_store->_delayed = false;
		if (!p->_value->is_tristate())
			p->_lval->trigger(trigger_lvalue(*p->_value));
		delete p->_value;
		p->_value = 0;
		}
	return true;
	}

bool
trigger_stmt::operator()(if_else_stmt *p) const
	{
	// Do we need to delay?
	if (handle_dec(p) != 0)
		return false;

	if (bool(p->_expr->evaluate(evaluate_expr())) != false)
		return p->_true_stmt->trigger(trigger_stmt(_parent));
	else if (p->_false_stmt != 0)
		return p->_false_stmt->trigger(trigger_stmt(_parent));
	// Expression evaluated to false, but the 'if' statement is
	// considered executed. Thus, return true.
	return true;
	}

bool
trigger_stmt::operator()(case_stmt *p) const
	{
	// Do we need to delay?
	if (handle_dec(p) != 0)
		return false;

	bool ret = true;
	bool status = false;
	// Get the expression result first.
	const num_type &value(p->_expr->evaluate(evaluate_expr()));
	case_stmt::citem_list::iterator itp(p->_case_item->begin());
	case_stmt::citem_list::iterator stop(p->_case_item->end());
	// Find the case item to trigger.
	for (; itp != stop; ++itp)
		{
		ret = (*itp)->trigger(trigger_case_item(p->_type, value, status, _parent));
		if (status != false)
			break;
		}
	// See if something happened, if not, trigger the default case.
	if (p->_def != 0 && status == false)
		ret = p->_def->trigger(trigger_case_item(p->_type, value, status, _parent));
	return ret;
	}

bool
trigger_stmt::operator()(loop_stmt *p) const
	{
	// Do we need to delay?
	if (handle_dec(p) != 0)
		return false;

	// Test whether we were delayed before.
	bool test_cond = false;
	if (p->_delayed == true)
		{
		// If so, we need to reset the flag and jump into the loop.
		p->_delayed = false;
		goto continue_from_delay;
		}

	// Begin work, pre-execution procedure.
	switch (p->_type)
		{
		case loop_stmt::FOREVER:
			test_cond = true;
			break;
		case loop_stmt::REPEAT:
			p->_repeat_count = p->_expr->evaluate(evaluate_expr());
			test_cond = (p->_repeat_count != 0);
			break;
		case loop_stmt::WHILE:
			test_cond = (bool(p->_expr->evaluate(evaluate_expr())) != false);
			break;
		case loop_stmt::FOR:
			p->_preassign->trigger(trigger_stmt(p->_preassign));
			test_cond = (bool(p->_expr->evaluate(evaluate_expr())) != false);
			break;
		default:
			break;
		}
	// Execution procedure.
	while (test_cond)
		{
		if (p->_stmt->trigger(trigger_stmt(_parent)) == false)
			{
			p->_delayed = true;
			if (p->_dec != 0)
				p->_dec->_delayed = true;
			return false;
			}

		// Post-execution procedure.
continue_from_delay:
		switch (p->_type)
			{
			case loop_stmt::FOREVER:
				test_cond = true;
				break;
			case loop_stmt::REPEAT:
				--(p->_repeat_count);
				test_cond = (p->_repeat_count != 0);
				break;
			case loop_stmt::WHILE:
				test_cond = (bool(p->_expr->evaluate(evaluate_expr())) != false);
				break;
			case loop_stmt::FOR:
				p->_postassign->trigger(trigger_stmt(p->_postassign));
				test_cond = (bool(p->_expr->evaluate(evaluate_expr())) != false);
				break;
			default:
				break;
			}
		}
	return true;
	}
