#define _GNU_SOURCE
#include "container_init.h"

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
#include "argparse.h"

#define CONTAINER_DEFAULT_ROOT "/tmp/container"

int exec_container(void* args) {
	char * argslist[] = {"/bin/sh", NULL};
	return execv(argslist[0], argslist);
}

int clone_func(void * args) {
	char hostname[] = "container-project-host";
	char stack[1024];
	char containerpath[1024];

	if (args != NULL && strlen(args) != 0) {
		strncpy(containerpath, (char *) args, strlen((char *) args));
	} else {
		strncpy(containerpath, CONTAINER_DEFAULT_ROOT, strlen(CONTAINER_DEFAULT_ROOT));
	}

	// clear environment variables so that they dont leak in the new shell
	clearenv();
	setenv("PS1", "\\u@\\H \\W $ ", 1);
	// fetch the alpine minirootfs from the mirror
	mkdir(CONTAINER_DEFAULT_ROOT, S_IRUSR | S_IWUSR);
	init_root_FS(containerpath);
	sethostname(hostname, strlen(hostname));
	// chroot to new root dir
	// TODO: error handling for chroot
	// TODO: take root directory as commandline argument, if it does not exist or is not provided then d
	// use a diretory in /tmp for chroot
	if (chroot(containerpath)) { //returns 0 when successful
		printf("Cannot chroot to container directory!\n");
		exit(1);
	}
	chdir("/");
	mount("proc", "/proc", "proc", 0,0);
	// add cgroup directory for the container_process
	int container_process = clone(exec_container, stack + 1024, SIGCHLD, NULL);
	wait(&container_process);

	//cleanup 
	int retval = umount("proc");
	return 0;
}

int init_container(const struct argopts args) {
	char stack[4096];
	struct cgroup_options cgroup_opts = (struct cgroup_options) {
		args.memlimit,
		args.cpulimit,
		args.swaplimit,
		args.pidlimit
	};
	init_cgroups();
	configure_cgroup_limits(cgroup_opts);
	printf("Done configuring cgroup limits. \n");
	int procid = clone(clone_func, stack + 4096, SIGCHLD | CLONE_NEWUTS | CLONE_NEWPID /*| CLONE_NEWNET */| CLONE_NEWNS, (void *) args.containerpath);
	add_pid_to_cgroup(procid);
	wait(&procid);
	cleanup_cgroup();
	return procid;
}