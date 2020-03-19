#include "file.h"

#include <uv.h>
#include <vector>

using namespace Agio;


void File::fileCb(uv_fs_t* r)
{
  File* f = getAgioService(r);

  if (r->result >= 0)
    {
      switch (r->fs_type) {
        case UV_FS_OPEN:
          {
            f->setFileDescriptor(static_cast<int>(r->result));
            f->onOpened();
            uv_fs_req_cleanup(r);
            break;
          }
        case UV_FS_CLOSE:
          {
            f->setFileDescriptor(-1);
            f->onClosed();
            uv_fs_req_cleanup(r);
            break;
          }
        case UV_FS_READ:
          {
//            f->onReadyRead(r->bufs, r->result == 0 ? UV_EOF : r->result);
            break;
          }
        case UV_FS_WRITE:
          {
            break;
          }
        case UV_FS_SENDFILE:
          {
            break;
          }
        case UV_FS_STAT:
          {
            f->onInfo(*static_cast<uv_stat_t*>(r->ptr));
            uv_fs_req_cleanup(r);
            delete r;
            break;
          }
        case UV_FS_LSTAT:
          {
            break;
          }
        case UV_FS_FSTAT:
          {
            break;
          }
        case UV_FS_FTRUNCATE:
          {
            break;
          }
        case UV_FS_UTIME:
          {
            break;
          }
        case UV_FS_FUTIME:
          {
            break;
          }
        case UV_FS_ACCESS:
          {
            break;
          }
        case UV_FS_CHMOD:
          {
            break;
          }
        case UV_FS_FCHMOD:
          {
            break;
          }
        case UV_FS_FSYNC:
          {
            break;
          }
        case UV_FS_FDATASYNC:
          {
            break;
          }
        case UV_FS_UNLINK:
          {
            break;
          }
        case UV_FS_RMDIR:
          {
            break;
          }
        case UV_FS_MKDIR:
          {
            break;
          }
        case UV_FS_MKDTEMP:
          {
            break;
          }
        case UV_FS_RENAME:
          {
            break;
          }
        case UV_FS_SCANDIR:
          {
            break;
          }
        case UV_FS_LINK:
          {
            break;
          }
        case UV_FS_SYMLINK:
          {
            break;
          }
        case UV_FS_READLINK:
          {
            break;
          }
        case UV_FS_CHOWN:
          {
            break;
          }
        case UV_FS_FCHOWN:
          {
            break;
          }
        case UV_FS_REALPATH:
          {
            break;
          }
        case UV_FS_COPYFILE:
          {
            break;
          }
        case UV_FS_LCHOWN:
          {
            break;
          }
        default:
          break;
        }
    }
  else
    {
      fprintf(stderr, "fs error: %s\n", uv_strerror(static_cast<int>(r->result)));
    }
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
  fd_ = uv_fs_open(m == Mode::Async ? loop_->cObject() : nullptr
                    , obj_
                    , path_.data()
                    , flags
                    , perm
                    , m == Mode::Async ? fileCb : nullptr);

  if(m == Mode::Sync)
    {
//      callFileOpened();
    }

  return fd_;
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
                     , m == Mode::Async ? fileCb : nullptr);
  return r;
}

std::vector<char> File::readAll()
{
  if(fd_ == -1)
    this->open(O_RDONLY, 0755, Sync);

  //! Implementation borrowed from Node.js source code
  std::vector<char> data;
  Buffer* buf = new Buffer(ASIO_FILE_READ_BUF_SIZE);

  while(true)
    {
      int r = uv_fs_read(loop_->cObject()
                     , obj_
                     , fd_
                     , buf->cObject()
                     , 1
                     , static_cast<int64_t>(data.size())  // offset
                     , nullptr);

      uv_fs_req_cleanup(obj_);
      if (r <= 0)
        break;
      data.insert(data.begin(), buf->front(), buf->back());
    }
  return data.size() == 0 ? std::vector<char>{'\0'} : data;
}

int File::read(Buffer* buf, const Mode& m)
{
  return uv_fs_read(loop_->cObject()
                    , obj_
                    , fd_
                    , buf->cObject()
                    , 1
                    , -1
                    , /*m == Mode::Async ? fileCb :*/ nullptr);
}

int File::write(Buffer* buf, const Mode& m)
{
  return uv_fs_write(loop_->cObject()
                     , obj_
                     , fd_
                     , buf->cObject()
                     , 1
                     , -1
                     , m == Mode::Async ? writtenCb : nullptr);
}

int File::write(const std::string& data, const Mode& m)
{
  //! This is the way Node.js source code does.
  uv_buf_t buf = uv_buf_init(const_cast<char*>(data.c_str()), data.length());
  return uv_fs_write(loop_->cObject()
                     , obj_
                     , fd_
                     , &buf
                     , 1
                     , -1
                     , m == Mode::Async ? writtenCb : nullptr);
}

FileInfo File::stat(const std::string& dir, const Mode& m)
{
  uv_fs_t* req = new uv_fs_t();
  uv_fs_stat(loop_->cObject()
                    , req
                    , dir.c_str()
                    , m == Mode::Async ? fileCb : nullptr);
  return m == Mode::Async ? FileInfo() : *static_cast<uv_stat_t*>(req->ptr);
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

void File::setFileDescriptor(const int& fd)
{
  fd_ = fd;
}
