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
  if (r->result != -1)
    {
      getInstance(r)->callFileClosed();
    }
  else
    {
      fprintf(stderr, "Error at closing file: %s.\n", uv_strerror((int)r->result));
    }
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
    }
  else
    {
      getInstance(r)->callFileReadyRead(r->result);
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
  addInstance(uv_handle, this);
}

File::File(const std::string &path, Loop *l)
  : FileUtils(l)
  , path(path)
{
  addInstance(uv_handle, this);
}

File::~File()
{
  uv_fs_req_cleanup(uv_handle);
}

template<typename T>
void File::bindCb(CallbackType &t, T &cb)
{
  switch (t) {
    case FileOpened:
      file_opened_cb = (FileOpenedCb) cb;
      break;
    case FileClosed:
      file_closed_cb = (FileClosedCb) cb;
      break;
    case FileReadyRead:
      file_ready_read_cb = (FileReadyReadCb) cb;
      break;
    case FileWritten:
      file_written_cb = (FileWrittenCb) cb;
      break;
    default:
      break;
    }
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

Buffer *File::getBuffer()
{
  return buffer;
}

bool File::callFileOpened()
{
  file_descriptor = uv_handle->result;
  uv_fs_req_cleanup(uv_handle);
  if (file_opened_cb)
    {
      file_opened_cb();
      return true;
    }
  return false;
}

bool File::callFileClosed()
{
  file_descriptor = 0;
  uv_fs_req_cleanup(uv_handle);
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

bool File::callFileEnd(const ssize_t &len)
{

}


