#include "pathie.hpp"

std::string Pathie::version()
{
  return std::to_string(PATHIE_VERSION_MAJOR) + "." + std::to_string(PATHIE_VERSION_MINOR) + "." + std::to_string(PATHIE_VERSION_PATCH);
}

std::string Pathie::gitrevision()
{
#ifdef PATHIE_VERSION_GIT
    return PATHIE_VERSION_GIT;
#else
    return "";
#endif
}
