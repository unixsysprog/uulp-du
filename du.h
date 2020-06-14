#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <memory.h>
#include <unistd.h>
#include <sys/stat.h>

long do_du(char[]);
long do_stat(char *);

static int blk_size = 512;
