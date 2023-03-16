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

#ifndef CUTILS_DB_UTIL
#define CUTILS_DB_UTIL

#include "char_util.h"

struct db_config {
    const char *host;
    const char *db;
    const char *user;
    const char *password;
    int port;
};

/**
 *
 */
void
apply_db_config(struct db_config *db_config, const char *config_file_section, char *section_name, char *name, char *value) {
    if (strcmp(config_file_section, section_name) == 0 && strcmp("host", name) == 0)
        db_config->host = str_copy(value, strlen(value));
    if (strcmp(config_file_section, section_name) == 0 && strcmp("db", name) == 0)
        db_config->db = str_copy(value, strlen(value));
    if (strcmp(config_file_section, section_name) == 0 && strcmp("user", name) == 0)
        db_config->user = str_copy(value, strlen(value));
    if (strcmp(config_file_section, section_name) == 0 && strcmp("password", name) == 0)
        db_config->password = str_copy(value, strlen(value));
    if (strcmp(config_file_section, section_name) == 0 && strcmp("port", name) == 0)
        db_config->port = atoi(value);
}

#endif //CUTILS_DB_UTIL