#ifndef PFILESYSTEM_H
#define PFILESYSTEM_H

#define ASIO_FILE_READ_BUF_SIZE 2

#include "service.h"
#include "buffer.h"

A_NS_BEGIN

class File
    : public AgioService<uv_fs_t, File>
{
  static void fileCb(uv_fs_t* r);
  static void writtenCb(uv_fs_t* r);

public:
  CallbackHandler<void(Buffer*, const ssize_t&)> onReadyRead;
  CallbackHandler<void()> onOpened;
  CallbackHandler<void()> onClosed;
  CallbackHandler<void()> onWritten;

  File(Loop* l = Loop::defaultLoop());
  File(const std::string& getPath, Loop* l);
  ~File();

  void setPath(const std::string& getPath);
  std::string getPath() const;

  int open(const int& flags, const int& perm, const Mode& m);
  int open(char* getPath, const int& flags, const int& perm, const Mode& m);
  int close(const Mode& m);
  int read(Buffer* buf, const Mode& m);
  std::string readAll();
  int write(Buffer* buf, const Mode& m); // TODO: not finished
  int write(const std::string& data, const Mode& m);
  int truncate(const int& size, const Mode& m);
  static int mkdir(const std::string& dir, const int& perm, Loop* l, const Mode& m);
  static int remove(const std::string& file, Loop* l);
  Buffer* getBuffer();

private:
  int fd_ = 0;
  std::string path_;

  void setFileDescriptor(const int& fd);
};




A_NS_END
#endif // PFILESYSTEM_H
