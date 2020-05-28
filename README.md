# libagio
[![license](https://img.shields.io/github/license/ultrasilicon/libagio.svg)](https://github.com/ultrasilicon/libagio/blob/master/LICENSE)
[![platform](https://img.shields.io/badge/Platform-desktop%20%7C%20mobile-ff69b4.svg?style=flat)](http://doc.qt.io/qt-5/supported-platforms.html)

### A cross-platform async I/O library for C++ agile development
* **This library is currently under primary development**
* **libagio** is created based on [libuv](https://github.com/libuv/libuv) to serve for [Project Hive](https://github.com/HiveChat/Hive-desktop).

## Build

- Dependency:
  - [libuv](https://github.com/libuv/libuv)
- Build with cmake:

```bash
$ git clone https://github.com/ultrasilicon/libagio.git
$ cd libagio
$ mkdir build 
$ cd build
$ cmake ..
$ make
$ make install
```

## Examples:

### TCP Server

```c++
#include <libagio/tcp_server.h>
#include <iostream>

using namespace std;
using namespace Agio;

int main()
{
  Loop loop;
  
  TcpServer *server = new TcpServer(&loop);
  server->bind("0.0.0.0", 63773);
  server->listen();
  
  on(&server->onNewConnection, [&](TcpServer* server){
      TcpSocket* sock = new TcpSocket(&loop);
      on(&sock->onReadyRead, [](Buffer* data, TcpSocket* sock){
          cout << sock->peerAddress() << ": " << data->toString() << endl;
        });
      
      server->accept(sock);
    });

  return loop.run();
}
```

### TCP Client

```c++
#include <libagio/tcp_socket.h>
#include <libagio/timer.h>
#include <iostream>

using namespace std;
using namespace Agio;

int main()
{
  Loop loop;

  TcpSocket *client = new TcpSocket(&loop);
  on(&client->onConnected, [](){
      cout << "connected." << endl;
    });

  Timer *timer = new Timer(2000, 500, &loop);
  on(&timer->onTimedOut, [&](Timer*){
      if(client->write("hello") == 0)
        cout << "written" << endl;
    });

  client->connect("127.0.0.1", 63773);
  client->start();
  timer->start();

  return loop.run();
}
```



## Progress

libuv | libParsley | Progress | Comments
------- | ------- | ------- | ------- 
uv_check_t | Parsley::Check | 100% | Done 
uv_lib_t | Parsley::lib | 100% | Done 
uv_async_t | Parsley::Async | 100% | Done 
\- | Parsley::AbstractSocket | 100% |Done
uv_timer_t | Parsley::Timer | 100% | Done 
uv_loop_t | Parsley::Loop | 80% | Good to use 
uv_fs_t | Parsley::File | 60% | Good for basic use, some file & dir operation not yet implemented 
uv_udp_t | Parsley::UdpSocket | 80% | Good for basic use, multicast not yet implemented 
uv_tcp_t | Parsley::TcpSocket | 90% |Good to use
uv_tcp_t | Parsley::TcpServer | 100% |Done




