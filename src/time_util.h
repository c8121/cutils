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

#ifndef CUTILS_TIME_UTIL
#define CUTILS_TIME_UTIL

#include <stdio.h>
#include <time.h>
#include <regex.h>

#include "char_util.h"

struct time_convert {
    regex_t *regex;
    char *format;
    struct time_convert *next;
};

/**
 *
 */
struct time_convert *time_convert_create(const char *regex, const char *format, struct time_convert *append_to) {

    struct time_convert *result = malloc(sizeof(struct time_convert));

    result->regex = malloc(sizeof(regex_t));
    int ret = regcomp(result->regex, regex, REG_EXTENDED);
    if (ret != 0)
        fail(EX_USAGE, "time_util.h, time_convert_create: Invalid regex");

    result->format = str_copy(format, strlen(format));
    result->next = NULL;

    if (append_to != NULL)
        append_to->next = result;

    return result;
}

struct time_convert *__time_converters = NULL;

/**
 * Check if given string is a valid time format.
 *
 * Valid formats:
 *    yyyy-mm-ss hh:mm:ss
 *    now
 *
 * @return 1 if valid
 */
int is_valid_time_string(const char *time_string) {
    if (is_null_or_empty(time_string))
        return 0;

    if (is_equal("now", time_string))
        return 1;

    if (strnlen(time_string, 19) < 19) {
        fprintf(stderr, "Invalid time format, expected \"yyyy-mm-ss hh:mm:ss\": \"%s\"\n", time_string);
        return 0;
    }

    return 1;
}

/**
 * Take time string or shortcut and convert to "yyyy-mm-ss hh:mm:ss"
 *
 * Available shortcuts:
 *    now
 *
 * @return String on success, NULL if given time string could not be interpreted
 * Caller must free result
 */
char *get_valid_time_string(const char *time_string) {

    if (!is_valid_time_string(time_string))
        return NULL;

    char *s = malloc(20);

    if (is_equal("now", time_string)) {

        time_t secs = time(NULL);
        struct tm *local = localtime(&secs);
        sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
                local->tm_hour, local->tm_min, local->tm_sec);

    } else {

        memset(s, 0, 20);
        strncpy(s, time_string, 19);

    }

    return s;

}

/**
 *
 */
void __init_time_converters() {
    if (__time_converters != NULL)
        return;

    struct time_convert *curr = time_convert_create("^[0-9]{6}$", "%y%m%d", NULL);
    __time_converters = curr;

    curr = time_convert_create("^.+,\\s*[0-9]{1,2}\\s+[A-Za-z]+\\s[0-9]{2}", "%a, %d %b %Y %H:%M:%S", curr);
    curr = time_convert_create(".*", "...", curr);
}

/**
 * Try to convert given time string to "yyyy-mm-ss hh:mm:ss"
 * Caller must free result
 */
char *convert_valid_time_string(const char *time_string) {

    if (time_string == NULL)
        return NULL;

    __init_time_converters();

    struct time_convert *curr = __time_converters;
    while (curr != NULL) {

        int ret = regexec(curr->regex, time_string, 0, NULL, 0);
        if (ret == 0) {
            struct tm t;
            memset(&t, 0, sizeof(struct tm));
            strptime(time_string, curr->format, &t);

            char *s = malloc(20);
            snprintf(s, 20, "%04i-%02i-%02i %02i:%02i:%02i", (t.tm_year + 1900), (t.tm_mon + 1), t.tm_mday,
                     t.tm_hour, t.tm_min, t.tm_sec);
            return s;
        }
        curr = curr->next;
    }

    return NULL;

}


#endif //CUTILS_TIME_UTIL