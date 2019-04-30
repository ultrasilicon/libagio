#include "tcp_server.h"

using namespace Parsley;


void TcpServer::newConnectionCb(uv_stream_t *handle, int status)
{
  if(status < 0)
    {
      fprintf(stderr, "%s\n", uv_strerror(status));
      return;
    }
  TcpServer* s = getInstance((uv_tcp_t*)handle);
  s->onNewConnection(s);
}

TcpServer::TcpServer(Loop *l)
  : TcpServer({}, 0, 128, l)
{
}

TcpServer::TcpServer(char *ip, const int &port, Loop *l)
  : TcpServer(ip, port, 128, l)
{
}

TcpServer::TcpServer(char *ip, const int &port, const int &backLog, Loop *l)
  : PUvObject(l, this)
  , ip_(ip)
  , port_(port)
  , back_log_(backLog)
{
  regInstance(obj_, this);
  uv_tcp_init(loop_->uvHandle(), obj_);
}

int TcpServer::bind()
{
  sockaddr_in *addr = CXX_MALLOC(sockaddr_in);
  uv_ip4_addr(ip_, port_, addr);
  return uv_tcp_bind(obj_
                     , (const sockaddr*) addr
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
  return uv_listen((uv_stream_t*) obj_
                   , back_log_
                   , &newConnectionCb);
}

int TcpServer::listen(const int &backLog)
{
  back_log_ = backLog;
  return listen();
}

void TcpServer::close()
{
  return uv_close((uv_handle_t*) obj_, nullptr);
}

int TcpServer::accept(TcpSocket *client)
{
  int r = uv_accept((uv_stream_t*) obj_
                    , (uv_stream_t*) client->getHandle());
  if(r < 0)
    uv_close((uv_handle_t*) client->getHandle(), nullptr);
  else
    client->start();
  return r;
}




