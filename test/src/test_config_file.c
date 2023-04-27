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

#include <stdio.h>


#include "../../src/config_file.h"

/**
 * 
 */
int main(int argc, char *argv[]) {

    printf("CONFIG FILE: '%s'\n", get_config_file_path(argc, argv, "../../config/default-config"));
    //printf("CONFIG FILE: '%s'\n", get_config_file_path(argc, argv, "../config/default-config"));
    //printf("CONFIG FILE: '%s'\n", get_config_file_path(argc, argv, "../../config/not-exists"));

}
