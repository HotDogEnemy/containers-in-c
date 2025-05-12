#include "argparse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
    Set up arg parsing. Supported list of args:
    d: Set directory for container root
    m: Set max mem limit
    c: Set max cpu limit
    s: Set max swap limit
*/
int parse_args(int argc, char ** argv, struct argopts *config) {
    int c;

    if (config == NULL) {
        return 0;  // we cant set parameters when config is null
    }

    while ((c = getopt(argc, argv, "d:m:c:s:p:")) != -1) {
        switch (c) {
            case 'd':
                printf("d = %s\n", optarg);
                strcpy(config->containerpath, optarg);
                break;
            case 'm':
                printf("m = %s\n", optarg);
                config->memlimit = atoi(optarg);
                break;
            case 'c':
                printf("c = %s\n", optarg);
                config->cpulimit = atoi(optarg);
                break;
            case 's':
                printf("s = %d\n", optarg);
                config->swaplimit = atoi(optarg);
                break;
            case 'p':
                printf("p = %s\n");
                config->pidlimit = atoi(optarg);
                break;
        }
    }

    printf("Configured options: \n");
    printf("config->containerpath = %s\n", config->containerpath);
    printf("config->memlimit = %dM\n", config->memlimit);
    printf("config->cpulimit = %d%%\n", config->cpulimit);
    printf("config->swaplimit = %dM\n", config->swaplimit);
    printf("config->pidlimit = %d\n", config->pidlimit);
    return 0;
}