/*
 * du.c : A simple implementation of du
 *
 * starts in designated directory (or current directory),
 * cycles through contained directories recursively and
 * prints allocated blocks.
 */

#include "du.h"

int main(int argc, char *argv[]) {
  int opt;

  if (argc == 1) {
    do_du(".");
  } else if (argc < 4) {

    while ((opt = getopt(argc, argv, "k")) != -1) {

      switch (opt) {
        case 'k':                             // change default block size
          blk_size = 1024;
          break;
        default:                              // Error
          fprintf(stderr, "Usage: ./du [-k] [file...]\n");
          exit(EXIT_FAILURE);

      } // switch

    } // while

    do_du(argv[argc-1]);                      // pass do_du last argument (should be path) and ignore return
  } else {
    fprintf(stderr, "Usage: ./du [-k] [file...]\n");
    exit(EXIT_FAILURE);
  } // if

} // main

/*
 * do_du
 * if path is a directory, recurse
 * if path is a file, do_stat
 * From both, add stat's reported size, then return size
 */
long do_du(char path[]) {

  DIR *dir;
  struct dirent * direntp;
  long size = do_stat(path);

  if ((dir = opendir(path)) != NULL) {

    while ((direntp = readdir(dir)) != NULL) { // Cycle through all paths

      if ((strcmp(direntp->d_name, "..") != 0) && (strcmp(direntp->d_name, ".") != 0)) {
        char fullpath[PATH_MAX + 1];

        strcpy(fullpath, path);
        strcat(fullpath, "/");
        strcat(fullpath, direntp->d_name);     // Build full path

        if (direntp->d_type == DT_DIR) {       // path is directory
          size += do_du(fullpath);             // recurse
        } else {                               // path is file
          size += do_stat(fullpath);           // get size from stat
        } // if

      } // if

    } // while

    closedir(dir);                             // cleanup
    printf("%8ld ", size / blk_size);          // print size
    printf("%s\n", path);                      // print full path
    return size;                               // return for sake of recursion

  } // if

  return -1;                                   // Error

} // do_du

long do_stat(char *path) {
  struct stat info;

  if (stat(path, &info) == -1) {               // stat messed up
    perror(path);                              // find out why
  } else {
    return (long)info.st_blocks * 512;         // multiply by default block size so size can be divided by blk_size
  } // if

  return -1;                                   // Error

} // do_stat
