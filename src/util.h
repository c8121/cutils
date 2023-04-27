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

#ifndef CUTILS
#define CUTILS

#include <stdlib.h>

/**
 * Free pointer if not null
 */
void freenn(void *p) {
    if (p != NULL)
        free(p);
}

/**
 * Print message to stderr and exit with given status
 */
void fail(int status, const char *message) {
    fprintf(stderr, "%s\n", message);
    exit(status);
}

#endif //CUTILS