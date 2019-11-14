#include "file.h"

using namespace Agio;


void File::openedCb(uv_fs_t* r)
{
  File* f = getPHandle(r);
  if (r->result >= 0)
    {
      f->setFileDescriptor(static_cast<int>(r->result));
      uv_fs_req_cleanup(r);

      f->onOpened();
    }
  else
    {
      fprintf(stderr, "error opening file: %s\n", uv_strerror(static_cast<int>(r->result)));
    }
}

void File::closedCb(uv_fs_t* r)
{
  File *f = getPHandle(r);
  if (r->result != -1)
    {
      f->setFileDescriptor(0);
      uv_fs_req_cleanup(r);
      f->onClosed();
    }
  else
    {
      fprintf(stderr, "Error at closing file: %s.\n", uv_strerror(static_cast<int>(r->result)));
    }
}

void File::readCb(uv_fs_t* r)
{
  File* f = getPHandle(r);

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
      f->onReadyRead(f->getBuffer(), r->result);
    }
  uv_fs_req_cleanup(r);
}

void File::writtenCb(uv_fs_t* r)
{
  if (r->result == -1) {
      fprintf(stderr, "Error writting data to file: s.\n");
    }

  //! Nov. 13 2019 Why close this?
//  uv_fs_t closeReq;
//  uv_fs_close(r->loop
//              , &closeReq
//              , static_cast<uv_file>(r->result)
//              , nullptr);
  uv_fs_req_cleanup(r);
//  uv_fs_req_cleanup(&closeReq);
}




File::File(Loop* l)
  : AgioService(l, this)
{
}

File::File(const std::string& path, Loop* l)
  : AgioService(l, this)
  , path_(path)
{
}

File::~File()
{
  uv_fs_req_cleanup(obj_);
}

void File::setPath(const std::string& path)
{
  path_ = path;
}

std::string File::getPath() const
{
  return path_;
}

int File::open(const int& flags, const int& perm, const Mode& m)
{
  int r = fd_ = uv_fs_open(m == Mode::Async ? loop_->cObject() : nullptr
                    , obj_
                    , path_.data()
                    , flags
                    , perm
                    , m == Mode::Async ? openedCb : nullptr);

  if(m == Mode::Sync)
    {
//      callFileOpened();
    }

  return r;
}

int File::open(char* path, const int& flags, const int& perm, const Mode& m)
{
  this->path_ = path;
  return open(flags, perm, m);
}

int File::close(const Mode& m)
{
  int r = uv_fs_close(loop_->cObject()
                     , obj_
                     , static_cast<uv_file>(obj_->result)
                     , m == Mode::Async ? closedCb : nullptr);

  if(m == Mode::Sync)
    {
//      callFileClosed();
    }

  return r;
}

std::string File::readAll()
{
  //! Implementation borrowed from Node.js source code
  std::string data;
  if(buffer_)
    delete buffer_;
  buffer_ = new Buffer(buffer_data_, sizeof(buffer_data_));
  int r;

  while(true)
    {
      r = uv_fs_read(loop_->cObject()
                     , obj_
                     , fd_
                     , buffer_->cObject()
                     , 1
                     , static_cast<int64_t>(data.length())  // offset
                     , nullptr);
      uv_fs_req_cleanup(obj_);

      if (r <= 0)
        break;
      data.append(buffer_->cObject()->base, static_cast<std::string::size_type>(r));
    }
  return data;
}

int File::read(Buffer* buf, const Mode& m)
{
  buffer_ = buf;
  return uv_fs_read(loop_->cObject()
                    , obj_
                    , fd_
                    , buffer_->cObject()
                    , static_cast<unsigned int>(buffer_->cObject()->len)
                    , -1
                    , m == Mode::Async ? readCb : nullptr);
}

int File::write(Buffer* buf, const Mode& m)
{
  return uv_fs_write(loop_->cObject()
                     , obj_
                     , fd_
                     , buf->cObject()
                     , static_cast<unsigned int>(buf->cObject()->len)
                     , -1
                     , m == Mode::Async ? writtenCb : nullptr);
}

int File::write(const std::string& data, const Mode& syncMode)
{
  //! This is the way Node.js source code does.
  uv_buf_t buf = uv_buf_init(const_cast<char*>(data.c_str()), data.length());
  return uv_fs_write(loop_->cObject()
                     , obj_
                     , fd_
                     , &buf
                     , 1
                     , -1
                     , syncMode == Mode::Async ? writtenCb : nullptr);
}

int File::truncate(const int& size, const Mode& m)
{
  return uv_fs_ftruncate(loop_->cObject()
                     , obj_
                     , static_cast<uv_file>(obj_->result)
                     , size
                     , nullptr); //! Add Aync Callback!
}

int File::mkdir(const std::string& dir, const int& perm, Loop* l, const Mode& m)
{
  uv_fs_t r;
  int ret = uv_fs_mkdir(l->cObject()
              , &r
              , dir.data()
              , perm
              , /*syncMode == Mode::Async ? nullptr : nullptr*/nullptr); //! Add Aync Callback!
  uv_fs_req_cleanup(&r);
  return ret;
}

int File::remove(const std::string& file, Loop* l)
{
  uv_fs_t r;
  int ret = uv_fs_unlink(l->cObject()
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

void File::setFileDescriptor(const int& fd)
{
  fd_ = fd;
}
