/**
 * @file cache.c
 *
 * Manage an in-memory cache of navaids for searching.
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

#include "cache.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zlib.h>

#include "parse.h"
#include "types.h"

/**
 * Size of chunks allocated dynamically as part of the navaid cache.
 */
#define CHUNKSIZE 8192

/**
 * Navigation data file handle.
 */
static gzFile gz;

/**
 * Closes resources on exit.
 *
 * The only closeable resource is the navigation data file. This function must
 * be registered with atexit before the navigation data file is opened.
 */
static void exit_handler()
{
    if (gz != NULL && gzclose_r(gz) != Z_OK)
        fprintf(stderr, "Error closing data file: %s", gzerror(gz, NULL));
}

/**
 * Dynamic cache structure.
 *
 * The cache is a NULL-terminated contiguous array of pointers to navaid
 * structures. Space is allocated in chunks and the max field keeps track
 * of the largest index that can be used on the array.
 */
struct dynamic_cache {
    int chunks;             ///< Number of chunks allocated to the cache
    int max;                ///< Maximum index available in the cache
    struct navaid **cache;  ///< Dynamically allocated array of navaid pointers
};

/**
 * Initializes an empty dynamic cache.
 *
 * @param d a pointer to a dynamic cache structure
 */
static void init(struct dynamic_cache *d)
{
    assert(d != NULL);
    d->chunks = 0;
    d->max = d->chunks * CHUNKSIZE - 1;
    d->cache = NULL;
}

/**
 * Creates a new chunk of memory in a dynamic cache.
 *
 * The chunks field is incremented to track the number of chunks and the
 * max field is updated to indicate the maximum index available in the
 * array of pointers that form the cache.
 *
 * @param d a pointer to a dynamic cache structure
 */
static void create_chunk(struct dynamic_cache *d)
{
    assert(d != NULL);
    size_t size = ++d->chunks * CHUNKSIZE * sizeof(struct navaid *);
    d->max = d->chunks * CHUNKSIZE - 1;
    if ((d->cache = realloc(d->cache, size)) == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
}

/**
 * Preprocesses raw lines from the navigation data file.
 *
 * Newlines and carriage returns are stripped from the end of the string
 * and all characters are converted to uppercase. Conversion to uppercase
 * improves consistency in the output and produces a marginal performance
 * improvement during searches.
 *
 * @param s a line from the navigation data file
 * @return the length of the processed string
 */
static int preprocess(char *s)
{
    int n = 0;
    for (; *s; ++s, ++n) {
        if (*s == '\n' || *s == '\r') {
            *s = '\0';
            break;
        }
        *s = toupper(*s);
    }
    return n;
}

/**
 * Checks if the navigation data is a supported version, based on its header.
 *
 * Prints a message to standard error and exits with failure status if version
 * cannot be determined or is not supported.
 *
 * @param header the header line from the navigation data file
 */
static void check_version(const char *header)
{
    assert(header != NULL && strlen(header) > 0);
    int version;
    if (sscanf(header, "%d", &version) == 0) {
        fprintf(stderr, "Malformed navigation data header:\n%s\n", header);
        exit(EXIT_FAILURE);
    }
    if (version != 810) {
        fprintf(stderr, "Unsupported navigation data version %d\n", version);
        exit(EXIT_FAILURE);
    }
}

/**
 * Creates a navaid cache.
 *
 * The cache is built from the compressed navigation data file as a
 * dynamically expanding contiguous array of pointers to navaid structures.
 * Each line of the input file is converted to uppercase and trimmed, before
 * being passed to a parser that creates navaid structures to add to the cache.
 *
 * The data file is located through the FG_ROOT environment variable.
 *
 * This function never returns NULL. Any errors in reading the input file or
 * allocating memory for the cache result in a message printed to standard
 * error and the program terminating with an error status.
 *
 * The pointer returned from this function must be freed after use (and the
 * structures to which it points may also need to be freed).
 *
 * @param bounds a pointer to a bounds structure
 * @return an array of pointers to navaid structures, terminated with NULL
 */
struct navaid **create_cache(struct bounds *bounds)
{
    char *fg_root;
    if ((fg_root = getenv("FG_ROOT")) == NULL) {
        fprintf(stderr, "Missing environment variable FG_ROOT\n");
        exit(EXIT_FAILURE);
    }

    char *path = NULL, *ndgz = "Navaids/nav.dat.gz";
    size_t size = strlen(fg_root) + strlen("/") + strlen(ndgz) + 1;
    if ((path = malloc(size)) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    snprintf(path, size, "%s/%s", fg_root, ndgz);

    if (atexit(exit_handler) != 0) {
        perror("atexit");
        exit(EXIT_FAILURE);
    }

    if ((gz = gzopen(path, "rb")) == NULL) {
        fprintf(stderr, "Failed to open %s\n", path);
        exit(EXIT_FAILURE);
    }

    struct dynamic_cache dcache;
    init(&dcache);

    bool have_spec = false;
    char buf[BUFSIZ];
    struct navaid *navaid;
    int i = 0;
    while (gzgets(gz, buf, BUFSIZ) != NULL) {
        if (preprocess(buf) == 0)
            continue;
        if (!have_spec) {
            check_version(buf);
            have_spec = true;
        } else if ((navaid = parse(buf, bounds)) != NULL) {
            if (i > dcache.max)
                create_chunk(&dcache);
            dcache.cache[i++] = navaid;
        }
    }

    if (i == 0) {
        fputs("Did not find any navigation data in data file\n", stderr);
        exit(EXIT_FAILURE);
    }

    int error;
    const char *gzmsg = gzerror(gz, &error);
    switch(error) {
    case Z_OK:
        if (i > dcache.max)
            create_chunk(&dcache);
        dcache.cache[i] = NULL;
        break;
    case Z_ERRNO:
        fprintf(stderr, "Problems reading %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
        break;
    default:
        fprintf(stderr, "Problems reading %s: %s\n", path, gzmsg);
        exit(EXIT_FAILURE);
        break;
    }

    free(path);
    return dcache.cache;
}

/**
 * Destroys a single navaid structure.
 *
 * @param navaid a pointer to a navaid structure
 */
static void destroy_navaid(struct navaid *navaid)
{
    if (navaid != NULL) {
        free(navaid->code);
        free(navaid->icao);
        free(navaid->name);
        free(navaid->runway);
    }
    free(navaid);
}

/**
 * Destroys a navaid cache.
 *
 * @param cache an array of pointers to navaid structures
 */
void destroy_cache(struct navaid **cache)
{
    struct navaid **p = cache;
    while (*p)
        destroy_navaid(*p++);
    free(cache);
}
