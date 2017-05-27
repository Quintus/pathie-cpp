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

#ifndef PATHIE_PATHIE_HPP
#define PATHIE_PATHIE_HPP
#if __cplusplus < 201103L
#error Pathie requires C++11 support. Please use an option such as -std=c++11 to enable it.
#endif

#include <string>

#include "errors.hpp"
#include "path.hpp"
#include "pathie_ofstream.hpp"
#include "pathie_ifstream.hpp"

/// Namespace for this library.
namespace Pathie {

  /// Returns the version number is MAJOR.MINOR.TINY.
  inline std::string version()
  {
    return std::to_string(PATHIE_VERSION_MAJOR) + "." + std::to_string(PATHIE_VERSION_MINOR) + "." + std::to_string(PATHIE_VERSION_PATCH);
  }

  /**
   * Returns the Git commit this was build from.
   * Empty string if build without Git.
   */
  inline std::string gitrevision()
  {
#ifdef PATHIE_VERSION_GIT
    return PATHIE_VERSION_GIT;
#else
    return "";
#endif
  }

#ifdef _WIN32
  std::string utf16_to_utf8(std::wstring);
  std::wstring utf8_to_utf16(std::string);
#endif

#ifdef _PATHIE_UNIX
  std::string utf8_to_filename(const std::string& utf8);
  std::string filename_to_utf8(const std::string& native_filename);
  std::string convert_encodings(const char* from_encoding, const char* to_encoding, const std::string& string);
#endif

}

#endif
