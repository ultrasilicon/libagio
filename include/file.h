#ifndef PFILESYSTEM_H
#define PFILESYSTEM_H

#define ASIO_FILE_READ_BUF_SIZE 4096

#include "service.h"
#include "buffer.h"
#include "callback.h"

A_NS_BEGIN


class File;
class FileInfo;


class File
    : public AgioService<uv_fs_t, File>
{
  static void fileCb(uv_fs_t* r);
  static void writtenCb(uv_fs_t* r);

public:
  Callback<void(Buffer*, const ssize_t&)> onReadyRead;
  Callback<void()> onOpened;
  Callback<void()> onClosed;
  Callback<void()> onWritten;
  Callback<void(FileInfo)> onInfo;

  File(Loop* l = Loop::defaultLoop());
  File(const std::string& path, Loop* l);
  ~File();

  void setPath(const std::string& getPath);
  std::string getPath() const;

  int open(const int& flags, const int& perm, const Mode& m);
  int open(char* getPath, const int& flags, const int& perm, const Mode& m);
  int close(const Mode& m);
  int read(Buffer* buf, const Mode& m);
  std::vector<char> readAll();
  int write(Buffer* buf, const Mode& m); // TODO: not finished
  int write(const std::string& data, const Mode& m);
  FileInfo stat(const std::string& dir, const Mode& m);
  int truncate(const int& size, const Mode& m);
  static int mkdir(const std::string& dir, const int& perm, Loop* l, const Mode& m);
  static int remove(const std::string& file, Loop* l);
  Buffer* getBuffer();

private:
  int fd_ = -1;
  std::string path_;

  void setFileDescriptor(const int& fd);
};



class FileInfo
    : public uv_stat_t
{
public:
  FileInfo() {}
  FileInfo(const uv_stat_t& stat) : uv_stat_t(stat) {}

  const uint64_t& parentDevice() const { return st_dev; }
  const uint64_t& mode() const { return st_mode; }
  const uint64_t& hardLinks() const { return st_nlink; }
  const uint64_t& uid() const { return st_uid; }
  const uint64_t& gid() const { return st_gid; }
  const uint64_t& device() const { return st_rdev; }
  const uint64_t& inode() const { return st_ino; }
  const uint64_t& size() const { return st_size; }
  const uint64_t& blockSize() const { return st_blksize; }
  const uint64_t& blocks() const { return st_blocks; }
  const uint64_t& flags() const { return st_flags; }
  const uint64_t& generation() const { return st_gen; }
  const uv_timespec_t& timeLastAccess() const { return st_atim; }
  const uv_timespec_t& timeLastModify() const { return st_mtim; }
  const uv_timespec_t& timeLastStatChange() const { return st_ctim; }
  const uv_timespec_t& timeCreated() const { return st_birthtim; }
};


A_NS_END
#endif // PFILESYSTEM_H
