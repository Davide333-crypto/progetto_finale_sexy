#ifndef INOTIFY_H
#define INOTIFY_H

#include <sys/types.h>
#include <sys/inotify.h>
#include <limits.h>
#include <stddef.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

char *getAbsPath(const char *relativePath, char *absolutePath);

void getEventNameFromMask(uint32_t mask, char *dest, size_t dest_size);

void displayEvent(struct inotify_event *event);

#endif