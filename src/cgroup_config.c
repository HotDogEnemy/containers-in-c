#include "cgroup_config.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define CGROUP_DIR_PATH "/sys/fs/cgroup/container-1"
#define CONCAT_PATH(a, b) (a"/"b)

#define MAX_MEM "memory.max"
#define MAX_CPU "cpu.max"
#define MAX_SWAP "memory.swap.max"
#define MAX_PID "pids.max"

int init_cgroups () {
	struct stat dirstat = {0};

	if (stat(CGROUP_DIR_PATH, &dirstat) == -1) {
		printf("Creating cgroup directory for container...\n");
		if (mkdir(CGROUP_DIR_PATH, S_IRUSR | S_IWUSR) == -1) {
			printf("ERROR: Could not create directory for cgroups (error code %d). Exiting...\n", errno);
		}
	}
}


int add_pid_to_cgroup(const int pid) {
	char buf[15];
	char * filepath = (char *) malloc(strlen(CGROUP_DIR_PATH) + strlen("/cgroup.procs") + 1); //we add one extra to account for null char
	strcpy(filepath, CGROUP_DIR_PATH);
	strcat(filepath, "/cgroup.procs");
	int fd = open(filepath, O_WRONLY);
	sprintf(buf, "%d\n", pid);
	write(fd, buf, strlen(buf));
	close(fd);
	free(filepath);
}

char * concatenate_path(const char * base, const char * additional) {
	printf("Inside concatenate_path() function\n");
	char * resultstr = (char *) malloc(strlen(base) + strlen(additional) + 2); // +2 to handle forward slash and \0
	strcpy(resultstr, base);
	strcat(resultstr, "/");
	strcat(resultstr, additional);

	return resultstr;
}

int cgroup_write_rule(int value, const char * property) {
	int fp = -1;
	char buf[20];
	char * fullpath = concatenate_path(CGROUP_DIR_PATH, property);
	if (value == -1) {
		return 0;  // -1 means no value was given, so we return
	}
	if (strcmp(MAX_MEM, property) == 0 || strcmp(MAX_SWAP, property) == 0) {
		sprintf(buf, "%dM", value);
	} else {
		sprintf(buf, "%d", value);
	}

	fp = open(fullpath, O_WRONLY);
	
	if (fp != -1) {
		write(fp, buf, strlen(buf));
		close(fp);
	}

	free(fullpath);
}

int configure_cgroup_limits(struct cgroup_options options) {
	printf("Writing Cgroup rules\n");
	cgroup_write_rule(options.mem_max, MAX_MEM);
	cgroup_write_rule(options.cpu_max, MAX_CPU);
	cgroup_write_rule(options.swap_max, MAX_SWAP);
	cgroup_write_rule(options.max_pids, MAX_PID);
			
	return 0; // TODO: Implement me
}


int cleanup_cgroup() {
	if (rmdir(CGROUP_DIR_PATH) == -1) {
		printf(" Error while cleaning up cgroup sysfs (error code %d). Exiting...\n", errno);
		exit(1);
	}
	return 0; // TODO: Implement me
}
