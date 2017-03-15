/*
 * NVS - Parse
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

#include "parse.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flags.h"
#include "types.h"
#include "util.h"

/**
 * Creates a navaid structure on the heap.
 *
 * The structure must be freed after use.
 *
 * @return a pointer to a navaid structure (never returns NULL)
 */
static struct navaid *create_navaid()
{
    struct navaid *n;
    if ((n = calloc(1, sizeof(struct navaid))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    return n;
}

/**
 * Returns a navaid if its coordinate falls within bounds.
 *
 * @param navaid the navaid to check
 * @param bounds the bounds
 * @return the navaid if in bounds, otherwise NULL
 */
static struct navaid *in_bounds(struct navaid *navaid, struct bounds *bounds)
{
    if (bounds == NULL) return navaid;

    if (navaid->coordinate.lat < bounds->min.lat) return NULL;
    if (navaid->coordinate.lat > bounds->max.lat) return NULL;
    if (navaid->coordinate.lon < bounds->min.lon) return NULL;
    if (navaid->coordinate.lon > bounds->max.lon) return NULL;

    return navaid;
}

/**
 * Parses an NDB from a 810 format string.
 *
 * @param s the 810 format navaid specification
 * @param bounds pointer to a bounds structure (may be NULL)
 * @return a pointer to a navaid structure
 */
static struct navaid *parse_ndb(const char *s, struct bounds *bounds)
{
    extern struct flags flags;
    if (!flags.ndb)
        return NULL;

    struct navaid *navaid = create_navaid();
    char code[CODE_MAX];
    int n;
    sscanf(s, "%d %lf %lf %d %lf %d %f %7s %n",
        (int *)&navaid->type,
        &navaid->coordinate.lat,
        &navaid->coordinate.lon,
        &navaid->elevation,
        &navaid->frequency,
        &navaid->range,
        &navaid->extra.unused,
        code,
        &n
    );
    navaid->code = strdup_f(code);
    navaid->name = strdup_f(s + n);

    return in_bounds(navaid, bounds);
}

/**
 * Parses a VOR from a 810 format string.
 *
 * @param s the 810 format navaid specification
 * @param bounds pointer to a bounds structure (may be NULL)
 * @return a pointer to a navaid structure
 */
static struct navaid *parse_vor(const char *s, struct bounds *bounds)
{
    extern struct flags flags;
    if (!flags.vor)
        return NULL;

    struct navaid *navaid = create_navaid();
    char code[CODE_MAX];
    int n;
    sscanf(s, "%d %lf %lf %d %lf %d %f %7s %n",
        (int *)&navaid->type,
        &navaid->coordinate.lat,
        &navaid->coordinate.lon,
        &navaid->elevation,
        &navaid->frequency,
        &navaid->range,
        &navaid->extra.variation,
        code,
        &n
    );
    navaid->frequency /= 100;
    navaid->code = strdup_f(code);
    navaid->name = strdup_f(s + n);

    return in_bounds(navaid, bounds);
}

/**
 * Parses an ILS/LOC from a 810 format string.
 *
 * @param s the 810 format navaid specification
 * @param bounds pointer to a bounds structure (may be NULL)
 * @return a pointer to a navaid structure
 */
static struct navaid *parse_loc(const char *s, struct bounds *bounds)
{
    extern struct flags flags;
    if (!flags.ils)
        return NULL;

    struct navaid *navaid = create_navaid();
    char code[CODE_MAX], icao[ICAO_MAX], runway[RWAY_MAX];
    int n;
    sscanf(s, "%d %lf %lf %d %lf %d %f %7s %7s %7s %n",
        (int *)&navaid->type,
        &navaid->coordinate.lat,
        &navaid->coordinate.lon,
        &navaid->elevation,
        &navaid->frequency,
        &navaid->range,
        &navaid->extra.bearing,
        code,
        icao,
        runway,
        &n
    );
    navaid->frequency /= 100;
    navaid->code = strdup_f(code);
    navaid->icao = strdup_f(icao);
    navaid->name = strdup_f(s + n);
    navaid->runway = strdup_f(runway);

    return in_bounds(navaid, bounds);
}

/**
 * Parses a DME from a 810 format string.
 *
 * @param s the 810 format navaid specification
 * @param bounds pointer to a bounds structure (may be NULL)
 * @return a pointer to a navaid structure
 */
static struct navaid *parse_dme(const char *s, struct bounds *bounds)
{
    extern struct flags flags;
    if (!flags.dme)
        return NULL;

    struct navaid *navaid = create_navaid();
    char code[CODE_MAX], icao[ICAO_MAX], runway[RWAY_MAX];
    int n;
    if (strstr(s, "DME-ILS") == NULL) {
        sscanf(s, "%d %lf %lf %d %lf %d %f %7s %n",
            (int *)&navaid->type,
            &navaid->coordinate.lat,
            &navaid->coordinate.lon,
            &navaid->elevation,
            &navaid->frequency,
            &navaid->range,
            &navaid->extra.bias,
            code,
            &n
        );
    } else {
        sscanf(s, "%d %lf %lf %d %lf %d %f %7s %7s %7s %n",
            (int *)&navaid->type,
            &navaid->coordinate.lat,
            &navaid->coordinate.lon,
            &navaid->elevation,
            &navaid->frequency,
            &navaid->range,
            &navaid->extra.bias,
            code,
            icao,
            runway,
            &n
        );
        navaid->icao = strdup_f(icao);
        navaid->runway = strdup_f(runway);
    }
    navaid->frequency /= 100;
    navaid->code = strdup_f(code);
    navaid->name = strdup_f(s + n);

    return in_bounds(navaid, bounds);
}

/**
 * Parses a navaid from a 810 format string.
 *
 * @param s the 810 format navaid specification
 * @param bounds pointer to a bounds structure (may be NULL)
 * @return a pointer to a navaid structure or NULL if ignored
 */
struct navaid *parse(const char *s, struct bounds *bounds)
{
    enum NavaidType type = NIL;
    sscanf(s, "%d", (int *)&type);

    switch (type) {
    case NDB:
        return parse_ndb(s, bounds);
    case VOR:
        return parse_vor(s, bounds);
    case ILS:
    case LOC:
        return parse_loc(s, bounds);
    case GS:
    case OM:
    case MM:
    case IM:
        return NULL;
    case DME:
    case SDM:
        return parse_dme(s, bounds);
    case EOD:
        return NULL;
    default:
        fprintf(stderr, "Unexpected navaid type %d in data file\n", type);
        exit(EXIT_FAILURE);
    }
}
