/*
 * This file is licensed under Public Domain or Creative Commons Zero.
 *
 * Author: Robert 'Bobby' Zenz
 *         robert.zenz@bonsaimind.org
 */

#define HOME_TEMPLATE "/tmp/guest-home-XXXXXX"
#define SHELL "/bin/sh"
#define SHELL_ARGS {NULL}


#define _BSD_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	// Create a temporary home.
	char *template = strdup(HOME_TEMPLATE);
	char *home = mkdtemp(template);
	
	pid_t shellPid = fork();
	
	if (shellPid == 0) {
		setenv("HOME", home, 1);
		
		char *execArgs[] = SHELL_ARGS;
		execv(SHELL, execArgs);
	} else {
		waitpid(shellPid, NULL, 0);
		
		// The shell we started has exited, let's remove the home.
		// Unfortunately there does not seem to be an easy command
		// to remove a non-empty directory except running rm.
		char *command = strdup("rm -rf ");
		command = strcat(command, "\"");
		command = strcat(command, home);
		command = strcat(command, "\"");
		
		printf("%s", command);
		system(command);
	}
	
	return 0;
}