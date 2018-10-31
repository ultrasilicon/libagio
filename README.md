# libParsley
[![license](https://img.shields.io/github/license/ultrasilicon/libParsley.svg)](https://github.com/ultrasilicon/libParsley/blob/master/LICENSE)
[![platform](https://img.shields.io/badge/Platform-desktop%20%7C%20mobile-ff69b4.svg?style=flat)](http://doc.qt.io/qt-5/supported-platforms.html)

### Asynchronous cross-platform high performance C++ library 
* **This library is currently under primary development**
* Platform: Linux, MacOS, Windows, Android and embedded unix.
* Naming convention follows libqt.
* libParsley is created based on [libuv](https://github.com/libuv/libuv) to serve for [Project Hive](https://github.com/HiveChat/Hive-desktop), the lovely p2p chat app.
### Examples:

##### Simple TCP Server

```c++
#include <iostream>
#include <PTcpServer.h>

using namespace std;
using namespace Parsley;

void receive_cb(Buffer* data, const string &ip)
{
  cout << ip << ": " << data->toString() << '\n';
  delete data;
}

int main()
{
  Loop l;

  TcpServer *server = new TcpServer(&l);
  server->bind("0.0.0.0", 63773);
  server->listen();
  connect(&server->onReadyRead, &receive_cb);

  return l.run();
}
```



### Progress

libuv | libParsley | Progress | Comments
------- | ------- | ------- | ------- 
uv_check_t | Parsley::Check | 100% | Done 
uv_async_t | Parsley::Async | 100% | Done 
\- | Parsley::AbstractSocket | 100% |Done
uv_timer_t | Parsley::Timer | 90% | Good to use
uv_loop_t | Parsley::Loop | 60% | Good for basic use 
uv_fs_t | Parsley::File | 60% | Good for basic use, some file & dir operatin not yet supported 
uv_udp_t | Parsley::UdpSocket | 60% | Good for basic use, multicast not yet implemented 
uv_tcp_t | Parsley::TcpSocket | 50% |Good for basic use
uv_tcp_t | Parsley::TcpServer | 90% |Good to use



## Documents
* Coming soon!

## Build
* Coming soon!


