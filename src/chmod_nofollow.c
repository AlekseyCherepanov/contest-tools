/* chmod dirs as a+rx, files as a+r, ignore symlinks and not regular files. */
/* It skips actual fchmod() if permissions are ok according to fstat(). */
/* It skips symlinks silently. */
/* May be used as ... /home/share/*.pot but NOT deeper!!! */
/* Or from `find` with -execdir ... {} + */

/* Copyright © 2017 Aleksey Cherepanov <lyosha@openwall.com>
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted. */

/* TODO: is it ok to define it here on 64-bit platform? */
#define _FILE_OFFSET_BITS 64

/* For O_NOATIME */
#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define file_mode (S_IRUSR | S_IRGRP | S_IROTH)
#define dir_mode (file_mode | S_IXUSR | S_IXGRP | S_IXOTH)

int main(int argc, char *argv[])
{
    unsigned int i;
    for (i = 1; i < argc; i++) {
        int f = open(argv[i], O_RDONLY | O_NOFOLLOW | O_NOATIME);
        struct stat sb;
        int target;
        if (f == -1) {
            /* Failed to open. Skip. */
            if (errno != ELOOP) {
                perror(argv[i]);
            }
            continue;
        }
        if (fstat(f, &sb) == -1) {
            perror(argv[i]);
            goto close_and_continue;
        }
        if (S_ISREG(sb.st_mode)) {
            /* Regular file */
            target = file_mode;
        } else if (S_ISDIR(sb.st_mode)) {
            /* Directory */
            target = dir_mode;
        } else {
            fprintf(stderr, "%s: is not a directory or regular file.",
                    argv[i]);
            goto close_and_continue;
        }
        if ((sb.st_mode & target) == target) {
            /* Permissions are ok. Skip. */
            goto close_and_continue;
        }
        if (fchmod(f, sb.st_mode | target) == -1) {
            perror(argv[i]);
        }
    close_and_continue:
        if (close(f) == -1) {
            perror(argv[i]);
        }
    }
    return 0;
}
