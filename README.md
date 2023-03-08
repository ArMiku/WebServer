# WebServer
C++ Linux WebServer服务器，基于C++ 11标准。

# 功能
+ 使用Socket实现不同主机之间的通信。
+ 利用IO复用和epoll与线程池实现多线程的Reactor高并发模型
+ 利用正则表达式和状态机解析HTTP请求报文，处理静态资源的请求
+ 利用`vector<char>`封装Buffer，实现自动增长的缓冲区
+ 基于堆结构实现的定时器，关闭超时的非活动连接
+ 实现Mysql数据库连接池，减少了数据库连接与建立的开销

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