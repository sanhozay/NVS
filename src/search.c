/*
 * NVS - Search Implementation
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

#include "search.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flags.h"
#include "morse.h"
#include "types.h"
#include "util.h"

/**
 * Maximum length of a formatted coordinate
 */
#define COORDINATE_MAX 32

/**
 * Creates a description of a navaid type
 *
 * @param type the navaid type
 * @return a string that describes the navaid type
 */
static char *description(const enum NavaidType type)
{
    switch(type) {
    case NDB:
        return "NDB";
    case VOR:
        return "VOR";
    case ILS:
        return "ILS";
    case LOC:
        return "LOC";
    case DME:
    case SDM:
        return "DME";
    default:
        assert(0);
        return NULL;
    }
}

/**
 * Formats a coordinate into a string.
 *
 * @param c the coordinate to format
 * @return a string representation of the coordinate
 */
static char *format(const struct coordinate c)
{
    extern struct flags flags;
    static char s[COORDINATE_MAX] = "";

    if (flags.coordinates) {
        char ns = c.lat >= 0 ? 'N' : 'S';
        char ew = c.lon >= 0 ? 'E' : 'W';
        snprintf(s, COORDINATE_MAX,
            "(%08.04f%c, %08.04f%c)",
            fabs(c.lat), ns,
            fabs(c.lon), ew
        );
    }
    return s;
}

/**
 * Common print function, used by NDB, VOR and DME.
 *
 * @param navaid a pointer to a navaid structure
 */
static void print_common(const struct navaid *navaid)
{
    extern struct flags flags;
    printf("%s %-4s %s %6.02f %3dnm %5dft %s %s\n",
        description(navaid->type),
        navaid->code,
        format(navaid->coordinate),
        navaid->frequency,
        navaid->range,
        navaid->elevation,
        navaid->name,
        flags.morse ? morse(navaid->code, " ") : ""
    );
}

/**
 * Prints the description of an ILS/LOC to standard output.
 *
 * @param navaid a pointer to a navaid structure
 */
static void print_loc(const struct navaid *navaid)
{
    extern struct flags flags;
    printf("%s %-4s %s %6.02f %3dnm %5dft %s-%-3s %03.0f° %s %s\n",
        description(navaid->type),
        navaid->code,
        format(navaid->coordinate),
        navaid->frequency,
        navaid->range,
        navaid->elevation,
        navaid->icao,
        navaid->runway,
        navaid->extra.bearing,
        navaid->name,
        flags.morse ? morse(navaid->code, " ") : ""
    );
}

/**
 * Prints the description of a DME to standard output.
 *
 * @param navaid a pointer to a navaid structure
 */
static void print_dme(const struct navaid *navaid)
{
    if (navaid->icao == NULL || navaid->runway == NULL)
        print_common(navaid);
    else {
        extern struct flags flags;
        printf("%s %-4s %s %6.02f %3dnm %5dft %s-%-3s %s %s\n",
            description(navaid->type),
            navaid->code,
            format(navaid->coordinate),
            navaid->frequency,
            navaid->range,
            navaid->elevation,
            navaid->icao,
            navaid->runway,
            navaid->name,
            flags.morse ? morse(navaid->code, " ") : ""
        );
    }
}

/**
 * Prints the description of a navaid to standard output.
 *
 * @param navaid a pointer to a navaid structure
 */
void print(const struct navaid *navaid)
{
    switch (navaid->type) {
    case NDB:
    case VOR:
        print_common(navaid);
        break;
    case ILS:
    case LOC:
        print_loc(navaid);
        break;
    case DME:
    case SDM:
        print_dme(navaid);
        break;
    default:
        return;
    }
}

/**
 * Checks if a navaid matches the given search term.
 *
 * @param term the search term, as entered on the command line
 * @param navaid the navaid to test
 * @return true if the navaid matches the search term
 */
static inline bool match(const char *term, const struct navaid *navaid)
{
    if (navaid == NULL)
        return false;
    if (strncmp(term, navaid->code, CODE_MAX) == 0)
        return true;
    if (navaid->icao != NULL && strncmp(term, navaid->icao, ICAO_MAX) == 0)
        return true;

    extern struct flags flags;
    if (flags.fuzzy && strstr(navaid->name, term) != NULL)
        return true;

    return false;
}

/**
 * Finds a navaid and prints its description to standard output.
 *
 * @param cache the navaid cache
 * @param code the code to search for
 * @return the number of navaids that match the search term
 */
int find(struct navaid **cache, const char *code)
{
    char *term = strdup_f(code);
    char *p = term;
    while ((*p = toupper(*p)))
        ++p;

    struct navaid *navaid;
    int matches = 0;
    while ((navaid = *cache++) != NULL) {
        if (match(term, navaid)) {
            print(navaid);
            ++matches;
        }
    }
    free(term);
    return matches;
}
