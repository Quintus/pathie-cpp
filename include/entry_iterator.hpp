/* -*- coding: utf-8 -*-
 * This file is part of Pathie.
 *
 * Copyright © 2015, 2017 Marvin Gülker
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PATHIE_ENTRY_ITERATOR_HPP
#define PATHIE_ENTRY_ITERATOR_HPP
#include <iterator>

namespace Pathie {

  class Path;

  /**
   * An iterator class for reading the entries in a directory.
   * Note that the entries of a directory always include the
   * "." (current directory) and ".." (parent directory) entries
   * unresolved, and that the order in which the entries in the
   * directory are returned is undefined (actually, the order
   * depends on the filesystem used).
   *
   * The iterators of this class are always const. You cannot
   * change the values referenced.
   *
   * Instances of this class cannot be copied, because they
   * wrap a native handle, e.g. a file descriptor on Linux.
   */
  class entry_iterator: public std::iterator<std::input_iterator_tag, Path, int>
  {
  public:
    entry_iterator();
    entry_iterator(Path* p_top);
    ~entry_iterator();
    entry_iterator& operator=(Path* p_top); // Restart assignment
    operator bool() const;
    bool operator==(const entry_iterator& other) const;
    bool operator!=(const entry_iterator& other) const;
    entry_iterator& operator++(int);
    entry_iterator& operator++();
    const Path& operator*() const;
    const Path* operator->() const;
  private:
    void open_native_handle();
    void close_native_handle();

    Path* mp_directory; ///< Path requested to read from.
    void* mp_cur; ///< Native handle to the opened directory.
    Path* mp_cur_path; ///< Path instance of the path pointed to by mp_cur (only a pointer to allow forward-declaration of Path).
  };
}

#endif /* PATHIE_ENTRY_ITERATOR_HPP */
