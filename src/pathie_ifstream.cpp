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

#include "../include/pathie_ifstream.hpp"

namespace Pathie {
#if defined(_PATHIE_UNIX)
  // All well and easy under UNIX. Just delegate to standard constructor.
  Pathie::ifstream::ifstream(Pathie::Path path, std::ios_base::openmode mode)
    : std::ifstream(path.native(), mode)
  {
    //
  }

  Pathie::ifstream::ifstream()
    : std::ifstream()
  {
    //
  }

  Pathie::ifstream::ifstream(std::string path, std::ios_base::openmode mode)
    : std::ifstream(utf8_to_filename(path), mode)
  {
    //
  }

  Pathie::ifstream::ifstream(char* path, std::ios_base::openmode mode)
    : std::ifstream(utf8_to_filename(path), mode)
  {
    //
  }

  void Pathie::ifstream::open(const char* filename, ios_base::openmode mode)
  {
    std::string filename_nstr = utf8_to_filename(filename);
    std::ifstream::open(filename_nstr.c_str(), mode);
  }

  void Pathie::ifstream::open(const std::string& filename, ios_base::openmode mode)
  {
    std::ifstream::open(utf8_to_filename(filename), mode);
  }

  void Pathie::ifstream::open(const Pathie::Path& filename, ios_base::openmode mode)
  {
    std::ifstream::open(filename.native(), mode);
  }


#elif defined (_WIN32)
#  if defined(_MSC_VER)
  // Easy again under MSVC under Windows; using Microsoft’s nonstandard constructor
  // for Unicode filenames.
  // It is documented here: http://msdn.microsoft.com/en-us/library/8et8s826.aspx
  Pathie::ifstream::ifstream(Pathie::Path path, std::ios_base::openmode mode)
    : std::ifstream(path.native(), mode)
  {
    //
  }

  Pathie::ifstream::ifstream()
    : std::ifstream()
  {
    //
  }

  Pathie::ifstream::ifstream(std::string path, std::ios_base::openmode mode)
    : std::ifstream(path, mode)
  {
    //
  }

  Pathie::ifstream::ifstream(char* path, std::ios_base::openmode mode)
    : std::ifstream(path, mode)
  {
    //
  }
#  elif defined(__GNUC__)
  // This one is tough, but solveable. There’s a nonstandard C++ extension by the
  // GCC team to create a C++ stream from a file descriptor and similar.
  // It is documented here: https://gcc.gnu.org/onlinedocs/gcc-4.9.2/libstdc++/api/a00054.html

  /**
   * Default constructor for deferred initialisation via open().
   * Beware that before you called open(), any methods other than
   * is_open() may behave unexpectedly!
   */
  Pathie::ifstream::ifstream()
    : std::basic_istream<char, std::char_traits<char> >()
  {
    mp_file = NULL;
    mp_filebuffer = NULL;
    m_buffer_allocated = false;

    // See the lengthy explanation in open() for why we do this here.
    mp_filebuffer = (__gnu_cxx::stdio_filebuf<char>*) malloc(sizeof(__gnu_cxx::stdio_filebuf<char>));
    memset(mp_filebuffer, '\0', sizeof(__gnu_cxx::stdio_filebuf<char>));

    this->init(mp_filebuffer);
  }

  /**
   * Construct a stream for the given UTF-8 file path.
   *
   * \param[in] filename The path to open the stream for. UTF-8.
   * \param     mode Mode to open the file in.
   */
  Pathie::ifstream::ifstream(const char* filename, ios_base::openmode mode)
    : std::basic_istream<char, std::char_traits<char> >()
  {
    mp_file = NULL;
    mp_filebuffer = NULL;
    m_buffer_allocated = false;

    // See the lengthy explanation in open() for why we do this here.
    mp_filebuffer = (__gnu_cxx::stdio_filebuf<char>*) malloc(sizeof(__gnu_cxx::stdio_filebuf<char>));
    memset(mp_filebuffer, '\0', sizeof(__gnu_cxx::stdio_filebuf<char>));

    this->init(mp_filebuffer);
    this->open(filename, mode);
  }

  /**
   * Construct a stream for the given UTF-8 file path.
   *
   * \param[in] filename The path to open the stream for. UTF-8.
   * \param     mode Mode to open the file in.
   */
  Pathie::ifstream::ifstream(const std::string& filename, ios_base::openmode mode)
    : std::basic_istream<char, std::char_traits<char> >()
  {
    mp_file = NULL;
    mp_filebuffer = NULL;
    m_buffer_allocated = false;

    // See the lengthy explanation in open() for why we do this here.
    mp_filebuffer = (__gnu_cxx::stdio_filebuf<char>*) malloc(sizeof(__gnu_cxx::stdio_filebuf<char>));
    memset(mp_filebuffer, '\0', sizeof(__gnu_cxx::stdio_filebuf<char>));

    this->init(mp_filebuffer);
    this->open(filename, mode);
  }

  /**
   * Construct a stream for the given Pathie::Path instance.
   *
   * \param[in] filename The path to open the stream for. A Pathie::Path instance.
   * \param     mode Mode to open the file in.
   */
  Pathie::ifstream::ifstream(const Pathie::Path& filename, ios_base::openmode mode)
    : std::basic_istream<char, std::char_traits<char> >()
  {
    mp_file = NULL;
    mp_filebuffer = NULL;
    m_buffer_allocated = false;

    // See the lengthy explanation in open() for why we do this here.
    mp_filebuffer = (__gnu_cxx::stdio_filebuf<char>*) malloc(sizeof(__gnu_cxx::stdio_filebuf<char>));
    memset(mp_filebuffer, '\0', sizeof(__gnu_cxx::stdio_filebuf<char>));

    this->init(mp_filebuffer);
    this->open(filename, mode);
  }

  Pathie::ifstream::~ifstream()
  {
    free(mp_filebuffer);
  }

  /**
   * The underlying buffer.
   */
  __gnu_cxx::stdio_filebuf<char>* Pathie::ifstream::rdbuf() const
  {
    return mp_filebuffer;
  }

  /**
   * Checks whether the stream has been open()ed already. This is the only
   * method safe to use before you called open() on a stream constructed
   * with the default constructor (apart from open() itself of course).
   */
  bool Pathie::ifstream::is_open() const
  {
    if (!m_buffer_allocated)
      return false;

    return mp_filebuffer->is_open();
  }

  /**
   * Open the given UTF-8 file path in this stream. You can call this anytime
   * after you constructed an instance with the default constructor; otherwise,
   * you have to close() whatever was opened before you call this method.
   *
   * \param[in] filename UTF-8 filename to open
   * \param     mode Mode to open the stream in.
   */
  void Pathie::ifstream::open(const char* filename, ios_base::openmode mode)
  {
    std::wstring w_filename = Pathie::utf8_to_utf16(filename);

    mp_file = _wfopen(w_filename.c_str(), L"r"); // Mode will be overridden
    if (!mp_file) {
      setstate(ios_base::failbit);
      return;
    }

    /* The following construction uses a “placement new” as it appears
     * to be the only "clean" solution applicable. The init() method,
     * an internum of the GCC implementation of basic_istream that
     * needs to be called in the stream’s constructor, requires a
     * pointer to the filebuffer object. However, we do not have that
     * filebuffer object at hand in the constructor, the
     * __gnu_cxx::stdio_filebuf instance will be created later when
     * open() is called. It is impossible to construct it earlier,
     * because it does not support a delayed open() call, the file
     * descriptor or FILE* pointer must be passed during its
     * construction, but we don’t have it there; it is available only
     * in open() -- remember that you can create the ifstream instance
     * without being attached to a file and then call open() later
     * with a filename. To be able to pass something meaningful to
     * init(), we have to "foresee" where in memory the stdio_filebuf
     * instance will be created. This only is possible with a
     * placement new into a place we have allocated previously using
     * malloc().
     *
     * An alternative would be to use internal GCC APIs by duplicating
     * the sourcecode of the __gnu_cxx::stdio_filebuf constructor; however
     * undocumented internal APIs are never good to use. For informational
     * purposes therefore the sourcecode link:
     *
     *   https://gcc.gnu.org/onlinedocs/gcc-4.9.2/libstdc++/api/a01222_source.html
     */

    new (mp_filebuffer) __gnu_cxx::stdio_filebuf<char>(mp_file, mode);
    m_buffer_allocated = true;

    if (!mp_filebuffer->is_open())
      setstate(ios_base::failbit);
    else
      clear();
  }

  /**
   * Open the given UTF-8 file path in this stream. You can call this anytime
   * after you constructed an instance with the default constructor; otherwise,
   * you have to close() whatever was opened before you call this method.
   *
   * \param[in] filename UTF-8 filename to open
   * \param     mode Mode to open the stream in.
   */
  void Pathie::ifstream::open(const std::string& filename, ios_base::openmode mode)
  {
    open(filename.c_str(), mode);
  }

  /**
   * Open the given Pathie::Path in this stream. You can call this anytime
   * after you constructed an instance with the default constructor; otherwise,
   * you have to close() whatever was opened before you call this method.
   *
   * \param[in] filename Pathie::Path to open the stream for.
   * \param     mode Mode to open the stream in.
   */
  void Pathie::ifstream::open(const Pathie::Path& filename, ios_base::openmode mode)
  {
    open(filename.str(), mode);
  }

  /**
   * Close the underlying file. Has no effect if no file is opened.
   */
  void Pathie::ifstream::close()
  {
    if (mp_file) {
      if (!mp_filebuffer->close())
        setstate(ios_base::failbit);

      // Do not deallocate, we may need it later if an open() call follows.
      memset(mp_filebuffer, '\0', sizeof(__gnu_cxx::stdio_filebuf<char>));
      m_buffer_allocated = false;
      fclose(mp_file);
    }
  }
#  else
#    error Unsupported compiler: do not know how to open C++ stream on Unicode file.
#  endif
#else
#  error Unsupported system.
#endif
};
