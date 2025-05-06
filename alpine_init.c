#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>

#define ALPINE_MINIROOTFS_URL "https://dl-cdn.alpinelinux.org/alpine/v3.21/releases/aarch64/alpine-minirootfs-3.21.3-aarch64.tar.gz"

// int get_mini_root_FS() { }

// int init() {}



int fetch_from_URL() {
	CURL *curl = curl_easy_init();
	CURLcode res;

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, ALPINE_MINIROOTFS_URL);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	if (res != CURLE_OK) {
		fprintf(stderr, "Encountered an error while fetching Alpine Mini RootFS from mirror. Terminating...\n");
		exit(1);
	}
}

// int unpack_archive();