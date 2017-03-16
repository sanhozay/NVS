/*
 * NVS - Flags Header
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
 * Program flags structure.
 */
struct flags {
    int coordinates: 1;
    int dme : 1;
    int fuzzy : 1;
    int ils : 1;
    int morse : 1;
    int ndb : 1;
    int quiet : 1;
    int spacing: 1;
    int vor : 1;
};

/* Checks if all restrictions are set.
 *
 * @return true if all search restriction flags are set, i.e. search all
 */
bool all_restrictions();

/**
 * Checks if any specific restriction is set.
 *
 * @return true if any search restriction flag is set.
 */
bool any_restriction();

/**
 * Sets all restriction flags to the same value.
 *
 * @param state the value to set
 */
void set_all_restrictions(const bool state);

/**
 * Sets all restriction flags to their default state.
 */
void set_default_restrictions();

/**
 * Prints restriction flags to standard output, prefixed by a message.
 *
 * For example: show_flags("Searching for");
 *
 * @param prefix the prefix for the flags message
 * @return true if a message was printed
 */
bool show_flags(const char *prefix);

#endif
