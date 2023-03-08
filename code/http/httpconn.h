#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include <sys/types.h>
#include <sys/uio.h>     // readv/writev
#include <arpa/inet.h>   // sockaddr_in
#include <stdlib.h>      // atoi()
#include <errno.h>      

#include "../log/log.h"
#include "../pool/sqlconnRAII.h"
#include "../buffer/buffer.h"
#include "httprequest.h"
#include "httpresponse.h"

class HttpConn {
public:
    HttpConn();

    ~HttpConn();
    // 初始化 http 连接
    void init(int sockFd, const sockaddr_in& addr);
    
    // 每个连接中定义的缓冲区读写接口
    ssize_t read(int* saveErrno);
    ssize_t write(int* saveErrno);
    
    // 关闭 http 连接
    void Close();

    // 获取 http 连接描述符，也是唯一标志
    int GetFd() const;

    // 获取端口号
    int GetPort() const;

    // 获取 IP 信息
    const char* GetIP() const;
    
    sockaddr_in GetAddr() const;
    
    // 用于完成解析请求和响应请求的整体逻辑
    bool process();

    // 获取已经写入的数据长度
    int ToWriteBytes() { 
        return iov_[0].iov_len + iov_[1].iov_len; 
    }

    // 获取这个 HTTP 连接KeepAlive的状态
    bool IsKeepAlive() const {
        return request_.IsKeepAlive();
    }

    static bool isET;
    static const char* srcDir;
    static std::atomic<int> userCount;
    
private:
   
    int fd_;                    // http连接对应的描述符
    struct  sockaddr_in addr_;  // 套接字地址 

    bool isClose_;              // 标记是否关闭连接
    
    int iovCnt_;
    struct iovec iov_[2];
    
    Buffer readBuff_;           // 读缓冲区
    Buffer writeBuff_;          // 写缓冲区

    HttpRequest request_;
    HttpResponse response_;
};

#endif // HTTP_CONN_H