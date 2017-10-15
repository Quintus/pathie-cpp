/* -*- coding: utf-8 -*-
 * This file is part of Pathie.
 *
 * Pathie is a path management library.
 * Copyright © 2015 Marvin Gülker
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

/**
 * minils -- List all files in a directory on standard output.
 *
 * Compile with:
 *   g++ -std=c++11 -I/path/to/pathie/include -L/path/to/pathie/lib minils.cpp -static -lpathie -o minils
 */
#include <iostream>
#include <pathie/path.hpp>

int main(int argc, char* argv[])
{
  if (argc == 1) {
    std::cout << "USAGE:" << std::endl << "minils DIRECTORY" << std::endl;
    return 0;
  }

  Pathie::Path dir(argv[1]);
  std::vector<Pathie::Path> entries = dir.entries();
  std::vector<Pathie::Path>::const_iterator iter;

  for(iter = entries.begin(); iter != entries.end(); iter++) {
    std::cout << (*iter) << std::endl;
  }

  return 0;
}
