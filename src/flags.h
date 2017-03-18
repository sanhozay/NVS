/**
 * @file flags.h
 *
 * Manage program flags.
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

#ifndef options_h
#define options_h

#include <stdbool.h>

/**
 * Program flags
 */
struct flags {
    int coordinates: 1; ///< Show coordinates
    int dme : 1;        ///< Search for DME
    int fuzzy : 1;      ///< Fuzzy search (search names as well as codes)
    int ils : 1;        ///< Search for ILS/LOC
    int morse : 1;      ///< Display Morse code ident
    int ndb : 1;        ///< Search for NDB
    int quiet : 1;      ///< Suppress extra messages
    int spacing: 1;     ///< Add spacers between search results
    int vor : 1;        ///< Search for VOR
};

bool all_restrictions();
bool any_restriction();
void set_all_restrictions(const bool state);
void set_default_restrictions();
bool show_flags(const char *prefix);

#endif
