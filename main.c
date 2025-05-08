#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "alpine_init.h"
#include "cgroup_config.h"

#define CONTAINER_DEFAULT_ROOT "/home/shivodit/Documents/sp-project/container-root"

int exec_container(void* args) {
	char * argslist[] = {"/bin/sh", NULL};
	return execv(argslist[0], argslist);
}

int clone_func(void * args) {
	char hostname[] = "container-project-host";
	char stack[1024];
	// clear environment variables so that they dont leak in the new shell
	clearenv();
	// fetch the alpine minirootfs from the mirror
	mkdir(CONTAINER_DEFAULT_ROOT, S_IFDIR);
	init_root_FS(CONTAINER_DEFAULT_ROOT);
	sethostname(hostname, strlen(hostname));
	// chroot to new root dir
	// TODO: error handling for chroot
	// TODO: take root directory as commandline argument, if it does not exist or is not provided then d
	// use a diretory in /tmp for chroot
	if (chroot(CONTAINER_DEFAULT_ROOT)) { //returns 0 when successful
		printf("Cannot chroot to container-root directory!\n");
		exit(1);
	}
	chdir("/");
	 // init_root_FS("/"); FIXME
	mount("proc", "/proc", "proc", 0,0);
	// add cgroup directory
	int container_process = clone(exec_container, stack + 1024, SIGCHLD, NULL);
	wait(&container_process);
	//cleanup 

	int retval = umount("proc");
	return 0;
}

int clone_process() {
	char stack[1024];
	int procid = clone(clone_func, stack + 1024, SIGCHLD | CLONE_NEWUTS | CLONE_NEWPID, NULL);
	wait(&procid);
	return procid;
}

int main() {
	clone_process();
	printf("Parent exiting...\n");
}
