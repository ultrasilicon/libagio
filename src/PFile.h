#ifndef PFILESYSTEM_H
#define PFILESYSTEM_H

#include "Parsley.h"
#include <string>

PARSLEY_NAMESPACE_BEGIN

class FileUtils;
class File;


class FileUtils
    : public PObject<uv_fs_t, File>
{
public:
  FileUtils(Loop *l) : PObject(l){}
  typedef std::function<void (void)> FileOpenedCb;
  typedef std::function<void (Buffer *buf, const ssize_t &len)> FileReadyReadCb; //! see if const Buffer *buf works better
  typedef std::function<void (void)> FileClosedCb;

protected:
  static void openedCb(uv_fs_t* r);
  static void closedCb(uv_fs_t* r);
  static void readCb(uv_fs_t* r);

};

class File
    : protected FileUtils
{

public:
  File(Loop *l);
  File(char *path, Loop *l);
  ~File();

  int open(const int &flags, const int &mode, const Mode &syncMode = Mode::AsyncMode);
  int open(char *path, const int &flags, const int &mode, const Mode &syncMode = Mode::AsyncMode);
  int close(const Mode &syncMode = Mode::AsyncMode);
  int read(Buffer *buf, const Mode &syncMode = Mode::AsyncMode);
  std::string readAll();
  static int mkdir(char *dir, const int &mode, Loop *l, const Mode &syncMode = Mode::AsyncMode);

  Buffer *getBuffer();

  bool callFileOpened();
  bool callFileClosed();
  bool callFileReadyRead(const ssize_t &len);



private:
  int file_descriptor;
  char *path; 
  char buffer_memory[4096];
  Buffer *buffer;

  FileOpenedCb file_opened_cb;
  FileReadyReadCb file_ready_read_cb;
  FileClosedCb file_closed_cb;
};

PARSLEY_NAMESPACE_END
#endif // PFILESYSTEM_H
