#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "alpine_init.h"

struct unpack_paths {
	char fullpath[1024];
	char rootpath[1024];
};

int init_root_FS(const char * rootpath) {
	char stack[1024];
	struct unpack_paths *paths = (struct unpack_paths*) malloc(sizeof(struct unpack_paths));
	// get the full qualified path for the archive
	char * fullpath = (char * ) malloc(strlen(rootpath) + strlen(ALPINE_ARCHIVE_NAME));
	printf("rootpath = %s\n", rootpath);
	mkdir(rootpath, S_IRUSR | S_IWUSR);
	strcpy(fullpath, rootpath);
	strcat(fullpath, ALPINE_ARCHIVE_NAME);
	printf("fullpath = %s\n", fullpath);
	fetch_from_URL(fullpath);

	strcpy(paths->fullpath, fullpath);
	strcpy(paths->rootpath, rootpath);

	printf("\tpaths->fullpath = %s\n\tpaths->rootpath = %s\n", paths->fullpath, paths->rootpath);

	int unpack_child_id = clone(unpack_archive, stack + 1024, SIGCHLD, paths);
	wait(&unpack_child_id);
	remove(paths->fullpath);
	free(paths);
	return 0;
}

int fetch_from_URL(const char * rootpath) {
	CURL *curl = curl_easy_init();
	CURLcode res;
	FILE *file = fopen(rootpath, "wb");

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, ALPINE_MINIROOTFS_URL);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(file);
	}

	if (res != CURLE_OK) {
		fprintf(stderr, "Encountered an error while fetching Alpine Mini RootFS from mirror (error code %d). Terminating...\n", res);
		exit(1);
	}
}

int unpack_archive(void * arg) {
	struct unpack_paths *paths = (struct unpack_paths *) arg;
	printf("Unpack archive:\n archive_path = %s\n containerdir = %s\n", paths->fullpath, paths->rootpath);
	char * args[] = {
		"/bin/tar",
		"xvf",
		paths->fullpath,
		"-C",
		paths->rootpath,
		NULL
	};
	execv(args[0], args);
}
