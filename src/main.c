/*
 * NVS - Main Program
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

#include "main.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache.h"
#include "flags.h"
#include "search.h"
#include "types.h"
#include "util.h"

#define BOUNDS_WILDCARD "*"
#define SPACER_LENGTH 1
#define SPACER_CHAR '-'

struct bounds *create_bounds();
void parse_bounds(const char *s, struct bounds *bounds);
bool show_bounds(const struct bounds *bounds);
void spacer(int n);
void usage();
bool valid(const struct bounds *bounds);

/**
 * Main program.
 */
int main(int argc, char **argv)
{
    extern struct flags flags;

    static struct option longopts[] = {
        {"all", no_argument, NULL, 'a'},
        {"bounds", required_argument, NULL, 'b'},
        {"coordinates", no_argument, NULL, 'c'},
        {"quiet", no_argument, NULL, 'q'},
        {"fuzzy", no_argument, NULL, 'f'},
        {"help", no_argument, NULL, 'h'},
        {"morse", no_argument, NULL, 'm'},
        {"spacers", no_argument, NULL, 's'},
        {"dme", no_argument, NULL, 'd'},
        {"ils", no_argument, NULL, 'i'},
        {"ndb", no_argument, NULL, 'n'},
        {"vor", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };

    if (argc <= 1) {
        usage();
        exit(EXIT_FAILURE);
    }

    struct bounds *bounds = NULL;
    int c;
    while ((c = getopt_long(argc, argv, "ab:cdfhimnvqs", longopts, NULL)) != -1)
        switch (c) {
        case 'a':
            set_all_restrictions(true);
            break;
        case 'b':
            bounds = create_bounds();
            parse_bounds(optarg, bounds);
            if (!valid(bounds)) {
                fprintf(stderr, "Invalid bounds: "
                    "top=%.02f, right=%.02f, bottom=%.02f, left=%.02f\n",
                    bounds->max.lat, bounds->max.lon,
                    bounds->min.lat, bounds->min.lon
                );
                exit(EXIT_FAILURE);
            }
            break;
        case 'c':
            flags.coordinates |= 1;
            break;
        case 'd':
            flags.dme |= 1;
            break;
        case 'f':
            flags.fuzzy |= 1;
            break;
        case 'h':
            usage();
            exit(EXIT_SUCCESS);
            break;
        case 'i':
            flags.ils |= 1;
            break;
        case 'm':
            flags.morse |= 1;
            break;
        case 'n':
            flags.ndb |= 1;
            break;
        case 'v':
            flags.vor |= 1;
            break;
        case 'q':
            flags.quiet |= 1;
            break;
        case 's':
            flags.spacing |= 1;
            break;
        default:
            usage();
            exit(EXIT_FAILURE);
            break;
        }

    argc -= optind;
    argv += optind;

    if (argc == 0) {
        usage();
        exit(EXIT_FAILURE);
    }

    if (!any_restriction())
        set_default_restrictions();

    if (!flags.quiet) {
        bool f = show_flags("Searching for");
        bool b = show_bounds(bounds);
        if ((f || b) && flags.spacing)
            spacer(SPACER_LENGTH);
    }

    struct navaid **cache = create_cache(bounds);
    free(bounds);

    for (; argc--; argv++) {
        int matches = find(cache, *argv);
        if (!flags.quiet && matches == 0)
            printf("%s not found\n", *argv);
        if (flags.spacing)
            spacer(SPACER_LENGTH);
    }
    destroy_cache(cache);

    return EXIT_SUCCESS;
}

/**
 * Create a bounds structure and returns a pointer to it.
 *
 * The pointer must be freed after use.
 *
 * @return a pointer to an initialized bounds structure
 */
struct bounds *create_bounds()
{
    struct bounds *bounds;
    if ((bounds = malloc(sizeof(struct bounds))) == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    bounds->max.lat = 90;
    bounds->max.lon = 180;
    bounds->min.lat = -bounds->max.lat;
    bounds->min.lon = -bounds->max.lon;
    return bounds;
}

/**
 * Parse search bounds from a string.
 *
 * Elements of the search bounds can be omitted to use the defaults
 * established in create_bounds. A complete bounds specification would
 * have four elements, e.g. "60,2,50,-2". A partial bounds specification
 * can have fewer elements, but they must be delimited, e.g. "60,,50,".
 *
 * @param s the bounds specification, e.g. 60,2,50,-4.
 * @param bounds a pointer to a bounds structure
 */
void parse_bounds(const char *s, struct bounds *bounds)
{
    char *b = strdup_f(s);

    double *lookup[4];
    lookup[0] = &bounds->max.lat;
    lookup[1] = &bounds->max.lon;
    lookup[2] = &bounds->min.lat;
    lookup[3] = &bounds->min.lon;

    char *tok, *end;
    int i = 0;
    while ((tok = strtok(i == 0 ? b : NULL, ",")) != NULL) {
        if (strlen(tok) > 0 && strncmp(tok, BOUNDS_WILDCARD, 1) != 0) {
            double d;
            if ((d = strtod(tok, &end)) == 0 && end == tok) {
                fprintf(stderr, "Invalid token in bounds: %s\n", tok);
                exit(EXIT_FAILURE);
            } else {
                *lookup[i] = d;
            }
        }
        ++i;
    }
    free(b);
}

/**
 * Shows the bounds in use.
 *
 * @param bounds a pointer to a bounds structure
 * @return true if bounds are in use, otherwise false
 */
bool show_bounds(const struct bounds *bounds)
{
    if (bounds == NULL)
        return false;

    printf("Using bounds top=%.02f, right=%.02f, bottom=%.02f, left=%.02f\n",
        bounds->max.lat, bounds->max.lon,
        bounds->min.lat, bounds->min.lon
    );
    return true;
}

/**
 * Prints a spacer line to standard output.
 *
 * The spacer line consists of a single character repeated a configurable
 * number of times, followed by a newline.
 *
 * @param n the number of spacer characters to print
 */
void spacer(int n)
{
    while (n--)
        putchar(SPACER_CHAR);
    putchar('\n');
}

/**
 * Prints a usage message to standard output.
 */
void usage()
{
    printf("nvs v%s\n", NVS_VERSION);
    puts("Usage: nvs [OPTIONS] ITEMS ...");
    puts("  -a, --all              Search for all navaid types, including DME");
    puts("  -b, --bounds=<bounds>  Bounded by [t],[r],[b],[l] (wildcard '*')");
    puts("  -c, --coordinates      Show coordinates");
    puts("  -f, --fuzzy            Search names as well as codes");
    puts("  -h, --help             Show this help message");
    puts("  -m, --morse            Show Morse code for each navaid");
    puts("  -q, --quiet            Don't display additional messages");
    puts("  -s, --spacers          Add spacer lines between results");
    puts("Search restrictions (multiples may be combined):");
    puts("  -d, --dme              Search for DMEs, including standalone");
    puts("  -i, --ils              Search for ILS/LOC");
    puts("  -n, --ndb              Search for NDBs");
    puts("  -v, --vor              Search for VOR/VORTAC");
}

/**
 * Checks if bounds are valid.
 *
 * The only checks performed are whether the maximum latitude and
 * longitude are greater than the corresponding minimums and whether
 * they are in range.
 *
 * @param bounds a pointer to a bounds structure
 * @return true if the bounds are valid
 */
bool valid(const struct bounds *bounds)
{
    return bounds->max.lat <= 90.0 &&
        bounds->min.lat >= -90.0 &&
        bounds->max.lon <= 180.0 &&
        bounds->min.lon >= -180.0 &&
        bounds->max.lat > bounds->min.lat &&
        bounds->max.lon > bounds->min.lon;
}
