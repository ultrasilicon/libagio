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
    : public InstanceMap<uv_fs_t, File>
{
  typedef std::function<void (const int&)> FileOpenCb;

protected:
  static void openCb(uv_fs_t* r);

};

class File
    : protected FileUtils
{

public:
  File(Loop *l);
  File(char *path, Loop *l);

  int open(const int &flags, const int &mode);
  int open(char *path, const int &flags, const int &mode);

  Loop *getLoop();

private:
  Loop *loop;
  uv_fs_t *file_handle;
  char *path;
};

PARSLEY_NAMESPACE_END
#endif // PFILESYSTEM_H
