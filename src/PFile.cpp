#include "PFile.h"

using namespace Parsley;

FileUtils::FileUtils(Loop *l)
  : PObject(l)
{
}

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
  regInstance(m_uv_handle, this);
}

File::File(const std::string &path, Loop *l)
  : FileUtils(l)
  , m_name(path)
{
  regInstance(m_uv_handle, this);
}

File::~File()
{
  uv_fs_req_cleanup(m_uv_handle);
}

int File::open(const int &flags, const int &mode, const Mode &syncMode)
{
  int r = m_fd = uv_fs_open(syncMode == Mode::AsyncMode ? m_loop->uvHandle() : nullptr
                    , m_uv_handle
                    , m_name.data()
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
  this->m_name = path;
  return open(flags, mode, syncMode);
}

int File::close(const Mode &syncMode)
{
  int r = uv_fs_close(m_loop->uvHandle()
                     , m_uv_handle
                     , m_uv_handle->result
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
  if(m_buffer)
    delete m_buffer;
  m_buffer = new Buffer(m_buffer_data, sizeof(m_buffer_data), m_loop);
  int r;

  while (true)
    {
      r = uv_fs_read(m_loop->uvHandle()
                     , m_uv_handle
                     , m_fd
                     , m_buffer->getUvHandle()
                     , 1
                     , contents.length()  // offset
                     , nullptr);
      uv_fs_req_cleanup(m_uv_handle);

      if (r <= 0)
        break;
      contents.append(m_buffer->getUvHandle()->base, r);
    }
  return contents;
}

int File::read(Buffer *buf, const Mode &syncMode)
{
  m_buffer = buf;
  return uv_fs_read(m_loop->uvHandle()
                    , m_uv_handle
                    , m_fd
                    , m_buffer->getUvHandle()
                    , m_buffer->getUvHandle()->len
                    , -1
                    , syncMode == Mode::AsyncMode ? readCb : nullptr);
}

int File::write(Buffer *buf, const Mode &syncMode)
{
  return uv_fs_write(m_loop->uvHandle()
                     , m_uv_handle
                     , m_fd
                     , buf->getUvHandle()
                     , buf->getUvHandle()->len
                     , -1
                     , syncMode == Mode::AsyncMode ? writtenCb : nullptr);
}

int File::write(std::string &data, const Mode &syncMode)
{
  //! This is the way Node.js source code does.
  uv_buf_t buf = uv_buf_init(const_cast<char*>(data.c_str()), data.length());
  return uv_fs_write(m_loop->uvHandle()
                     , m_uv_handle
                     , m_fd
                     , &buf
                     , 1
                     , -1
                     , syncMode == Mode::AsyncMode ? writtenCb : nullptr);
}

int File::truncate(const int &size, const Mode &syncMode)
{
  return uv_fs_ftruncate(m_loop->uvHandle()
                     , m_uv_handle
                     , m_uv_handle->result
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
  return m_buffer;
}

void File::setFileDescriptor(const ssize_t &fd)
{
  m_fd = fd;
}
