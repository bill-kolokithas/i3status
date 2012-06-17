// vim:ts=8:expandtab
#include <stdio.h>
#include <stdint.h>

#include "i3status.h"

#define TERABYTE (1024ULL * 1024 * 1024 * 1024)
#define GIGABYTE (1024ULL * 1024 * 1024)
#define MEGABYTE (1024ULL * 1024)
#define KILOBYTE (1024ULL)

/*
 * Prints the given amount of bytes in a human readable manner.
 *
 */
int print_bytes_human(char *outwalk, uint64_t bytes) {
        if (bytes > TERABYTE)
                return sprintf(outwalk, "%.02f TB", (double)bytes / TERABYTE);
        else if (bytes > GIGABYTE)
                return sprintf(outwalk, "%.01f GB", (double)bytes / GIGABYTE);
        else if (bytes > MEGABYTE)
                return sprintf(outwalk, "%.01f MB", (double)bytes / MEGABYTE);
        else if (bytes > KILOBYTE)
                return sprintf(outwalk, "%.01f KB", (double)bytes / KILOBYTE);
        else {
                return sprintf(outwalk, "%.01f B", (double)bytes);
        }
}
