#ifndef ALPINE_INIT
#define ALPINE_INIT

#define ALPINE_MINIROOTFS_URL "https://dl-cdn.alpinelinux.org/alpine/v3.21/releases/aarch64/alpine-minirootfs-3.21.3-aarch64.tar.gz"
#define ALPINE_ARCHIVE_NAME "/alpine_minirootfs.tar.gz"

int init_root_FS(const char * containerpath);
int unpack_archive(void * arg);
int fetch_from_URL(const char * rootpath);

#endif