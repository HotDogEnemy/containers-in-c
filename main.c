#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include "alpine_init.h"

#define CONTAINER_DEFAULT_ROOT "/home/shivodit/Documents/sp-project/container-root"

int clone_func(void * args) {
	char hostname[1024];
	// clear environment variables so that they dont leak in the new shell
	clearenv();
	// fetch the alpine minirootfs from the mirror
	init_root_FS(CONTAINER_DEFAULT_ROOT);
	// chroot to new root dir
	// TODO: error handling for chroot
	// TODO: take root directory as commandline argument, if it does not exist or is not provided then d
	// use a diretory in /tmp for chroot
	if (chroot(CONTAINER_DEFAULT_ROOT)) { //returns 0 when successful
		printf("Cannot chroot to container-root directory!\n");
		exit(1);
	}
	chdir("/");
	char * argslist[] = {"/bin/sh", NULL};
	execv(argslist[0], argslist);
	return 0;
}

int clone_process() {
	char stack[1024];
	int retval = clone(clone_func, stack + 1024, SIGCHLD, NULL);
	wait(&retval);
	return retval;
}

int main() {
	clone_process();
	printf("Hello world!");
}
