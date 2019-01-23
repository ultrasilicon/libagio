#include "PAddress.h"
#include <iostream>
using namespace Parsley;

IPAddress::IPAddress()
{
}

IPAddress::IPAddress(sockaddr_storage *addr)
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

std::string IPAddress::toIPString(in_addr addr) const
{
  //! Ghost code!!
  //! Do not call this function, it manipulates other class members!
  char buf[INET_ADDRSTRLEN];
  if(!inet_ntop(AF_INET, &addr, buf, sizeof(buf)))
    return {};
  return buf;
}

std::string IPAddress::toIPString(in6_addr addr)
{
  //! Ghost code!!
  //! Do not call this function, it manipulates other class members!
  char buf[INET6_ADDRSTRLEN];
  if(!inet_ntop(AF_INET6, &addr, buf, sizeof(buf)))
    return {};
  return buf;
}

void IPAddress::setAddress(sockaddr_in *addr)
{
  m_version = IPv4;
  m_ip4 = CXX_MALLOC(sockaddr_in);
  memset(&m_ip4, 0, sizeof(sockaddr_in));
  m_ip4 = addr;
}

void IPAddress::setAddress(sockaddr_in6 *addr)
{
  m_version = IPv6;
  m_ip6 = CXX_MALLOC(sockaddr_in6);
  memset(&m_ip6, 0, sizeof(sockaddr_in6));
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
    {
      in_addr addr = m_ip4->sin_addr;
      char buf[INET_ADDRSTRLEN];
      std::cout << "std::string IPAddress::toIPString(const sockaddr_in &addr)\n";
      if(!inet_ntop(AF_INET, &addr, buf, sizeof(buf)))
        return {};
      return buf;
    }
  else if(m_version == IPv6 && m_ip6)
    {
      in6_addr addr = m_ip6->sin6_addr;
      char buf[INET6_ADDRSTRLEN];
      if(!inet_ntop(AF_INET6, &addr, buf, sizeof(buf)))
        return {};
      return buf;
    }
  else
    return "";
}


