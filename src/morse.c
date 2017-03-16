/*
 * NVS - Morse Implementation
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

#include "morse.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

#define MORSE_MAX 512

static char* morse_letters[26] = {
    ".-",    /* A */
    "-...",  /* B */
    "-.-.",  /* C */
    "-..",   /* D */
    ".",     /* E */
    "..-.",  /* F */
    "--.",   /* G */
    "....",  /* H */
    "..",    /* I */
    ".---",  /* J */
    "-.-",   /* K */
    ".-..",  /* L */
    "--",    /* M */
    "-.",    /* N */
    "---",   /* O */
    ".--.",  /* P */
    "--.-",  /* Q */
    ".-.",   /* R */
    "...",   /* S */
    "-",     /* T */
    "..-",   /* U */
    "...-",  /* V */
    ".--",   /* W */
    "-..-",  /* X */
    "-.--",  /* Y */
    "--.."  /* Z */
};

char *morse_numbers[10] = {
    "-----", /* 0 */
    ".----", /* 1 */
    "..---", /* 2 */
    "...--", /* 3 */
    "....-", /* 4 */
    ".....", /* 5 */
    "-....", /* 6 */
    "--...", /* 7 */
    "---..", /* 8 */
    "----."  /* 9 */
};

/**
 * Translate a character into a morse string.
 *
 * @param c the character to translate
 * @return the Morse translation
 */
static char *translate(const char c)
{
    if (isupper(c)) return morse_letters[c - 'A'];
    if (islower(c)) return morse_letters[c - 'a'];
    if (isdigit(c)) return morse_numbers[c - '0'];
    return NULL;
}

/**
 * Generate a Morse code representation of a string.
 *
 * The returned string is statically allocated and does not need to be
 * freed after use.
 *
 * @param s the string to represent as Morse code
 * @param delim the delimiter between Morse characters
 * @return individual characters of s converted to Morse
 */
char *morse(const char *s, const char *delim)
{
    static char buf[MORSE_MAX];
    *buf = '\0';

    const char *p;
    for (p = s; *p; p++) {
        if (strlen(buf) > 0 && append(buf, delim, MORSE_MAX) == NULL) {
            fprintf(stderr, "Morse translation of %s is too long\n", s);
            return NULL;
        }
        char *m;
        if ((m = translate(*p)) == NULL) {
            fprintf(stderr, "No morse translation for %c in %s\n", *p, s);
            return NULL;
        }
        if (append(buf, m, MORSE_MAX) == NULL) {
            fprintf(stderr, "Morse translation of %s is too long\n", s);
            return NULL;
        }
    }
    return buf;
}
