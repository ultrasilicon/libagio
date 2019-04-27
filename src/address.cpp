#include "address.h"
#include <iostream>
using namespace Parsley;

IPAddress::IPAddress()
{
}

IPAddress::IPAddress(const sockaddr_storage &addr)
{
  setAddress(addr);
}

IPAddress::IPAddress(const sockaddr_in &addr)
{
  setAddress(addr);
}

IPAddress::IPAddress(const sockaddr_in6 &addr)
{
  setAddress(addr);
}

IPAddress::IPAddress(const std::string& ip, const uint16_t& port)
{
  setAddress(ip, port);
}

IPAddress::~IPAddress()
{
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

void IPAddress::setAddress(const sockaddr_storage &addr)
{
  if(addr.ss_family == AF_INET)
    {
      setAddress((sockaddr_in&) addr);
    }
  else if(addr.ss_family == AF_INET6)
    {
      setAddress((sockaddr_in6&) addr);
    }
  else if(addr.ss_family == AF_UNIX)
    {
      setAddress("0.0.0.0", 0);
    }
  else
    {
      setAddress((sockaddr_in&) addr);
    }
}

void IPAddress::setAddress(const sockaddr_in &addr)
{
  version_ = IPv4;
  ip4_ = addr;
}

void IPAddress::setAddress(const sockaddr_in6 &addr)
{
  version_ = IPv6;
  ip6_ = addr;
}

void IPAddress::setAddress(const std::string &ip, const uint16_t &port)
{
  char buf[sizeof(in6_addr)];
  if(uv_inet_pton(AF_INET, ip.c_str(), buf) == 0)
    {
      uv_ip4_addr(ip.c_str(), port, &ip4_);
//      m_ip4->sin_len = 0;
//      m_ip4->sin_family = AF_INET;
//      m_ip4->sin_port = htons(port);
//      m_ip4->sin_addr = *((in_addr*)&buf);
//      memset(m_ip4->sin_zero, 0, 8);
      version_ = IPv4;
    }
  else if(uv_inet_pton(AF_INET6, ip.c_str(), buf) == 0)
    {
      uv_ip6_addr(ip.c_str(), port, &ip6_);
//      m_ip6->sin6_len = 0;
//      m_ip6->sin6_family = AF_INET6;
//      m_ip6->sin6_port = htons(port);
//      m_ip6->sin6_addr = *((in6_addr*)&buf);
      version_ = IPv6;
    }
}

IPAddress::Version IPAddress::version() const
{
  return version_;
}

std::string IPAddress::toIPString() const
{
  if(version_ == IPv4)
    return toIPString(ip4_.sin_addr);
  else if(version_ == IPv6)
    return toIPString(ip6_.sin6_addr);
  return "";
}


