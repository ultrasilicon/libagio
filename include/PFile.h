#ifndef PFILESYSTEM_H
#define PFILESYSTEM_H

#include "Parsley.h"
#include "PBuffer.h"
#include <string>
#include <iostream>

PARSLEY_NAMESPACE_BEGIN

class FileUtils;
class File;


class FileUtils
    : public PObject<uv_fs_t, File>
{
public:
  FileUtils(Loop *l) : PObject(l){}

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
  File(Loop *l);
  File(const std::string &path, Loop *l);
  ~File();

  Callback<void, Buffer*, const ssize_t&> onReadyRead;
  Callback<void> onOpened;
  Callback<void> onClosed;
  Callback<void> onWritten;

  int open(const int &flags, const int &mode, const Mode &syncMode);
  int open(char *path, const int &flags, const int &mode, const Mode &syncMode);
  int close(const Mode &syncMode);
  int read(Buffer *buf, const Mode &syncMode);
  std::string readAll();
  int write(Buffer *buf, const Mode &syncMode); // TODO: not finished
  int write(std::string &data, const Mode &syncMode);
  int truncate(const int &size, const Mode &syncMode);
  static int mkdir(const std::string &dir, const int &mode, Loop *l, const Mode &syncMode);
  static int remove(const std::string &file, Loop *l);

  Buffer *getBuffer();

private:
  void setFileDescriptor(const ssize_t& fd);

  ssize_t file_descriptor = 0;
  std::string path;
  char buffer_memory[4096];
  Buffer *buffer = nullptr;
};




PARSLEY_NAMESPACE_END
#endif // PFILESYSTEM_H
