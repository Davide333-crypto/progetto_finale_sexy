#include "inotify.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *getAbsPath(const char *relativePath, char *absolutePath)
{
    return realpath(relativePath, absolutePath);
}

void getEventNameFromMask(uint32_t mask, char *dest, size_t dest_size)
{
    if (!dest || dest_size == 0)
        return;

    dest[0] = '\0';

    if (mask & IN_CREATE)
        strncat(dest, "in_create ", dest_size - strlen(dest) - 1);

    if (mask & IN_DELETE)
        strncat(dest, "in_delete ", dest_size - strlen(dest) - 1);

    if (mask & IN_MOVED_FROM)
        strncat(dest, "in_moved_from ", dest_size - strlen(dest) - 1);

    if (mask & IN_MOVED_TO)
        strncat(dest, "in_moved_to ", dest_size - strlen(dest) - 1);

    if (mask & IN_ISDIR)
        strncat(dest, "in_isdir ", dest_size - strlen(dest) - 1);

    if (mask & IN_DELETE_SELF)
        strncat(dest, "in_delete_self ", dest_size - strlen(dest) - 1);

    if (mask & IN_IGNORED)
        strncat(dest, "in_ignored ", dest_size - strlen(dest) - 1);

    if (dest[0] == '\0')
        strncpy(dest, "unknown", dest_size - 1);
}

void displayEvent(struct inotify_event *event)
{
    char buffer[256];

    getEventNameFromMask(event->mask,
                         buffer,
                         sizeof(buffer));

    printf("wd=%d mask=%s nome=%s\n",
           event->wd,
           buffer,
           (event->len > 0) ? event->name : "");
}