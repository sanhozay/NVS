/**
 * @file flags.c
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

#include "flags.h"

#include <stdio.h>

/**
 * Program flags.
 */
struct flags flags;

/**
 * Checks if all navaid restrictions are set, i.e.\ all types will be searched.
 *
 * @return true if all search restriction flags are set, otherwise false
 */
bool all_restrictions(void)
{
    return flags.dme && flags.ils && flags.ndb && flags.vor;
}

/**
 * Checks if any specific navaid restriction is set.
 *
 * @return true if any search restriction flag is set, otherwise false
 */
bool any_restriction(void)
{
    return flags.dme || flags.ils || flags.ndb || flags.vor;
}

/**
 * Sets all navaid restriction flags to the same value.
 *
 * @param state the value to set
 */
void set_all_restrictions(const bool state)
{
    flags.dme = flags.ils = flags.ndb = flags.vor = state;
}

/**
 * Sets all navaid restriction flags to their default value.
 *
 * Default is to search for all types apart from DME. DMEs are usually
 * co-located with another type.
 */
void set_default_restrictions(void)
{
    set_all_restrictions(true);
    flags.dme = false;
}

/**
 * Prints navaid search flags to standard output, prefixed by a message.
 *
 * Prints a message if the search is a fuzzy search or the search excludes
 * at least one type of navaid (including DME).
 *
 * @param prefix the prefix for the flags message
 * @return true if a message was printed
 */
bool show_flags(const char *prefix)
{
    if (all_restrictions() && !flags.fuzzy)
        return false;

    if (prefix != NULL)
        printf("%s", prefix);

    if (flags.dme) printf(" DME");
    if (flags.ils) printf(" ILS");
    if (flags.ndb) printf(" NDB");
    if (flags.vor) printf(" VOR");

    if (flags.fuzzy) printf(" (including names)");

    putchar('\n');
    return true;
}
