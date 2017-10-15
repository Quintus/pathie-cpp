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
 * findexample -- List all files in a directory recursively on standard output.
 *
 * Compile with:
 *   g++ -std=c++11 -I/path/to/pathie/include -L/path/to/pathie/lib findexample.cpp -static -lpathie -o findexample
 */
#include <iostream>
#include <pathie/path.hpp>

int main(int argc, char* argv[])
{
  if (argc == 1) {
    std::cout << "USAGE: " << std::endl << "findexample DIRECTORY" << std::endl;
    return 0;
  }

  Pathie::Path dir(argv[1]);
  dir.find([](const Pathie::Path& entry) -> bool {
    std::cout << entry << std::endl;
    return true;
  });

  return 0;
}
