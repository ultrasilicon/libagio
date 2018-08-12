# libParsley
[![license](https://img.shields.io/github/license/ultrasilicon/libParsley.svg)](https://github.com/ultrasilicon/libParsley/blob/master/LICENSE)
[![platform](https://img.shields.io/badge/Platform-desktop%20%7C%20mobile-ff69b4.svg?style=flat)](http://doc.qt.io/qt-5/supported-platforms.html)

```
If you want any functions to be completed in a week, please request an issue. I will add it to libParsley immediately.
Because this project is the base of Hive-desktop, some libuv functions might not be complete for a long time just because they are not used. Thanks for your love!
```



### An OOP wrapper for libuv in C++ with customised callback system. 
* **This library is currently under primary development**
* Platform: Linux, MacOS, Windows, Android and embedded unix.
* Naming convention follows libqt.
* libParsley is created to serve for Project Hive, the lovely p2p chat app.
### Progress

libuv | libParsley | Progress | Comments
------- | ------- | ------- | ------- 
uv_check_t | Parsley::Check | 100% | Done 
uv_async_t | Parsley::Async | 100% | Done 
\- | Parsley::AbstractSocket | 100% |Done
uv_timer_t | Parsley::Timer | 90% | Good to use
uv_loop_t | Parsley::Loop | 60% | Good for basic use 
uv_fs_t | Parsley::File | 60% | Good for basic use, more file & dir operatin not yet supported 
uv_udp_t | Parsley::UdpSocket | 60% | Good for basic use, multicast not yet implemented 
uv_tcp_t | Parsley::TcpSocket | 30% |
uv_tcp_t | Parsley::TcpServer | 10% |



## Documents
* Coming soon!

## Build
* Coming soon!


