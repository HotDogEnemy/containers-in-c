#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "alpine_init.h"

int init_root_FS(const char * rootpath) {
	// get the full qualified path for the archive
	char * fullpath = (char * ) malloc(strlen(rootpath) + strlen(ALPINE_ARCHIVE_NAME));
	strcpy(fullpath, rootpath);
	strcat(fullpath, ALPINE_ARCHIVE_NAME);
	fetch_from_URL(fullpath);
	unpack_archive(fullpath);
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

int unpack_archive(const char * archive_path) {
	char * args[] = {
		"/bin/tar",
		"xvf",
		archive_path,
		NULL
	};
	execv(args[0], args);
}	