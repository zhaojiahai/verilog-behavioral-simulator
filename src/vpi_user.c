/*
 * Verilog Behavioral Simulator
 * Copyright (C) 2000 Jimen Ching
 *
 * This file is part of the Verilog Behavioral Simulator package.
 * See the file COPYRIGHT for copyright and disclaimer information.
 * See the file COPYING for the licensing terms and conditions.
 * See the file CONTRIBUTORS for a list of contributing authors.
 *
 * vpi_user.c
 *
 * Implementation of VPI functions.
 */

#include "vpi_user.h"

int vpi_errno;

int
vpi_chk_error(p_vpi_error_info error_info_p)
	{
	if (vpi_errno != 0)
		{
		if (error_info_p != 0)
			{
			error_info_p->state = vpiRun;
			error_info_p->level = vpiInternal;
			error_info_p->message = "Unsupported VPI call";
			error_info_p->product = "VBS";
			error_info_p->code = "unused";
			error_info_p->file = 0;
			error_info_p->line = 0;
			}
		return 1;
		}

	return 0;
	}

int
vpi_compare_objects(vpiHandle obj1, vpiHandle obj2)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

int
vpi_free_object(vpiHandle obj)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

int
vpi_get(int prop, vpiHandle obj)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

void
vpi_get_cb_info(vpiHandle obj, p_cb_data cb_data_p)
	{
	p_cb_data data_p = (p_cb_data) obj;
	cb_data_p->reason = data_p->reason;
	cb_data_p->cb_rtn = data_p->cb_rtn;
	cb_data_p->obj = data_p->obj;
	cb_data_p->time = data_p->time;
	cb_data_p->value = data_p->value;
	cb_data_p->user_data = data_p->user_data;
	}

void
vpi_get_delay(vpiHandle obj, p_vpi_delay delay_p)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	}

char *
vpi_get_str(int prop, vpiHandle obj)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

void
vpi_get_systf_info(vpiHandle obj, p_vpi_systf_data systf_data_p)
	{
	p_vpi_systf_data data_p = (p_vpi_systf_data) obj;
	systf_data_p->type = data_p->type;
	systf_data_p->sysfunctype = data_p->sysfunctype;
	systf_data_p->tfname = data_p->tfname;
	systf_data_p->calltf = data_p->calltf;
	systf_data_p->compiletf = data_p->compiletf;
	systf_data_p->sizetf = data_p->sizetf;
	systf_data_p->user_data = data_p->user_data;
	}

void
vpi_get_time(vpiHandle obj, p_vpi_time time_p)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	}

void
vpi_get_value(vpiHandle obj, p_vpi_value value_p)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	}

int
vpi_get_vlog_info(p_vpi_vlog_info vlog_info_p)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

vpiHandle
vpi_handle(int type, vpiHandle ref)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return ref;
	}

vpiHandle
vpi_handle_by_index(vpiHandle obj, int index)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return obj;
	}

vpiHandle
vpi_handle_by_name(char *name, vpiHandle scope)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return scope;
	}

vpiHandle
vpi_handle_multi(int type, vpiHandle ref1, vpiHandle ref2, ...)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return ref1;
	}

vpiHandle
vpi_iterate(int type, vpiHandle ref)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return ref;
	}

unsigned int
vpi_mcd_close(unsigned int mcd)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return mcd;
	}

char *
vpi_mcd_name(unsigned int cd)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

unsigned int
vpi_mcd_open(char *file)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

int
vpi_mcd_printf(unsigned int mcd, char *format, ...)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

int
vpi_printf(char *format, ...)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

void
vpi_put_delays(vpiHandle obj, p_vpi_delay delay_p)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	}

vpiHandle
vpi_put_value(vpiHandle obj, p_vpi_value value_p, p_vpi_time time_p, int flags)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

vpiHandle
vpi_register_cb(p_cb_data cb_data_p)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

vpiHandle
vpi_register_systf(p_vpi_systf_data systf_data_p)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

int
vpi_remove_cb(vpiHandle cb_obj)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}

vpiHandle
vpi_scan(vpiHandle itr)
	{
	vpi_errno = !0; /* FIXME: anything to cause vpi_chk_error to work. */
	return 0;
	}
