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
#include "../include/pathie.hpp"
#include "testhelpers.hpp"

using namespace Pathie;

void test_transcode()
{
  std::string utf8str     = "\x42\xc3\xa4\x72\x20\x4d\xc3\xb6\x68\x72\x65\x20\x53\x6f\xc3\x9f\x65"; // UTF-8: Bär Möhre Soße
  std::string iso88591str = "\x42\xe4\x72\x20\x4d\xf6\x68\x72\x65\x20\x53\x6f\xdf\x65"; // ISO-8859-1: Bär Möhre Soße

  std::string result1 = convert_encodings("ISO-8859-1", "UTF-8", iso88591str);
  EQUAL(utf8str, result1);

  std::string result2 = convert_encodings("UTF-8", "ISO-8859-1", utf8str);
  EQUAL(iso88591str, result2);

  // No way to specify what iconv() shall do with characters that are
  // not available in the target encoding. POSIX.1-2008 just says the
  // iconv() function "shall perform an implementation-defined
  // conversion on this character." So, what to test here?

  // const char* unicodestr = "\xe2\x9c\x93"; // UTF-8: ✓
}

int main(int argc, char* argv[])
{
  std::locale::global(std::locale(""));
  test_transcode();
}
