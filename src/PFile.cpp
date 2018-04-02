#include "PFile.h"

using namespace Parsley;

void FileUtils::openedCb(uv_fs_t *r)
{
  File *f = getInstance(r);
  if (r->result >= 0)
    {
      f->callFileOpened();
    }
  else
    {
      fprintf(stderr, "error opening file: %s\n", uv_strerror((int)r->result));
    }
}

void FileUtils::closedCb(uv_fs_t *r)
{
  if (r->result == -1)
    {
      fprintf(stderr, "Error at closing file: %s.\n", uv_strerror((int)r->result));
    }
  uv_fs_req_cleanup(r);
}

void FileUtils::readCb(uv_fs_t *r)
{
  if (r->result < 0)
    {
      fprintf(stderr, "Read error!");
    }
  else if (r->result == 0)
    {
      uv_fs_req_cleanup(r);
      uv_fs_t closeReq;
      uv_fs_close(r->loop
                  , &closeReq
                  , r->result
                  , NULL);
    }
  else
    {
      getInstance(r)->callFileReadyRead(r->result);
    }
}

File::File(Loop *l)
  : FileUtils(l)
  , loop(l)
{
  regInstance(uv_handle, this);
}

File::File(char *path, Loop *l)
  : FileUtils(l)
  , loop(l)
  , path(path)
{
  regInstance(uv_handle, this);
}

File::~File()
{
  uv_fs_req_cleanup(uv_handle);
}

int File::open(const int &flags, const int &mode, const Mode &syncMode)
{
  return uv_fs_open(loop->uvHandle()
                    , uv_handle
                    , path
                    , flags
                    , mode
                    , syncMode == Mode::Async ? openedCb : NULL);
}

int File::open(char *path, const int &flags, const int &mode, const Mode &syncMode)
{
  this->path = path;
  return open(flags, mode, syncMode);
}

int File::close(const Mode &syncMode)
{
  return uv_fs_close(loop->uvHandle()
                     , uv_handle
                     , uv_handle->result
                     , syncMode == Mode::Async ? closedCb : NULL);
}

int File::read(Buffer *buf, const Mode &syncMode)
{
  if(buffer)
    {
      if(buffer->base)
        {
          free(buffer->base);
        }
      free(buffer); //! be careful
    }
  buffer = buf;
  return uv_fs_read(loop->uvHandle()
                    , uv_handle
                    , uv_handle->result
                    , buffer
                    , buffer->len
                    , -1
                    , syncMode == Mode::Async ? readCb : NULL);
}

int File::mkdir(char *dir, const int &mode, Loop *l, const Mode &syncMode)
{
  uv_fs_t r;
  int ret = uv_fs_mkdir(l->uvHandle()
              , &r
              , dir
              , mode
              , /*syncMode == Mode::Async ? NULL : NULL*/NULL); //! Add Aync Callback!
  uv_fs_req_cleanup(&r);
  return ret;
}

Loop *File::getLoop()
{
  return loop;
}

Buffer *File::getBuffer()
{
  return buffer;
}

bool File::callFileOpened()
{
  if (file_opened_cb)
    {
      file_opened_cb();
      return true;
    }
  return false;
}

bool File::callFileClosed()
{
  if (file_closed_cb)
    {
      file_closed_cb();
      return true;
    }
  return false;
}

bool File::callFileReadyRead(const ssize_t &len)
{
  if (file_ready_read_cb)
    {
      file_ready_read_cb(buffer, len);
      return true;
    }
  return false;
}


