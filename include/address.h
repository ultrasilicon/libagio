#ifndef PADDRESS_H
#define PADDRESS_H

#include "agio.h"

#include <netinet/in.h>

A_NS_BEGIN

class IP;
class Address;

class IP
{
public:
  enum Version {
    None = 0,
    IPv4 = 1,
    IPv6 = 2,
  };

  static std::string toString(sockaddr_storage &addr);
  static std::string toIPString(in_addr addr);
  static std::string toIPString(in6_addr addr);

  IP();
  IP(const sockaddr_storage &addr);
  IP(const sockaddr_in &addr);
  IP(const sockaddr_in6 &addr);
  IP(const std::string& ip, const uint16_t& port);
  ~IP();

  void setAddress(const sockaddr_storage &addr);
  void setAddress(const sockaddr_in &addr);
  void setAddress(const sockaddr_in6 &addr);
  void setAddress(const std::string& ip, const uint16_t& port);
  Version version() const;
  bool isValid() const;
  std::string toIPString() const;

private:
  Version version_ = None;
  sockaddr_in ip4_;
  sockaddr_in6 ip6_;
};

class Address
{
public:
  Address() {}
};

A_NS_END
#endif // PADDRESS_H
