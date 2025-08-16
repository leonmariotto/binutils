#include "elftool_structs.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <byteswap.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <elf.h>
#include <endian.h>

#include <stdlib.h>

#define STBLOCKS_UNIT 512

int elftool_load(elftool_t *bin, char *bin_path) {
  struct stat stats;
  int fd;
  int r = 0;

  if ((fd = open(bin_path, O_RDONLY)) <= 0) {
    r = EINVAL;
    fprintf(stderr, "open failed");
  }
  if (r == 0) {
    if (fstat(fd, &stats)) {
      r = EINVAL;
      fprintf(stderr, "fstat failed");
    }
  }
  if (r == 0) {
    if ((stats.st_mode & S_IFMT) != S_IFREG) {
      r = EINVAL;
      fprintf(stderr, "This is not a regular file, abort");
    }
  }
  if (r == 0) {
    bin->length = stats.st_blocks * STBLOCKS_UNIT;
    if (bin->length % 4096) {
      r = EINVAL;
      fprintf(stderr, "bin->length not aligned to pagesize");
    }
  }
  if (r == 0) {
    //if (!(bin->mem = mmap(NULL, bin->length, PROT_READ | PROT_WRITE,
    //                      MAP_PRIVATE, fd, 0))) {
    //  r = EINVAL;
    //  fprintf(stderr, "mmap call failed");
    //}
    if (!(bin->mem = malloc(bin->length))) {
      r = ENOMEM;
      fprintf(stderr, "malloc call failed\n");
    }
    ssize_t r = read(fd, bin->mem, bin->length);
    if (r < 0) {
      fprintf(stderr, "Read return error\n");
      r = EINVAL;
    }
  }
  if (fd >= 0) {
    close(fd);
  }
  return (r);
}
