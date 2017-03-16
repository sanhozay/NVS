/*
 * NVS - Morse Header
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

#ifndef morse_h
#define morse_h

/**
 * Generate a Morse code representation of a string.
 *
 * @param s the string to represent as Morse code
 * @param delim the delimiter between Morse characters
 * @return individual characters of s converted to Morse
 */
char *morse(const char *s, const char *delim);

#endif
