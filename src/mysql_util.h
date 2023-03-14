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

#ifndef CUTILS_MYSQL_UTIL
#define CUTILS_MYSQL_UTIL

#include <mysql/mysql.h>

/**
 * @return 1 on success, 0 otherwise
 */
int __mysql_prepare_stmt(void *conn, MYSQL_STMT **stmt, const char *sql) {

    if (conn == NULL) {
        fprintf(stderr, "%s", "Not connected\n");
        return 0;
    }

    if (*stmt == NULL) {
        *stmt = mysql_stmt_init(conn);
        if (mysql_stmt_prepare(*stmt, sql, strlen(sql)) != 0) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            return 0;
        }
    }

    return 1;
}

/**
 *
 */
MYSQL_BIND *__mysql_create_bind(enum enum_field_types type, void *value) {

    MYSQL_BIND *bind = malloc(sizeof(MYSQL_BIND));
    memset(bind, 0, sizeof(MYSQL_BIND));
    bind->buffer_type = type;
    bind->buffer = value;
    bind->is_null = 0;

    return bind;
}

/**
 *
 */
MYSQL_BIND *__mysql_create_binds(int count) {

    MYSQL_BIND *bind = malloc(sizeof(MYSQL_BIND) * count);
    memset(bind, 0, sizeof(MYSQL_BIND) * count);

    for (int i = 0; i < count; i++) {
        bind[i].is_null = 0;
    }

    return bind;
}


/**
 * @return -1 on fail, 0 on no data found, 1 on success
 */
int __mysql_fetch_result(void *conn, MYSQL_STMT *stmt, MYSQL_BIND *param, MYSQL_BIND *result) {

    if (mysql_stmt_bind_param(stmt, param) != 0) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    if (mysql_stmt_bind_result(stmt, result) != 0) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    if (mysql_stmt_store_result(stmt) != 0) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    int ret = 1;
    int s = mysql_stmt_fetch(stmt);
    if (s == MYSQL_NO_DATA) {
        ret = 0;
    } else if (s != 0) {
        fprintf(stderr, "mysql_stmt_fetch = %i, %s\n", s, mysql_error(conn));
    }

    return ret;
}

/**
 * @return -1 on fail, insert id otherwise
 */
unsigned long __mysql_execute(void *conn, MYSQL_STMT *stmt, MYSQL_BIND *param) {

    if (mysql_stmt_bind_param(stmt, param) != 0) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    return mysql_stmt_insert_id(stmt);
}


#endif //CUTILS_MYSQL_UTIL