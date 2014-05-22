/*
 * This file is licensed under Public Domain or Creative Commons Zero.
 *
 * Author: Robert 'Bobby' Zenz
 *         robert.zenz@bonsaimind.org
 */

#define HOME_TEMPLATE "/tmp/guest-home-XXXXXX"
#define SHELL "/bin/bash"


#define _BSD_SOURCE 1

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>


void do_login_shell();
void do_shell();
void run(char *command_to_run, int args_count, ...);


int main(int argc, char **argv) {
	if (argv[0][0] == '-') {
		do_login_shell(argv);
	} else {
		do_shell(argv);
	}
	
	return 0;
}

/**
 * Sets up a temporary home and runs the real shell. After the real shell
 * has exited the temporary home gets deleted.
 *
 * @param argv the arguments for the real shell.
 */
void do_login_shell(char **argv) {
	// Create a temporary home.
	char *template = strdup(HOME_TEMPLATE);
	char *home = mkdtemp(template);
	
	pid_t shellPid = fork();
	
	if (shellPid == 0) {
		setenv("HOME", home, 1);
		
		execv(SHELL, argv);
	} else {
		waitpid(shellPid, NULL, 0);
		
		// The shell we started has exited, let's remove the home.
		// Unfortunately there does not seem to be an easy command
		// to remove a non-empty directory except running rm.
		run("rm", 2, "-rf", home);
	}
}

/**
 * Runs the real shell.
 *
 * @param argv the arguments for the real shell.
 */
void do_shell(char **argv) {
	execv(SHELL, argv);
}

/**
 * Runs the given command with the given parameters. The parameters will be
 * concatenated into one string and they will be quoted.
 *
 * @param command_to_run the command to run.
 * @param args_count the count of the arguments that follow.
 * @param ... the arguments itself.
 */
void run(char *command_to_run, int args_count, ...) {
	char *command = strdup(command_to_run);
	
	va_list args;
	va_start(args, args_count);
	for (int idx = 0; idx < args_count; idx++) {
		command = strcat(command, " \"");
		command = strcat(command, va_arg(args, char *));
		command = strcat(command, "\"");
	}
	va_end(args);
	
	printf("%s\n", command);
	system(command);
}

