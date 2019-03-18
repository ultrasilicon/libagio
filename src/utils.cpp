#include "utils.h"

using namespace Parsley;

int Utils::getFd(uv_handle_t* handle)
{
  int fd;
#ifndef Q_OS_WIN
  uv_fileno(handle, &fd);
#else
  uv_fileno(handle, (uv_os_fd_t*)(&fd));
#endif
  return fd;
}

