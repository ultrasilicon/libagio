#ifndef PFILESYSTEM_H
#define PFILESYSTEM_H

#include "Parsley.h"
#include "PBuffer.h"
#include <string>
#include <iostream>

P_NS_BEGIN

class FileUtils;
class File;


class FileUtils
    : public PObject<uv_fs_t, File>
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
  Callback<void, Buffer*, const ssize_t&> onReadyRead;
  Callback<void> onOpened;
  Callback<void> onClosed;
  Callback<void> onWritten;

  File(Loop *l);
  File(const std::string &path, Loop *l);
  ~File();
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
  ssize_t m_fd = 0;
  std::string m_name;
  char m_buffer_data[4096];
  Buffer *m_buffer = nullptr;

  void setFileDescriptor(const ssize_t& fd);
};




P_NS_END
#endif // PFILESYSTEM_H
