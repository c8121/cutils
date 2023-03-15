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

#ifndef CUTILS_SORT_UTIL
#define CUTILS_SORT_UTIL

#include <stdio.h>

/**
 * Return new start item of chain
 */
void *linked_list_sort(void *start,
                       void *(*get_next_function)(void *item),
                       int (*compare_function)(void *a, void *b),
                       void (*move_after_function)(void *move, void *after)) {

    if (start == NULL) {
        return start;
    }

    struct linked_item *last;
    struct linked_item *curr;

    int changed = 0;
    do {
        changed = 0;
        last = NULL;
        curr = start;

        while (curr != NULL) {

            if (last != NULL) {

                int comp = (*compare_function)(last, curr);

                if (comp > 0) {
                    if (last == start) {
                        start = curr;
                    }
                    (*move_after_function)(last, curr);
                    if (get_next_function(curr) != last) {
                        fprintf(stderr, "linked_list_sort: move failed\n");
                        return NULL;
                    }

                    changed++;
                }
            }
            last = curr;
            curr = (*get_next_function)(curr);
        }

    } while (changed > 0);

    return start;
}

#endif //CUTILS_SORT_UTIL