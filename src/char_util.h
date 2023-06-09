/*
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

/*
 * Author: christian c8121 de
 */

#ifndef CUTILS_CHAR_UTIL
#define CUTILS_CHAR_UTIL

#include <stdlib.h>
#include <string.h>

/**
 * @return A copy of given string
 * Caller must free result
 */
char *str_copy(const char *s, size_t len) {

    char *copy = malloc(len + 1);
    memset(copy, 0, len + 1);
    strncpy(copy, s, len);

    return copy;
}

/**
 * Concatenate two string into new string
 * Caller must free result.
 */
char *str_cat(const char *s1, const char *s2) {
    size_t l1 = strlen(s1);
    char *ret = malloc(l1 + strlen(s2) + 1);

    char *r = ret;
    char *p;
    for (p = (char *) s1; r < (ret + l1); r++)
        *r = *(p++);

    for (p = (char *) s2; *p != '\0'; r++)
        *r = *(p++);
    *r = '\0';

    return ret;
}

/**
 * @return 1 if it is newline , 0 if not
 */
int is_newline(int c) {
    if (c == '\n' || c == '\r')
        return 1;
    return 0;
}

/**
 * @return 1 if it is whitespace (incl. newline), 0 if not
 */
int is_whitespace(int c) {
    if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f')
        return 1;
    return 0;
}

/**
 * @return 1 if s == NULL or *s == 0
 */
int is_null_or_empty(const char *s) {
    if (s == NULL || *s == 0)
        return 1;
    return 0;
}

/**
 * @return 1 if s1 == s2
 */
int is_equal(const char *s1, const char *s2) {

    if (s1 == s2)
        return 1;

    size_t l = strlen(s1);
    if (l != strlen(s2))
        return 0;

    return strncmp(s1, s2, l) == 0 ? 1 : 0;
}

/**
 * Modify given string: Remove any of <code>trim_chars</code> at beginning of <code>s</code>
 */
void ltrim(char *s, const char *trim_chars) {

    char *dst = s;
    for (; strchr(trim_chars, *s) != NULL; ++s);

    if (dst == s)
        return;

    while ((*dst++ = *s++));
}

/**
 * Modify given string: Remove any of <code>trim_chars</code> at end of <code>s</code> (set chars to \0)
 */
void rtrim(char *s, const char *trim_chars) {

    char *p = s + strlen(s);
    while (p > s && (*p == '\0' || strchr(trim_chars, *p) != NULL)) {
        *p = '\0';
        p--;
    }
}

#endif //CUTILS_CHAR_UTIL