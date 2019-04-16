#ifndef PFILESYSTEM_H
#define PFILESYSTEM_H

#include "parsley.h"
#include "buffer.h"
#include <string>
#include <iostream>

P_NS_BEGIN

class FileUtils;
class File;


class FileUtils
    : public PUvObject<uv_fs_t, File>
{
public:
  FileUtils(Loop *l);

protected:
  static void openedCb(uv_fs_t* r);
  static void closedCb(uv_fs_t* r);
  static void readCb(uv_fs_t* r);
  static void writtenCb(uv_fs_t* r);

};

class File
    : protected FileUtils
{
  friend FileUtils;
public:
  CallbackHandler<void(Buffer*, const ssize_t&)> onReadyRead;
  CallbackHandler<void()> onOpened;
  CallbackHandler<void()> onClosed;
  CallbackHandler<void()> onWritten;

  File(Loop *l = Loop::defaultLoop());
  File(const std::string &getPath, Loop *l);
  ~File();

  void setPath(const std::string& getPath);
  std::string getPath() const;

  int open(const int &flags, const int &perm, const Mode &m);
  int open(char *getPath, const int &flags, const int &perm, const Mode &m);
  int close(const Mode &m);
  int read(Buffer *buf, const Mode &m);
  std::string readAll();
  int write(Buffer *buf, const Mode &m); // TODO: not finished
  int write(const std::string &data, const Mode &m);
  int truncate(const int &size, const Mode &m);
  static int mkdir(const std::string &dir, const int &perm, Loop *l, const Mode &m);
  static int remove(const std::string &file, Loop *l);
  Buffer *getBuffer();

private:
  ssize_t fd_ = 0;
  std::string path_;
  char buffer_data_[4096];
  Buffer *buffer_ = nullptr;

  void setFileDescriptor(const ssize_t& fd);
};




P_NS_END
#endif // PFILESYSTEM_H
