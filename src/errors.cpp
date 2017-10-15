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

#include "../include/errors.hpp"

#include <cerrno>
#include <cstring>
#include <sstream>

#if defined(_WIN32)
#include <windows.h>
#elif defined(_PATHIE_UNIX)
#include <glob.h>
#endif

using namespace Pathie;

PathieError::PathieError()
{
  m_pathie_errmsg = "Unknown pathie exception.";
}

PathieError::PathieError(std::string message)
{
  m_pathie_errmsg = message;
}

PathieError::~PathieError() throw()
{
  //
}

const char* PathieError::what() const throw()
{
  return m_pathie_errmsg.c_str();
}

ErrnoError::ErrnoError(int val)
{
  std::stringstream ss;
  ss << val;

  m_val = val;
  m_pathie_errmsg = "Errno " + ss.str() + ": " + strerror(val);
}

ErrnoError::~ErrnoError() throw()
{
  //
}

#ifdef _WIN32
WindowsError::WindowsError(DWORD val)
{
  std::stringstream ss;
  ss << val;

  wchar_t* buf = NULL;
  FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                 NULL,
                 val,
                 LANG_USER_DEFAULT,
                 (wchar_t*) &buf, // What a weird API.
                 0,
                 NULL);

  m_val = val;
  m_pathie_errmsg = std::string("Windows Error Code ") + ss.str() + ": " + utf16_to_utf8(buf);

  LocalFree(buf);
}

WindowsError::~WindowsError() throw()
{
  //
}

WindowsHresultError::WindowsHresultError(HRESULT val)
{
  std::stringstream ss;
  ss << val;

  m_val = val;
  m_pathie_errmsg = std::string("Windows HRESULT Error Code :") + ss.str();
}

WindowsHresultError::~WindowsHresultError() throw()
{
  //
}

#endif

#ifdef _PATHIE_UNIX
GlobError::GlobError(int val)
{
  std::stringstream ss;
  ss << val;

  m_val = val;

  m_pathie_errmsg = "Glob error code " + ss.str() + ": ";

  switch(val) {
  case GLOB_NOSPACE:
    m_pathie_errmsg += "GLOB_NOSPACE";
    break;
  case GLOB_ABORTED:
    m_pathie_errmsg += "GLOB_ABORTED";
    break;
  case GLOB_NOMATCH:
    m_pathie_errmsg += "GLOB_NOMATCH";
    break;
  default:
    m_pathie_errmsg += "Unknown glob error";
    break;
  }
}

GlobError::~GlobError() throw()
{
  //
}
#endif
