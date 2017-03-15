/*
 * NVS - Search Header
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

#ifndef nav_h
#define nav_h

struct navaid;

/**
 * Finds a navaid and prints information to standard output.
 *
 * @param cache the navaid cache
 * @param code the navaid code to search for
 * @return the number of matching navaids
 */
int find(struct navaid **cache, const char *code);

#endif
