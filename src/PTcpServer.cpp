#include "PTcpServer.h"

#include <stdlib.h>

using namespace Parsley;


void TcpServerUtils::newConnectionCb(uv_stream_t *handle, int status)
{
  if(status < 0)
    {
      fprintf(stderr, "%s\n", uv_strerror(status));
      return;
    }
  getInstance((uv_tcp_t*)handle)->accept();
}

TcpServer::TcpServer(Loop *l)
  : TcpServer("", 0, 128, l)
{
}

TcpServer::TcpServer(char *ip, const int &port, Loop *l)
  : TcpServer(ip, port, 128, l)
{
}

TcpServer::TcpServer(char *ip, const int &port, const int &backLog, Loop *l)
  : TcpServerUtils (l)
  , ip_(ip)
  , port_(port)
  , back_log_(backLog)
{
  regInstance(uv_handle, this);
  uv_tcp_init(loop->uvHandle(), uv_handle);
}

int TcpServer::bind()
{
  struct sockaddr_in *addr = (sockaddr_in*) malloc(sizeof(sockaddr_in));
  uv_ip4_addr(ip_, port_, addr);
  return uv_tcp_bind(uv_handle
                     , (const struct sockaddr*) addr
                     , 0);
}

int TcpServer::bind(char *ip, const int &port)
{
  ip_ = ip;
  port_ = port;
  return bind();
}

int TcpServer::listen()
{
  return uv_listen((uv_stream_t*) uv_handle
                   , back_log_
                   , &newConnectionCb);
}

int TcpServer::listen(const int &backLog)
{
  back_log_ = backLog;
  return listen();
}

int TcpServer::stop()
{
  return 0;
}

void TcpServer::accept()
{
  TcpSocket *client = new TcpSocket(loop);
  //< Here we invoke TcpSocket as a user, but we are acturally an internal class.
  //< To be more efficient, why not use a static function binding?
  //< a children map might be needed, recording sockets' fd.
  connect(&client->onReadyRead, &onReadyRead); //<< record fd?
  client_set.insert(client);
  if(uv_accept((uv_stream_t*)uv_handle, (uv_stream_t*)client->getUvHandle()) == 0)
    {
      client->start();
    }
  else
    {
      uv_close((uv_handle_t*) client, nullptr);
    }
}

//void TcpServer::onPacketReady(Buffer buf, char *ip) //< Theoretically shoud not exist until mecanism above is built
//{
//  onReadyRead.call(buf, ip);
//}









