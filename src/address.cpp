#include "address.h"

#include <string>

using namespace Agio;


std::string HostAddress::toString(const sockaddr_storage& addr)
{
  char hoststr[NI_MAXHOST];
  char portstr[NI_MAXSERV];
  getnameinfo(reinterpret_cast<const sockaddr*>(&addr)
              , sizeof(addr)
              , hoststr
              , sizeof(hoststr)
              , portstr
              , sizeof(portstr)
              , NI_NUMERICHOST | NI_NUMERICSERV);
  return std::string(hoststr) + ":" + portstr;
}

std::string HostAddress::toIpString(const in_addr& addr)
{
  char buf[INET_ADDRSTRLEN];
  if(uv_inet_ntop(AF_INET, &addr, buf, sizeof(buf)) != 0)
    return {};
  return buf;
}

std::string HostAddress::toIpString(const in6_addr& addr)
{
  char buf[INET6_ADDRSTRLEN];
  if(uv_inet_ntop(AF_INET6, &addr, buf, sizeof(buf)) != 0)
    return {};
  return buf;
}


HostAddress::HostAddress()
{
}

HostAddress::HostAddress(const sockaddr_storage& addr)
{
  setAddress(addr);
}

HostAddress::HostAddress(const sockaddr_in& addr)
{
  setAddress(addr);
}

HostAddress::HostAddress(const sockaddr_in6& addr)
{
  setAddress(addr);
}

HostAddress::HostAddress(const std::string& ip, const uint16_t& port)
{
  setAddress(ip, port);
}

HostAddress::~HostAddress()
{
}

void HostAddress::setAddress(const sockaddr_storage& addr)
{
  switch (addr.ss_family) {
    case AF_INET:
      {
        setAddress(const_cast<sockaddr_in&>(reinterpret_cast<const sockaddr_in&>(addr)));
        break;
      }
    case AF_INET6:
      {
        setAddress(const_cast<sockaddr_in6&>(reinterpret_cast<const sockaddr_in6&>(addr)));
        break;
      }
    case AF_UNIX:
      {
        setAddress("0.0.0.0", 0);
        break;
      }
    default:
      {
        setAddress(const_cast<sockaddr_in&>(reinterpret_cast<const sockaddr_in&>(addr)));
        break;
      }
    }
}

void HostAddress::setAddress(const sockaddr_in& addr)
{
  version_ = IPv4;
  ip4_ = addr;
}

void HostAddress::setAddress(const sockaddr_in6& addr)
{
  version_ = IPv6;
  ip6_ = addr;
}

void HostAddress::setAddress(const std::string& ip, const uint16_t& port)
{
  char buf[sizeof(in6_addr)];
  if(uv_inet_pton(AF_INET, ip.c_str(), buf) == 0)
    {
      //! impl with libuv
      uv_ip4_addr(ip.c_str(), port, &ip4_);
      //! Don't know why libuv doesn't translate port number correctly
      ip4_.sin_port = static_cast<uint16_t>(port);
      //! impl without libuv
//      ip4_->sin_len = 0;
//      ip4_->sin_family = AF_INET;
//      ip4_->sin_port = htons(port);
//      ip4_->sin_addr = *((in_addr*)&buf);
//      memset(ip4_->sin_zero, 0, 8);
      version_ = IPv4;
    }
  else if(uv_inet_pton(AF_INET6, ip.c_str(), buf) == 0)
    {
      //! impl with libuv
      uv_ip6_addr(ip.c_str(), port, &ip6_);
      //! Don't know why libuv doesn't translate port number correctly
      ip6_.sin6_port = static_cast<uint16_t>(port);
      //! impl without libuv
//      ip6_->sin6_len = 0;
//      ip6_->sin6_family = AF_INET6;
//      ip6_->sin6_port = htons(port);
//      ip6_->sin6_addr = *((in6_addr*)&buf);
      version_ = IPv6;
    }
}

HostAddress::Version HostAddress::version() const
{
  return version_;
}

bool HostAddress::isValid() const
{
  return version_ != None;
}

std::string HostAddress::ipString() const
{
  if(version_ == IPv4)
    return toIpString(ip4_.sin_addr);
  else if(version_ == IPv6)
    return toIpString(ip6_.sin6_addr);
  return "";
}

uint16_t HostAddress::port() const
{
  if(version_ == IPv4)
    return ip4_.sin_port;
  else if(version_ == IPv6)
    return ip6_.sin6_port;
  return 0;
}

std::string HostAddress::toString() const
{
  return ipString() + ':' + std::to_string(port());
}


