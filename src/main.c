#define _GNU_SOURCE

#include "argparse.h"
#include "container_init.h"

#include <stdio.h>


int main(int argc, char ** argv) {
	struct argopts args = {"", -1, -1, -1, -1};
	parse_args(argc, argv, &args);
	init_container(args);
	printf("Parent exiting...\n");
}
