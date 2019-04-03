#ifndef PADDRESS_H
#define PADDRESS_H

#include "parsley.h"

#include <netinet/in.h>

P_NS_BEGIN
class IPAddress;

class IPAddress
{
public:
  enum Version {
    None = 0,
    IPv4 = 1,
    IPv6 = 2,
  };

  IPAddress();
  IPAddress(const sockaddr_storage &addr);
  IPAddress(const sockaddr_in &addr);
  IPAddress(const sockaddr_in6 &addr);
  IPAddress(const std::string& ip, const uint16_t& port);
  ~IPAddress();

  static std::string toString(sockaddr_storage &addr);
  static std::string toIPString(in_addr addr);
  static std::string toIPString(in6_addr addr);

  void setAddress(const sockaddr_storage &addr);
  void setAddress(const sockaddr_in &addr);
  void setAddress(const sockaddr_in6 &addr);
  void setAddress(const std::string& ip, const uint16_t& port);
  Version version() const;
  std::string toIPString() const;

//private:
  Version m_version = None;
  sockaddr_in m_ip4;
  sockaddr_in6 m_ip6;
};



P_NS_END
#endif // PADDRESS_H