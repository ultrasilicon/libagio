#ifndef PADDRESS_H
#define PADDRESS_H

#include "agio.h"

#include <string>

A_NS_BEGIN

class HostAddress;


class HostAddress
{
public:
  enum Version {
    None = 0,
    IPv4 = 1,
    IPv6 = 2,
  };

  static std::string toString(const sockaddr_storage& addr);
  static std::string toIpString(const in_addr& addr);
  static std::string toIpString(const in6_addr& addr);

  HostAddress();
  HostAddress(const sockaddr_storage& addr);
  HostAddress(const sockaddr_in& addr);
  HostAddress(const sockaddr_in6& addr);
  HostAddress(const std::string& ip, const int& port);
  ~HostAddress();

  friend std::ostream& operator<<(std::ostream& out, const HostAddress& addr);

  void setAddress(const sockaddr_storage& addr);
  void setAddress(const sockaddr_in& addr);
  void setAddress(const sockaddr_in6& addr);
  void setAddress(const std::string& ip, const int& port);
  Version version() const;
  bool isValid() const;
  std::string ipString() const;
  uint16_t port() const;
  std::string toString() const;

private:
  Version version_ = None;
  sockaddr_in ip4_;
  sockaddr_in6 ip6_;
};


inline std::ostream& operator<<(std::ostream& out, const HostAddress& addr) {
  return out << addr.toString();
}

A_NS_END
#endif // PADDRESS_H
