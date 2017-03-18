/**
 * @file types.h
 *
 * Global data type definitions.
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

/**
 * Maximum length of a navaid code.
 */
#define CODE_MAX 8

/**
 * Maximum length of an ICAO airport code.
 */
#define ICAO_MAX 8

/**
 * Maximum length of a runway code.
 */
#define RWAY_MAX 8

/**
 * Navaid types.
 */
enum NavaidType {
    NIL = 0,    ///< Not valid or unassigned
    NDB = 2,    ///< Non-Directional Beacon
    VOR = 3,    ///< VOR and VORTAC
    ILS = 4,    ///< ILS localizer
    LOC = 5,    ///< Localizer
    GS  = 6,    ///< Glideslope
    OM  = 7,    ///< Outer Marker
    MM  = 8,    ///< Middle Marker
    IM  = 9,    ///< Inner Marker
    DME = 12,   ///< DME component of VOR or ILS
    SDM = 13,   ///< Standalone or NDB DME
    EOD = 99    ///< End of data marker
};

/**
 * Geospatial coordinate.
 */
struct coordinate {
    double lat; ///< Latitide
    double lon; ///< Longitude
};

/**
 * Bounds.
 */
struct bounds {
    struct coordinate min; ///< Minimum bounds coordinate (bottom left)
    struct coordinate max; ///< Maximum bounds coordinate (top right)
};

/**
 * Navaid structure.
 */
struct navaid {
    enum NavaidType type;           ///< Type of navaid (VOR, NDB, etc.)
    struct coordinate coordinate;   ///< Coordinate
    int elevation;                  ///< Elevation above sea level in feet
    int range;                      ///< Reception range in nm
    double frequency;               ///< Radio frequency
    union {
        float unused;               ///< NDB unused
        float variation;            ///< VOR twist
        float bearing;              ///< ILS bearing (true)
        float bias;                 ///< DME bias
    } extra;                        ///< Navaid specific field
    char *code;                     ///< Identification code
    char *icao;                     ///< Airport ICAO code (ILS/LOC/DME)
    char *runway;                   ///< Runway code (ILS/LOC)
    char *name;                     ///< Descriptive name
};

#endif
