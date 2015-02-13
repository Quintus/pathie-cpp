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

#ifndef PATHIE_TESTHELPERS_HPP
#define PATHIE_TESTHELPERS_HPP
#include <iostream>
#include <functional>
#include <typeinfo>
#include <map>
#include <cstring>

#define EQUAL(ex, real) assert_equal(ex, real, __FILE__, __LINE__)
#define IS_TRUE(real) assert_true(real, __FILE__, __LINE__)
#define IS_FALSE(real) assert_false(real, __FILE__, __LINE__)
#define SKIP() skip(__FILE__, __LINE__)

// Use ANSI colors so we can spot failures more easily. Windows
// does not support ANSI colors.
#ifdef _PATHIE_UNIX
#define FAIL "\e[31;1mFAIL\e[0m"
#define OK "\e[32mOK\e[0m"
#else
#define FAIL "FAIL"
#define OK "OK"
#endif

inline void assert_equal(std::string expected, std::string real, std::string file, int line)
{
  std::cout << "Expecting '" << expected << "'... ";
  if (expected == real)
    std::cout << OK;
  else
    std::cout << FAIL << std::endl
	      << "\t"
	      << file << ":" << line << ": Got '" << real << "'";

  std::cout << std::endl;
}

inline void assert_equal(std::wstring expected, std::wstring real, std::string file, int line)
{
  std::cout << "Expecting '";
  std::wcout << expected;
  std::cout << "'... ";
  if (expected == real)
    std::cout << OK;
  else {
    std::cout << FAIL << std::endl;
    std::cout << "\t";
    std::cout << file << ":" << line << ": Got: '";
    std::wcout << real;
  }

  std::cout << std::endl;
}

inline void assert_equal(std::string expected, Pathie::Path real, std::string file, int line)
{
  assert_equal(expected, real.str(), file, line);
}

inline void assert_equal(long expected, long real, std::string file, int line)
{
  std::cout << "Expecting '" << expected << "'... ";
  if (expected == real)
    std::cout << OK;
  else
    std::cout << FAIL << std::endl
	      << "\t"
	      << file << ":" << line << ": Got '" << real << "'";

  std::cout << std::endl;
}

inline void assert_true(bool value, std::string file, int line)
{
  std::cout << "Expecting true... ";
  if (value)
    std::cout << OK;
  else {
    std::cout << FAIL << std::endl
	      << "\t"
	      << file << ":" << line << ": Got false.";
  }

  std::cout << std::endl;
}

inline void assert_false(bool value, std::string file, int line)
{
  std::cout << "Expecting false... ";
  if (!value)
    std::cout << OK;
  else {
    std::cout << FAIL << std::endl
	      << "\t"
	      << file << ":" << line << ": Got true.";
  }

  std::cout << std::endl;
}

// C++11 lambdas are soooo cool!
template<typename T>
void assert_exception(T* exceptionclass, std::string file, int line, std::function<void ()> func)
{
  std::cout << "Expecting exception '" << typeid(T).name() << "... ";

  // Note we only have the exceptionclass argument so we don’t
  // have to use a macro to get the type. C++ does not allow
  // us to treat types as if they were objects...
  delete exceptionclass;

  try {
    func();
  }
  catch(T& err) {
    std::cout << OK << std::endl;
    return;
  }
  catch(std::exception& err) {
    std::cout << FAIL << std::endl
	      << "\t"
	      << file << ":" << line << ": "
	      << "Got: " << err.what() << "(" << typeid(err).name() << ")" << std::endl;
    return;
  }

  std::cout << FAIL << std::endl
	    << "\t"
	    << file << ":" << line << ": " << "Nothing thrown." << std::endl;
}

void skip(std::string file, int line)
{
#ifdef _PATHIE_UNIX
  std::cout << "\e[36m" << "Skipping test at " << file << ":" << line << "\e[0m" << std::endl;
#else
  std::cout << "Skipping test at " << file << ":" << line << std::endl;
#endif
}

std::map<std::string, std::string> get_testsettings()
{
  std::map<std::string, std::string> result;
  std::ifstream file("testsettings.conf");
  char buf[4096];
  std::string key;
  std::string value;

  if (!file.is_open())
    throw(std::runtime_error("Failed to access testsettings.conf!"));

  while (!file.eof()) {
    memset(buf, '\0', 4096);
    file.getline(buf, 4096);

    // Ignore comments and empty lines
    if (strlen(buf) == 0)
      continue;
    if (buf[0] == '#')
      continue;

    // Exctract key and value
    std::string str(buf);
    size_t separator_pos = str.find("=");
    key = str.substr(0, separator_pos);
    value = str.substr(separator_pos + 1, std::string::npos); // Exclude "=" itself

    result[key] = value;
  }

  file.close();

  std::map<std::string, std::string>::const_iterator iter;
  std::cout << "+++ Parsed test settings: +++" << std::endl;
  for(iter=result.begin(); iter != result.end(); iter++) {
    std::cout << iter->first << " => " << iter->second << std::endl;
  }
  std::cout << "+++ End +++" << std::endl;

  return result;
}

#endif
