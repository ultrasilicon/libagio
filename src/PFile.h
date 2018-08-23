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

  using FileOpenedCb = std::function<void (void)>;
  using FileReadyReadCb = std::function<void (Buffer *buf, const ssize_t &len)>; //! see if const Buffer *buf works better
  using FileClosedCb = std::function<void (void)>;
  using FileWrittenCb = std::function<void (void)>;

  FileUtils(Loop *l) : PObject(l){}

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

  bool callFileOpened();
  bool callFileClosed();
  bool callFileReadyRead(const ssize_t &len); // TODO: not finished
  bool callFileEnd(const ssize_t &len); // TODO: not finished

private:
  int file_descriptor = 0;
  std::string path;
  char buffer_memory[4096];
  Buffer *buffer;

  FileOpenedCb file_opened_cb;
  FileReadyReadCb file_ready_read_cb;
  FileClosedCb file_closed_cb;
  FileWrittenCb file_written_cb;
};

//template<class T, typename Ret, typename... Args>
//void File::bindCb(CallbackType t, T *obj, Ret(T::*f)(Args...))
//{
//  switch (t) {
//    case FileOpened:
//      file_opened_cb = std::bind(,);
//      break;
//    case FileClosed:
//      file_closed_cb = (FileClosedCb) t;
//      break;
//    case FileReadyRead:
//      file_ready_read_cb = (FileReadyReadCb) t;
//      break;
//    case FileWritten:
//      file_written_cb = (FileWrittenCb) t;
//      break;
//    default:
//      break;
//    }
//}



PARSLEY_NAMESPACE_END
#endif // PFILESYSTEM_H
