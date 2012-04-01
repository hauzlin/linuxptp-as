/**
 * @file print.c
 * @note Copyright (C) 2011 Richard Cochran <richardcochran@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "print.h"

static int verbose;
static int print_level = LOG_INFO;
static int use_syslog = 1;

void print_no_syslog(void)
{
	use_syslog = 0;
}

void print_set_level(int level)
{
	print_level = level;
}

void print_verbose(void)
{
	verbose = 1;
}

void print(int level, char const *format, ...)
{
	pid_t pid;
	va_list ap;
	char buf[1024];

	if (level > print_level)
		return;

	pid = getpid();

	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap);
	va_end(ap);

	if (verbose) {
		fprintf(stdout, "ptp4l[%d]: %s\n", pid, buf);
		fflush(stdout);
	}
	if (use_syslog) {
		syslog(level, "[%d] %s", pid, buf);
	}
}