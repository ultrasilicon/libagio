#include "PAddress.h"
#include <iostream>
using namespace Parsley;

IPAddress::IPAddress()
{
}

IPAddress::IPAddress(const sockaddr_storage &addr)
{
  sockaddr_storage *c = CXX_MALLOC(sockaddr_storage);
  memcpy((sockaddr_storage*) &addr, c, sizeof(sockaddr_storage));
  setAddress(c);
}

IPAddress::IPAddress(const sockaddr_in &addr)
{
  sockaddr_in *c = CXX_MALLOC(sockaddr_in);
  memcpy((sockaddr_in*) &addr, c, sizeof(sockaddr_in));
  setAddress(c);
}

IPAddress::IPAddress(const sockaddr_in6 &addr)
{
  sockaddr_in6 *c = CXX_MALLOC(sockaddr_in6);
  memcpy((sockaddr_in6*) &addr, c, sizeof(sockaddr_in6));
  setAddress(c);
}

IPAddress::IPAddress(sockaddr_storage *addr)
{
  setAddress(addr);
}

IPAddress::IPAddress(sockaddr_in *addr)
{
  setAddress(addr);
}

IPAddress::IPAddress(sockaddr_in6 *addr)
{
  setAddress(addr);
}

IPAddress::IPAddress(const std::string& addr, const uint16_t& port)
{
  setAddress(addr, port);
}

IPAddress::~IPAddress()
{
  if(m_version == IPv4)
    free(m_ip4);
  if(m_version == IPv6)
    free(m_ip6);
}

std::string IPAddress::toString(sockaddr_storage &addr)
{
  char hoststr[NI_MAXHOST];
  char portstr[NI_MAXSERV];
  getnameinfo((sockaddr *) &addr
              , sizeof(addr)
              , hoststr
              , sizeof(hoststr)
              , portstr
              , sizeof(portstr)
              , NI_NUMERICHOST | NI_NUMERICSERV);
  return std::string(hoststr) + ":" + portstr;
}

std::string IPAddress::toIPString(in_addr addr)
{
  char buf[INET_ADDRSTRLEN];
  if(!inet_ntop(AF_INET, &addr, buf, sizeof(buf)))
    return {};
  return buf;
}

std::string IPAddress::toIPString(in6_addr addr)
{
  char buf[INET6_ADDRSTRLEN];
  if(!inet_ntop(AF_INET6, &addr, buf, sizeof(buf)))
    return {};
  return buf;
}

void IPAddress::setAddress(sockaddr_storage *addr)
{
  if(addr->ss_family == AF_INET)
    {
      setAddress((sockaddr_in*) addr);
    }
  else if(addr->ss_family == AF_INET6)
    {
      setAddress((sockaddr_in6*) addr);
    }
  else if(addr->ss_family == AF_UNIX)
    {
      setAddress("0.0.0.0", 0);
    }
  else
    {
      setAddress((sockaddr_in*) addr);
    }
}

void IPAddress::setAddress(sockaddr_in *addr)
{
  m_version = IPv4;
  m_ip4 = addr;
}

void IPAddress::setAddress(sockaddr_in6 *addr)
{
  m_version = IPv6;
  m_ip6 = addr;
}

void IPAddress::setAddress(const std::string &addr, const uint16_t &port)
{
  char buf[16];
  if(inet_pton(AF_INET, addr.c_str(), buf))
    {
      m_ip4 = new sockaddr_in();
      m_ip4->sin_len = 0;
      m_ip4->sin_family = AF_INET;
      m_ip4->sin_port = htons(port);
      m_ip4->sin_addr = *((in_addr*)&buf);
      memset(m_ip4->sin_zero, 0, 8);
      m_version = IPv4;
    }
  else if(inet_pton(AF_INET6, addr.c_str(), buf))
    {
      m_ip6 = new sockaddr_in6();
      m_ip6->sin6_len = 0;
      m_ip6->sin6_family = AF_INET6;
      m_ip6->sin6_port = htons(port);
      m_ip6->sin6_addr = *((in6_addr*)&buf);
      m_version = IPv6;
    }
}

IPAddress::Version IPAddress::version() const
{
  return m_version;
}

std::string IPAddress::toIPString() const
{
  if(m_version == IPv4 && m_ip4)
    return toIPString(m_ip4->sin_addr);
  else if(m_version == IPv6 && m_ip6)
    return toIPString(m_ip6->sin6_addr);
  else
    return "";
}


