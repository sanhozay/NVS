/*
 * NVS - Cache Header
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

#ifndef cache_h
#define cache_h

struct bounds;

/**
 * Creates an array of pointers to navaid structures for searching.
 *
 * @return an array of pointers to navaid structures
 */
struct navaid **create_cache(struct bounds *bounds);

/**
 * Destroys a navaid cache.
 *
 * @param cache an array of pointers to navaid structures
 */
void destroy_cache(struct navaid **cache);

#endif
