#ifndef PFILESYSTEM_H
#define PFILESYSTEM_H

#include "Parsley.h"
#include <string>
#include <iostream>

PARSLEY_NAMESPACE_BEGIN

class FileUtils;
class File;


class FileUtils
    : public PObject<uv_fs_t, File>
{
public:
  enum CallbackType {
    FileOpened = 1,
    FileClosed = 2,
    FileReadyRead = 3,
    FileWritten = 4
  };
  FileUtils(Loop *l) : PObject(l){}
  typedef std::function<void (void)> FileOpenedCb;
  typedef std::function<void (Buffer *buf, const ssize_t &len)> FileReadyReadCb; //! see if const Buffer *buf works better
  typedef std::function<void (void)> FileClosedCb;
  typedef std::function<void (void)> FileWrittenCb;

protected:
  static void openedCb(uv_fs_t* r);
  static void closedCb(uv_fs_t* r);
  static void readCb(uv_fs_t* r);
  static void writtenCb(uv_fs_t* r);

};

class File // TODO: callback binds
    : protected FileUtils
{
public:
  File(Loop *l);
  File(char *path, Loop *l);
  ~File();

  template<typename T>
  void bindCb(CallbackType &t, T &cb);

  int open(const int &flags, const int &mode, const Mode &syncMode = Mode::AsyncMode);
  int open(char *path, const int &flags, const int &mode, const Mode &syncMode = Mode::AsyncMode);
  int close(const Mode &syncMode = Mode::AsyncMode);
  int read(Buffer *buf, const Mode &syncMode = Mode::AsyncMode);
  std::string readAll();
  int write(Buffer *buf, const Mode &syncMode = Mode::AsyncMode); // TODO: not finished
  int writeAll(std::string &data, const Mode &syncMode);
  static int mkdir(char *dir, const int &mode, Loop *l, const Mode &syncMode = Mode::AsyncMode);

  Buffer *getBuffer();

  bool callFileOpened();
  bool callFileClosed();
  bool callFileReadyRead(const ssize_t &len); // TODO: not finished
  bool callFileEnd(const ssize_t &len); // TODO: not finished

private:
  int file_descriptor = 0;
  char *path; 
  char buffer_memory[4096];
  Buffer *buffer;

  FileOpenedCb file_opened_cb;
  FileReadyReadCb file_ready_read_cb;
  FileClosedCb file_closed_cb;
  FileWrittenCb file_written_cb;
};



PARSLEY_NAMESPACE_END
#endif // PFILESYSTEM_H
