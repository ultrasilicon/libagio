#include "PFile.h"

using namespace Parsley;

void FileUtils::openedCb(uv_fs_t *r)
{
  File *f = getInstance(r);
  if (r->result >= 0)
    {
      f->setFileDescriptor(r->result);
      uv_fs_req_cleanup(r);

      f->onOpened.call();
    }
  else
    {
      fprintf(stderr, "error opening file: %s\n", uv_strerror((int)r->result));
    }
}

void FileUtils::closedCb(uv_fs_t *r)
{
  File *f = getInstance(r);
  if (r->result != -1)
    {
      f->setFileDescriptor(0);
      uv_fs_req_cleanup(r);
      f->onClosed.call();
    }
  else
    {
      fprintf(stderr, "Error at closing file: %s.\n", uv_strerror((int)r->result));
    }
}

void FileUtils::readCb(uv_fs_t *r)
{
  File *f = getInstance(r);

  if (r->result < 0)
    {
      fprintf(stderr, "Read error!");
    }
  else if (r->result == 0)
    {
      uv_fs_req_cleanup(r);
    }
  else
    {
      getInstance(r)->onReadyRead.call(f->getBuffer(), r->result);
    }
  uv_fs_req_cleanup(r);
}

void FileUtils::writtenCb(uv_fs_t *r)
{
  if (r->result == -1) {
      fprintf(stderr, "Error writting data to file: s.\n");
    }
  std::cout<<"written";

  uv_fs_t closeReq;
  uv_fs_close(r->loop
              , &closeReq
              , r->result
              , nullptr);
  uv_fs_req_cleanup(r);
  uv_fs_req_cleanup(&closeReq);
}




File::File(Loop *l)
  : FileUtils(l)
{
  regInstance(uv_handle, this);
}

File::File(const std::string &path, Loop *l)
  : FileUtils(l)
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
  int r = file_descriptor = uv_fs_open(syncMode == Mode::AsyncMode ? loop->uvHandle() : nullptr
                    , uv_handle
                    , path.data()
                    , flags
                    , mode
                    , syncMode == Mode::AsyncMode ? openedCb : nullptr);

  if(syncMode == Mode::SyncMode)
    {
//      callFileOpened();
    }

  return r;
}

int File::open(char *path, const int &flags, const int &mode, const Mode &syncMode)
{
  this->path = path;
  return open(flags, mode, syncMode);
}

int File::close(const Mode &syncMode)
{
  int r = uv_fs_close(loop->uvHandle()
                     , uv_handle
                     , uv_handle->result
                     , syncMode == Mode::AsyncMode ? closedCb : nullptr);

  if(syncMode == Mode::SyncMode)
    {
//      callFileClosed();
    }

  return r;
}

std::string File::readAll()
{
  //! This is the way Node.js source code does.
  std::string contents;
  buffer = (Buffer *)malloc(sizeof(Buffer));
  *buffer = uv_buf_init(buffer_memory, sizeof(buffer_memory));
  int r;

  while (true)
    {
      r = uv_fs_read(loop->uvHandle()
                     , uv_handle
                     , file_descriptor
                     , buffer
                     , 1
                     , contents.length()  // offset
                     , nullptr);
      uv_fs_req_cleanup(uv_handle);

      if (r <= 0)
        break;
      contents.append(buffer->base, r);
    }
  return contents;
}

int File::read(Buffer *buf, const Mode &syncMode)
{
  buffer = buf;
  return uv_fs_read(loop->uvHandle()
                    , uv_handle
                    , file_descriptor
                    , buffer
                    , buffer->len
                    , -1
                    , syncMode == Mode::AsyncMode ? readCb : nullptr);
}

int File::write(Buffer *buf, const Mode &syncMode)
{
  return uv_fs_write(loop->uvHandle()
                     , uv_handle
                     , file_descriptor
                     , buf
                     , buf->len
                     , -1
                     , syncMode == Mode::AsyncMode ? writtenCb : nullptr);
}

int File::write(std::string &data, const Mode &syncMode)
{
  //! This is the way Node.js source code does.
  uv_buf_t buf = uv_buf_init(const_cast<char*>(data.c_str()), data.length());
  return uv_fs_write(loop->uvHandle()
                     , uv_handle
                     , file_descriptor
                     , &buf
                     , 1
                     , -1
                     , syncMode == Mode::AsyncMode ? writtenCb : nullptr);
}

int File::truncate(const int &size, const Mode &syncMode)
{
  return uv_fs_ftruncate(loop->uvHandle()
                     , uv_handle
                     , uv_handle->result
                     , size
                     , nullptr); //! Add Aync Callback!
}

int File::mkdir(const std::string &dir, const int &mode, Loop *l, const Mode &syncMode)
{
  uv_fs_t r;
  int ret = uv_fs_mkdir(l->uvHandle()
              , &r
              , dir.data()
              , mode
              , /*syncMode == Mode::Async ? nullptr : nullptr*/nullptr); //! Add Aync Callback!
  uv_fs_req_cleanup(&r);
  return ret;
}

int File::remove(const std::string &file, Loop *l)
{
  uv_fs_t r;
  int ret = uv_fs_unlink(l->uvHandle()
              , &r
              , file.c_str()
              , nullptr); //! Add Aync Callback!
  uv_fs_req_cleanup(&r);
  return ret;
}

Buffer *File::getBuffer()
{
  return buffer;
}

void File::setFileDescriptor(const ssize_t &fd)
{
  file_descriptor = fd;
}
