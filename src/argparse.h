#ifndef ARGPARSE
#define ARGPARSE

struct argopts {
    char containerpath[1024];
    int memlimit;
    int cpulimit;
    int pidlimit;
    int swaplimit;
};

int parse_args(int argc, char ** argv, struct argopts *config);

#endif