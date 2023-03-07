# WebServer
C++ Linux WebServer服务器，基于C++ 11标准。

# 压力测试
MacBook Pro 16-inch M1 Max下Parallel Desktop虚拟机分配2C 4G  
客户端数1500，持续时间10

`./webbench-1.5/webbench -c 1500 -t 10 http://127.0.0.1:1316/`

Speed=376806 pages/min, 19762174 bytes/sec.  
Requests: 62801 susceed, 0 failed.

# TODO
+ config配置文件
+ 单元测试
+ 实现循环缓冲区
+ 添加文件上传功能