/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999-2002 Andrea Mazzoleni
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "os.h"
#include "conf.h"
#include "mouseall.h"
#include "target.h"
#include "portable.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void probe(void)
{
	int i, j;

	printf("Mouses %d\n", mouseb_count_get());
	for(i=0;i<mouseb_count_get();++i) {
		printf("mouse %d, buttons %d\n", i, mouseb_button_count_get(i));
		for(j=0;j<mouseb_axe_count_get(i);++j) {
			printf("\taxe %d [%s]\n", j, mouseb_axe_name_get(i, j));
		}
		for(j=0;j<mouseb_button_count_get(i);++j) {
			printf("\tbutton %d [%s]\n", j, mouseb_button_name_get(i, j));
		}
	}

	printf("\n");
}

static int done;

void sigint(int signum)
{
	done = 1;
}

void run(void)
{
	char msg[1024];
	char new_msg[1024];
	int i, j;
	target_clock_t last;

	printf("Press Break to exit\n");

	signal(SIGINT, sigint);

	last = target_clock();
	msg[0] = 0;
	while (!done) {

		new_msg[0] = 0;
		for(i=0;i<mouseb_count_get();++i) {
			if (i!=0)
				sncat(new_msg, sizeof(new_msg), "\n");

			snprintf(new_msg + strlen(new_msg), sizeof(new_msg) - strlen(new_msg), "mouse %d, [", i);
			for(j=0;j<mouseb_button_count_get(i);++j) {
				if (mouseb_button_get(i, j))
					sncat(new_msg, sizeof(new_msg), "_");
				else
					sncat(new_msg, sizeof(new_msg), "-");
			}
			sncat(new_msg, sizeof(new_msg), "], [");

			for(j=0;j<mouseb_axe_count_get(i);++j) {
				int v = mouseb_axe_get(i, j);

				if (j)
					sncat(new_msg, sizeof(new_msg), ",");

				snprintf(new_msg + strlen(new_msg), sizeof(new_msg) - strlen(new_msg), "%6d", v);
			}
			sncat(new_msg, sizeof(new_msg), "]");
		}

		if (strcmp(msg, new_msg)!=0) {
			target_clock_t current = target_clock();
			double period = (current - last) * 1000.0 / TARGET_CLOCKS_PER_SEC;
			sncpy(msg, sizeof(msg), new_msg);
			snprintf(new_msg + strlen(new_msg), sizeof(new_msg) - strlen(new_msg), " (%4.0f ms)", period);
			last = current;
			printf("%s\n", new_msg);
		}

		os_poll();
		mouseb_poll();
		target_idle();
	}
}

static void error_callback(void* context, enum conf_callback_error error, const char* file, const char* tag, const char* valid, const char* desc, ...)
{
	va_list arg;
	va_start(arg, desc);
	vfprintf(stderr, desc, arg);
	fprintf(stderr, "\n");
	if (valid)
		fprintf(stderr, "%s\n", valid);
	va_end(arg);
}

void os_signal(int signum)
{
	os_default_signal(signum);
}

int os_main(int argc, char* argv[])
{
	int i;
	adv_conf* context;
        const char* section_map[1];
	adv_bool opt_log;
	adv_bool opt_logsync;

	opt_log = 0;
	opt_logsync = 0;

	context = conf_init();

	if (os_init(context) != 0)
		goto err_conf;

	mouseb_reg(context, 1);
	mouseb_reg_driver_all(context);

	if (conf_input_args_load(context, 0, "", &argc, argv, error_callback, 0) != 0)
		goto err_os;

	for(i=1;i<argc;++i) {
		if (target_option(argv[i], "log")) {
			opt_log = 1;
		} else if (target_option(argv[i], "logsync")) {
			opt_logsync = 1;
		} else {
			fprintf(stderr, "Unknown argument '%s'\n", argv[1]);
			goto err_os;
		}
	}

	if (opt_log || opt_logsync) {
		const char* log = "advm.log";
		remove(log);
		log_init(log, opt_logsync);
        }

	section_map[0] = "";
	conf_section_set(context, section_map, 1);

	if (mouseb_load(context) != 0)
		goto err_os;

	if (os_inner_init("AdvanceMOUSE") != 0)
		goto err_os;

	if (mouseb_init() != 0)
		goto err_os_inner;

	probe();
	run();

	mouseb_done();
	os_inner_done();

	log_std(("m: the end\n"));

	if (opt_log || opt_logsync) {
		log_done();
	}

	os_done();
	conf_done(context);

	return EXIT_SUCCESS;

err_os_inner:
	os_inner_done();
	log_done();
err_os:
	os_done();
err_conf:
	conf_done(context);
	return EXIT_FAILURE;
}

