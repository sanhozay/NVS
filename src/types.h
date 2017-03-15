/*
 * NVS - Common Types Header
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

#ifndef types_h
#define types_h

#define CODE_MAX 8
#define ICAO_MAX 8
#define RWAY_MAX 8

/**
 * Navaid types.
 */
enum NavaidType {
    NIL = 0,    /* Not valid or unassigned */
    NDB = 2,    /* Non-Directional Beacon */
    VOR = 3,    /* VOR and VORTAC */
    ILS = 4,    /* ILS localizer */
    LOC = 5,    /* Localizer */
    GS  = 6,    /* Glideslope */
    OM  = 7,    /* Outer Marker */
    MM  = 8,    /* Middle Marker */
    IM  = 9,    /* Inner Marker */
    DME = 12,   /* DME component of VOR or ILS */
    SDM = 13,   /* Standalone or NDB DME*/
    EOD = 99    /* End of data marker */
};

/**
 * Coordinate.
 */
struct coordinate {
    double lat;
    double lon;
};

/**
 * Bounds.
 */
struct bounds {
    struct coordinate min;
    struct coordinate max;
};

/**
 * Navaid structure.
 */
struct navaid {
    enum NavaidType type;
    struct coordinate coordinate;
    int elevation;
    int range;
    double frequency;
    union {
        float unused;       /* NDB unused */
        float variation;    /* VOR twist */
        float bearing;      /* ILS bearing (true) */
        float bias;         /* DME bias */
    } extra;
    char *code;
    char *icao;
    char *runway;
    char *name;
};

#endif
