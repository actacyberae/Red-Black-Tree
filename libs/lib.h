#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define STATELEN 256
#define MAXLINE 2048

void err_msg(const char *buf, ...);

ssize_t write_msg(int fd, char *msg, size_t len);

void print_line(int fd, const void *buf, ...);

void *Calloc(size_t nelem, size_t elsize);

void *Malloc(size_t elsize);
