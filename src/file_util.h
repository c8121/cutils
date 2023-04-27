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

#ifndef CUTILS_FILE_UTIL
#define CUTILS_FILE_UTIL

#define READLINE_BUF_SIZE 1024

#include <sys/stat.h>
#include <stdio.h>

#include "char_util.h"
#include "char_buffer_util.h"

/**
 * @return 1 if it does exist and is a regular file, 0 if not
 */
int file_exists(const char *file_name) {
    if (is_null_or_empty(file_name))
        return 0;

    struct stat file_stat;
    if (stat(file_name, &file_stat) == 0) {
        if (S_ISREG(file_stat.st_mode))
            return 1;
    }
    return 0;
}

/**
 * @return 1 if it does exist and is a directory, 0 if not
 */
int dir_exists(const char *dir_name) {
    if (is_null_or_empty(dir_name))
        return 0;

    struct stat file_stat;
    if (stat(dir_name, &file_stat) == 0) {
        if (S_ISDIR(file_stat.st_mode))
            return 1;
    }
    return 0;
}

/**
 * Safely read a line, no matter how long it might be (so you don't have to care about buffer sizes)
 *
 * @return line including newline or NULL
 * Caller must free result
 */
char *freadline(FILE *in) {

    char buf[READLINE_BUF_SIZE];
    struct char_buffer *cb = NULL;
    size_t len;
    while (fgets(buf, READLINE_BUF_SIZE, in) != NULL) {
        len = strnlen(buf, READLINE_BUF_SIZE);
        cb = char_buffer_append(cb, buf, len);

        //Read one line only
        if (buf[len - 1] == '\n')
            break;
    }

    if (cb == NULL)
        return NULL;

    char *ret = char_buffer_copy(cb);
    char_buffer_free(cb);

    return ret;
}

/**
 * Find filename extension (.txt for example).
 * Caller must free result
 */
char *file_ext(const char *file_name, int max_len, const char *default_ext) {

    char *p = file_name != NULL ? strrchr(file_name, '.') : NULL;
    if (p == NULL)
        return str_copy(default_ext, strnlen(default_ext, max_len));

    return str_copy(p + 1, max_len);
}

#endif //CUTILS_FILE_UTIL