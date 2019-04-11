#include "file.h"

using namespace Parsley;

FileUtils::FileUtils(Loop *l)
  : PUvObject(l)
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
  regInstance(m_uv_obj, this);
}

File::File(const std::string &path, Loop *l)
  : FileUtils(l)
  , name_(path)
{
  regInstance(m_uv_obj, this);
}

File::~File()
{
  uv_fs_req_cleanup(m_uv_obj);
}

int File::open(const int &flags, const int &mode, const Mode &m)
{
  int r = fd_ = uv_fs_open(m == Mode::Async ? m_loop->uvHandle() : nullptr
                    , m_uv_obj
                    , name_.data()
                    , flags
                    , mode
                    , m == Mode::Async ? openedCb : nullptr);

  if(m == Mode::Sync)
    {
//      callFileOpened();
    }

  return r;
}

int File::open(char *path, const int &flags, const int &mode, const Mode &m)
{
  this->name_ = path;
  return open(flags, mode, m);
}

int File::close(const Mode &m)
{
  int r = uv_fs_close(m_loop->uvHandle()
                     , m_uv_obj
                     , m_uv_obj->result
                     , m == Mode::Async ? closedCb : nullptr);

  if(m == Mode::Sync)
    {
//      callFileClosed();
    }

  return r;
}

std::string File::readAll()
{
  //! This is the way Node.js source code does.
  std::string contents;
  if(buffer_)
    delete buffer_;
  buffer_ = new Buffer(buffer_data_, sizeof(buffer_data_), m_loop);
  int r;

  while (true)
    {
      r = uv_fs_read(m_loop->uvHandle()
                     , m_uv_obj
                     , fd_
                     , buffer_->getUvHandle()
                     , 1
                     , contents.length()  // offset
                     , nullptr);
      uv_fs_req_cleanup(m_uv_obj);

      if (r <= 0)
        break;
      contents.append(buffer_->getUvHandle()->base, r);
    }
  return contents;
}

int File::read(Buffer *buf, const Mode &m)
{
  buffer_ = buf;
  return uv_fs_read(m_loop->uvHandle()
                    , m_uv_obj
                    , fd_
                    , buffer_->getUvHandle()
                    , buffer_->getUvHandle()->len
                    , -1
                    , m == Mode::Async ? readCb : nullptr);
}

int File::write(Buffer *buf, const Mode &m)
{
  return uv_fs_write(m_loop->uvHandle()
                     , m_uv_obj
                     , fd_
                     , buf->getUvHandle()
                     , buf->getUvHandle()->len
                     , -1
                     , m == Mode::Async ? writtenCb : nullptr);
}

int File::write(std::string &data, const Mode &syncMode)
{
  //! This is the way Node.js source code does.
  uv_buf_t buf = uv_buf_init(const_cast<char*>(data.c_str()), data.length());
  return uv_fs_write(m_loop->uvHandle()
                     , m_uv_obj
                     , fd_
                     , &buf
                     , 1
                     , -1
                     , syncMode == Mode::Async ? writtenCb : nullptr);
}

int File::truncate(const int &size, const Mode &m)
{
  return uv_fs_ftruncate(m_loop->uvHandle()
                     , m_uv_obj
                     , m_uv_obj->result
                     , size
                     , nullptr); //! Add Aync Callback!
}

int File::mkdir(const std::string &dir, const int &mode, Loop *l, const Mode &m)
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
  return buffer_;
}

void File::setFileDescriptor(const ssize_t &fd)
{
  fd_ = fd;
}
