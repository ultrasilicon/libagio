#include "PTcpServer.h"

#include <stdlib.h>

using namespace Parsley;


TcpServerUtils::TcpServerUtils(Loop *l)
  : PUvObject(l)
{
}

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
  : TcpServer({}, 0, 128, l)
{
}

TcpServer::TcpServer(char *ip, const int &port, Loop *l)
  : TcpServer(ip, port, 128, l)
{
}

TcpServer::TcpServer(char *ip, const int &port, const int &backLog, Loop *l)
  : TcpServerUtils (l)
  , m_ip(ip)
  , m_port(port)
  , m_back_log(backLog)
{
  regInstance(m_uv_obj, this);
  uv_tcp_init(m_loop->uvHandle(), m_uv_obj);
}

int TcpServer::bind()
{
  sockaddr_in *addr = CXX_MALLOC(sockaddr_in);
  uv_ip4_addr(m_ip, m_port, addr);
  return uv_tcp_bind(m_uv_obj
                     , (const struct sockaddr*) addr
                     , 0);
}

int TcpServer::bind(char *ip, const int &port)
{
  ip = ip;
  m_port = port;
  return bind();
}

int TcpServer::listen()
{
  return uv_listen((uv_stream_t*) m_uv_obj
                   , m_back_log
                   , &newConnectionCb);
}

int TcpServer::listen(const int &backLog)
{
  m_back_log = backLog;
  return listen();
}

int TcpServer::stop()
{
  return 0;
}

void TcpServer::accept()
{
  TcpSocket *client = new TcpSocket(m_loop);
//  < Here we invoke TcpSocket as a user, but we are acturally an internal class.
//  < To be more efficient, why not use a static function binding?
//  < a children map might be needed, recording sockets' fd.
  connect(&client->onReadyRead, &this->onReadyRead); //<< record fd?
  m_client_set.insert(client);
  if(uv_accept((uv_stream_t*) m_uv_obj, (uv_stream_t*) client->getUvHandle()) == 0)
    {
      client->start();
    }
  else
    {
      uv_close((uv_handle_t*) client, nullptr);
    }
}




