/*
 * NVS - Parse Header
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

#ifndef parse_h
#define parse_h

struct bounds;

/**
 * Parses a navaid from a 810 format string.
 *
 * @param s the 810 format navaid specification
 * @param bounds pointer to a bounds structure (may be NULL)
 * @return a pointer to a navaid structure or NULL if ignored
 */
struct navaid *parse(const char *s, struct bounds *bounds);

#endif
