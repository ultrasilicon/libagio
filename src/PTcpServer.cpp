#include "PTcpServer.h"

#include <stdlib.h>

using namespace Parsley;


void TcpServerUtils::tcpNewConnectionCb(uv_stream_t *handle, int status)
{
  if(status < 0)
    {
      fprintf(stderr, "Listen error %s\n", uv_strerror(status));
      return;
    }
  getInstance((uv_tcp_t*)handle)->accept();
}



TcpServer::TcpServer(Loop *l)
  : TcpServerUtils (l)
{
  uv_tcp_init(loop->uvHandle(), uv_handle);
}

TcpServer::TcpServer(const char *ip, const int &port, const int &backLog, Loop *l)
  : TcpServerUtils (l)
{
  uv_tcp_init(loop->uvHandle(), uv_handle);

  int r = uv_listen((uv_stream_t*) uv_handle, backLog, tcpNewConnectionCb);
  if(r)
    {
      fprintf(stderr, "Listen error %s\n", uv_strerror(r));
    }
}

int TcpServer::bind(const char *ip, const int &port)
{
  struct sockaddr_in *addr = (sockaddr_in*) malloc(sizeof(sockaddr_in));
  uv_ip4_addr(ip, port, addr);
  return uv_tcp_bind(uv_handle, (const struct sockaddr*) addr, 0);
}

int TcpServer::listen()
{
  return uv_listen((uv_stream_t*) uv_handle
                   , 128
                   , &tcpNewConnectionCb);
}

int TcpServer::stop()
{
  return 0;
}

bool TcpServer::accept()
{
  uv_connect_t *v = new uv_connect_t();
  delete v;

  std::map<int, int> *m = new std::map<int,int>();
//  m->insert();
//  delete m;

  TcpSocket *client = new TcpSocket(loop);
  connect(&client->onReadyRead, this, &TcpServer::onReadyRead); //< Make id and record.
  client_set.insert(client);
  return uv_accept((uv_stream_t*)uv_handle, (uv_stream_t*)client->getUvHandle()) == 0;
}

void TcpServer::onReadyRead(Buffer buf, char* ip)
{
  fprintf(stderr, "%s", buf.base);
}











