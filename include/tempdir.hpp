#ifndef PATHIE_TEMPDIR_HPP
#define PATHIE_TEMPDIR_HPP
#include "path.hpp"

namespace Pathie {

  /**
   * A class for working with temporary directories. Instanciating this class
   * creates a temporary directory in the system's temporary directory path,
   * which you can retrieve by calling path() after the constructor has
   * completed. When the instance is deleted, the temporary directory is
   * removed recursively in the destructor of this class. If you want to keep
   * the temporary directory for whatever reason, call keep() before the object
   * is destroyed.
   *
   * This class relies on rand() when generating the temporary
   * directory name.  Therefore, it is recommended to initialise the
   * random number generator before creating instances of this class
   * by calling the `srand()` function.
   *
   * In a multithreaded environment, this class generates conflicting
   * directory names if the C random number generator is in the same state
   * in two threads and an instance of Tempdir is constructed in these two
   * threads in the very same second. Other than that, Tempdir should be
   * threadsafe.
   */
  class Tempdir
  {
  public:
    Tempdir(std::string namepart);
    ~Tempdir();

    void remove() const;
    void keep(bool k = true);

    Path path() const;
    bool is_kept() const;
  private:
    bool m_keep;
    Path m_path;
  };

}

#endif /* PATHIE_TEMPDIR_HPP */
