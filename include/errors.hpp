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

#ifndef PATHIE_ERRORS_HPP
#define PATHIE_ERRORS_HPP
#include <exception>
#include <string>
#include <cstdlib>

#ifdef _WIN32
// Only ever include <windows.h> after <cstdlib>, which defines __MSVCRT__,
// otherwise you’ll get cryptic "undefined..." error messages on compilation.
#include <windows.h>
#endif

#include "pathie.hpp"

namespace Pathie {

  /// Base class for all exceptions in this library.
  class PathieError: public std::exception {
  public:
    PathieError(); ///< Constructs a new instance.
    PathieError(std::string message); ///< Contructs a new instance with the given what() message.
    virtual ~PathieError() throw();

    virtual const char* what() const throw(); ///< The error message.
  protected:
    std::string m_pathie_errmsg; ///< The error message given in the constructor.
  };


  /// This exception is thrown when a call to a C/system function results
  /// in `errno` being set.
  class ErrnoError: public PathieError {
  public:
    ErrnoError(int val); ///< Constructs a new instance from the given `errno` value.
    virtual ~ErrnoError() throw();

    inline int get_val(){return m_val;} ///< The `errno` value.
  private:
    int m_val;
  };

#ifdef _WIN32

  /// This exception is thrown only on Windows, when a call to the Win32API
  /// fails.
  class WindowsError: public PathieError {
  public:
    WindowsError(DWORD val); ///< Constructs a new instance from the given GetLastError() value.
    virtual ~WindowsError() throw();

    inline int get_val(){return m_val;} ///< The GetLastError() value.
  private:
    DWORD m_val;
  };

  /// Similar to WindowsError, this exception is thrown when a HANDLE function
  /// from the Win32API fails.
  class WindowsHresultError: public PathieError {
  public:
    WindowsHresultError(HRESULT value); ///< Constructs a new instance from the given handle function result.
    virtual ~WindowsHresultError() throw();

    inline HRESULT get_val(){return m_val;} ///< The handle function result.
  private:
    int m_val;
  };
#endif

#ifdef _PATHIE_UNIX

  /// This exception is thrown only on UNIX, when a call to the POSIX glob(3)
  /// function fails.
  class GlobError: public PathieError {
  public:
    GlobError(int val); ///< Contructs a new instance from the given glob(3) error code.
    virtual ~GlobError() throw();

    inline int get_val(){return m_val;} ///< The glob(3) error code.
  private:
    int m_val;
  };
#endif

}
#endif
