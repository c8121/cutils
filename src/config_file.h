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

#ifndef CUTILS_CONFIG_FILE
#define CUTILS_CONFIG_FILE

#include <stdio.h>
#include <string.h>

#include "file_util.h"
#include "char_util.h"
#include "util.h"
#include "cli_args.h"


#define MAX_LEN_SECTION_NAME 512
#define MAX_LEN_NAME 512
#define MAX_LEN_VALUE 4096

/**
 * Read config file like this:
 *
 * <code><pre>
 * [Section]
 * name: value
 * name: value
 *
 * (...)
 * </pre></code>
 *
 * Note: Use quotation for multiline-values of to keep whitespace at beginning of values.
 *
 * @return 1 on success, 0 on failure
 */
int
read_config_file(const char *file_name, void (*apply_config_function)(char *section_name, char *name, char *value)) {

    if (file_exists(file_name) == 0)
        return 0;

    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open config-file: %s\n", file_name);
        return 0;
    }

    char section_name[MAX_LEN_SECTION_NAME];
    memset(section_name, 0, MAX_LEN_SECTION_NAME);

    char name[MAX_LEN_NAME];
    memset(name, 0, MAX_LEN_NAME);

    char value[MAX_LEN_VALUE];
    memset(value, 0, MAX_LEN_VALUE);

    int pos = 1; //0 = section, 1 = name, 2 = value
    char *p = name;
    int last_was_newline = 0;
    int last_was_escape = 0;
    int in_quote = 0;
    int ignore = 0;

    int c;
    int l = 0;
    while ((c = fgetc(fp)) != EOF) {

        //Keep quoted string as they are
        if (c == '"') {
            if (in_quote) {
                if (!last_was_escape) {
                    in_quote = 0;
                    continue;
                } else
                    *(p--) = c; //Overwrite escape char
            } else {
                in_quote = 1;
                continue;
            }
        } else if (in_quote) {
            last_was_escape = c == '\\'; //Only check for escaped quote-char
            *(p++) = c;
            continue;
        }

        if ((l == 0 || last_was_newline) && c == '#') {
            ignore = 1;
            continue; //Begin of commented out line
        }

        if (ignore) {
            if (is_newline(c)) { //Ignore until next line
                last_was_newline = 0;
                ignore = 0;
            }
            continue;
        }

        if (c == '[' && last_was_newline) {
            //Begin of section
            pos = 0;
            p = section_name;
            memset(section_name, 0, MAX_LEN_SECTION_NAME);
        } else if (c == ']' && pos == 0) {
            //End of section, begin of name
            pos = 1;
            p = name;
            memset(name, 0, MAX_LEN_NAME);
        } else if (c == ':' && pos == 1) {
            //End of name, begin of value
            pos = 2;
            p = value;
            memset(value, 0, MAX_LEN_VALUE);
        } else if (is_newline(c) && pos == 2) {
            //End of value, begin of next name
            if (*name != 0 && *value != 0)
                apply_config_function(section_name, name, value);
            pos = 1;
            p = name;
            memset(name, 0, MAX_LEN_NAME);
        } else {
            if (is_whitespace(c)) {
                if (pos == 2 && p != value) //Ignore whitespace in names and begin of values
                    *(p++) = c;
            } else {
                *(p++) = c;
            }
        }

        last_was_newline = is_newline(c);
        l++;
    }

    if (*name != 0 && *value != 0)
        apply_config_function(section_name, name, value);

    fclose(fp);

    return 1;
}

/**
 * Load and apply config, if given cli argument exists.
 * Do nothing if cli argument does not exist.
 * Fail with EX_IOERR if file does nox exist and parameter 'call_fail' is true (not 0)
 * Load from default config file if no config argument given.
 *
 * @return 1 on success or if no config file given, 0 on failure
 */
int read_config_file_from_cli_arg(const char *cli_arg,
                                  int argc, char *argv[], int call_fail,
                                  const char *default_config_file,
                                  void (*apply_config_function)(char *section_name, char *name, char *value)) {

    int i = cli_get_opt_idx(cli_arg, argc, argv);
    if (i > 0) {
        if (read_config_file(argv[i], apply_config_function) == 0) {
            if (call_fail)
                fail(EX_IOERR, "Failed to read from config file");
            else
                return 0;
        }
    } else if (!is_null_or_empty(default_config_file)) {
        if (read_config_file(default_config_file, apply_config_function) == 0) {
            if (call_fail)
                fail(EX_IOERR, "Failed to read from default config file");
            else
                return 0;
        }
    }

    return 1;
}

/**
 * Get file relative to argv[0], uses realpath(...).
 * @return realpath from $0/file_name, or NULL if file does not exist
 * Caller must free result
 */
char *get_config_file_path(int argc, char *argv[], const char *file_name) {

    char dir[FILENAME_MAX];
    memset(&dir, 0, FILENAME_MAX);

    realpath(argv[0], dir);
    char *p = strrchr(dir, '/');
    if (p != NULL)
        *p = '\0';
    else
        dir[0] = '\0';

    char path[FILENAME_MAX];
    memset(&path, 0, FILENAME_MAX);
    snprintf(path, FILENAME_MAX - strlen(file_name) - 2, "%s/%s", dir, file_name);

    char *ret = malloc(FILENAME_MAX);
    memset(ret, 0, FILENAME_MAX);
    realpath(path, ret);

    if(!file_exists(ret)) {
        fprintf(stderr, "File not found: %s\n", ret);
        free(ret);
        return NULL;
    }

    return ret;
}

#endif //CUTILS_CONFIG_FILE