#ifndef CGROUP_CONFIG
#define CGROUP_CONFIG

struct cgroup_options {
	int mem_max;
	int cpu_max;
	int swap_max;
	int max_pids;
};

int init_cgroups();
int add_pid_to_cgroup(const int pid);
int cleanup_cgroup();
int configure_cgroup_limits(struct cgroup_options options);

#endif
