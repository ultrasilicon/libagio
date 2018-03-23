#include "PFile.h"

using namespace Parsley;

void FileUtils::openCb(uv_fs_t *r)
{
  File *f = getInstance(r);
  if (r->result >= 0)
    {

//      Buffer iov = uv_buf_init(buffer, sizeof(Buffer));
//      uv_fs_read(f->loop->uvHandle(), &r, r->result, &iov, 1, -1, on_read);
    }
  else
    {
      fprintf(stderr, "error opening file: %s\n", uv_strerror((int)r->result));
    }
}

File::File(Loop *l)
{
  loop = l;
  file_handle = (uv_fs_t*) malloc(sizeof(uv_fs_t));
  regInstance(file_handle, this);
}

File::File(char *path, Loop *l)
  : loop(l)
  , path(path)
{

}

int File::open(const int &flags, const int &mode)
{
  return uv_fs_open(loop->uvHandle(), file_handle, path, flags, mode, openCb);
}

int File::open(char *path, const int &flags, const int &mode)
{
  this->path = path;
  return open(flags, mode);
}

Loop *File::getLoop()
{
  return loop;
}


