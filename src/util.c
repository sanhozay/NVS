/**
 * @file util.c
 *
 * Utility functions.
 *
 * Copyright (c) 2017 Richard Senior
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/// @cond Doxygen_Suppress
#define _POSIX_C_SOURCE 200809L
/// @endcond

#include "util.h"

#include <stdlib.h>
#include <string.h>

/**
 * Appends a string to a buffer with bounds checking.
 *
 * @param buf the buffer
 * @param s the string to append
 * @param size the size of the buffer
 * @return the new string on success, otherwise NULL
 */
char *append(char *buf, const char *s, size_t size)
{
    if (strlen(buf) + strlen(s) + 1 >= size)
        return NULL;
    return strcat(buf, s);
}

/**
 * Duplicates a string in dynamic storage.
 *
 * Exits with failure status when out of memory, never returns NULL. The
 * returned pointer must be freed after use.
 *
 * @param s the string to duplicate
 * @return a duplicate of the string
 */
char *strdup_f(const char *s)
{
    if (s == NULL)
        return NULL;

    char *d;
    if ((d = strdup(s)) == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    return d;
}
