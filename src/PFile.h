#ifndef PFILESYSTEM_H
#define PFILESYSTEM_H

#include "Parsley.h"
#include "PLoop.h"


PARSLEY_NAMESPACE_BEGIN

//class UdpSocketUtils
//    : public InstanceMap<uv_udp_t, UdpSocket>
//{
//protected:
//  static void receiveCb(uv_udp_t* handle, ssize_t nread, const Buffer *buf, const sockaddr *addr, unsigned flags);
//  static void writeCb(uv_udp_send_t* req, int status);

//};


//class UdpSocket
//    : public AbstractSocket
//    , protected UdpSocketUtils
//{
//public:
//  SockReadyReadCb read_cb;

//  UdpSocket(Loop *l);
//  UdpSocket(const char *ipAddr, const int &port, Loop *loop);

//  void bind(const char *ipAddr, const int &port);
//  void start();
//  void stop();
//  void write(const char *ipAddr, const int &port, const Buffer *buf);
//  void setBroadcatEnabled(const bool &enabled = true);

//  uv_udp_t* getSocket() {return udp_socket;}


//protected:
//  uv_udp_t* udp_socket;

//};

class FileUtils;
class File;


class FileUtils
    : public PObject<uv_fs_t, File>
{
public:
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

  int open(const int &flags, const int &mode, const Mode &syncMode = Mode::Async);
  int open(char *path, const int &flags, const int &mode, const Mode &syncMode = Mode::Async);
  int close(const Mode &syncMode = Mode::Async);
  int read(Buffer *buf, const Mode &syncMode = Mode::Async);
  static int mkdir(char *dir, const int &mode, Loop *l, const Mode &syncMode = Mode::Async);

  Loop *getLoop();
  Buffer *getBuffer();

  bool callFileOpened();
  bool callFileClosed();
  bool callFileReadyRead(const ssize_t &len);



private:
  Loop *loop;
  char *path; 
  Buffer *buffer;

  FileOpenedCb file_opened_cb;
  FileReadyReadCb file_ready_read_cb;
  FileClosedCb file_closed_cb;
};

PARSLEY_NAMESPACE_END
#endif // PFILESYSTEM_H
