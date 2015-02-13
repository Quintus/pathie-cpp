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

#include <cstdlib>
#include <cstring>
#include <locale>
#include <pathie.hpp>
#include "testhelpers.hpp"

#if defined(_PATHIE_UNIX)
#include <unistd.h>
#include <limits.h>
#elif defined(_WIN32)
#include <windows.h>
#else
#error Unsupported system.
#endif

using namespace Pathie;

void test_ofstream()
{
  Path p1("foo");

  p1.mkdir();
  Path p2("foo/bar.txt");

  Pathie::ofstream file(p2, Pathie::ofstream::out);
  IS_TRUE(file.is_open());
  IS_TRUE(file.good());

  file << "Test stuff" << std::endl;
  file.close();

  IS_TRUE(p2.exists());

  char buf[256];
  memset(buf, '\0', 256);
  FILE* p_file = p2.fopen("r");
  fread(buf, 1, 256, p_file);
  fclose(p_file);

  EQUAL("Test stuff\n", buf);

  Path p3("foo/bär blaß.txt");
  Pathie::ofstream unicode_file(p3, Pathie::ofstream::out);
  IS_TRUE(unicode_file.is_open());
  IS_TRUE(unicode_file.good());

  unicode_file << "Bärenstark" << std::endl;
  unicode_file.close();

  IS_TRUE(p3.exists());

  memset(buf, '\0', 256);
  p_file = p3.fopen("r");
  fread(buf, 1, 256, p_file);
  fclose(p_file);

  EQUAL("Bärenstark\n", buf);

  Pathie::ofstream stream;
  IS_FALSE(stream.is_open());
  stream.open("foo/foobarbaz.txt");
  IS_TRUE(stream.is_open());
  stream.close();
  IS_FALSE(stream.is_open());
}

void test_ifstream()
{
  Path p1("testfile.txt");
  Path p2("tästfile.txt");

  Pathie::ifstream file(p1);
  IS_TRUE(file.is_open());
  IS_TRUE(file.good());

  char buf[256];
  memset(buf, '\0', 256);
  file.read(buf, 256);
  EQUAL("There is some testtext\nin this file.\n", buf);

  Pathie::ifstream unicode_file(p2);
  IS_TRUE(unicode_file.is_open());
  IS_TRUE(unicode_file.good());

  memset(buf, '\0', 256);
  unicode_file.read(buf, 256);
  EQUAL("Thäre is ßöme testtext\nin this file.\n", buf);

  Pathie::ifstream stream;
  IS_FALSE(stream.is_open());
  stream.open("testfile.txt");
  IS_TRUE(stream.is_open());
  stream.close();
  IS_FALSE(stream.is_open());
}

int main(int argc, char* argv[])
{
  std::locale::global(std::locale(""));
  std::cout << "Cleaning." << std::endl;
  system("rake clean");

  test_ofstream();
  test_ifstream();
}
