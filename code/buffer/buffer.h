#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>   //perror
#include <iostream>
#include <unistd.h>  // write
#include <sys/uio.h> //readv
#include <vector> //readv
#include <atomic>
#include <assert.h>

class Buffer {
public:
    Buffer(int initBuffSize = 1024);
    ~Buffer() = default;

    // 缓冲区还可以读取的字节数
    size_t WritableBytes() const;
    
    // 缓冲区还可以读取的字节数       
    size_t ReadableBytes() const;
    
    // 缓冲区已经读取的字节数
    size_t PrependableBytes() const;
    
    // 获取当前读指针
    const char* Peek() const;
    
    // 确保将数据写入缓冲区
    void EnsureWriteable(size_t len);
    // 更新写指针
    void HasWritten(size_t len);
    
    // 更新读指针
    void Retrieve(size_t len);
    // 将读指针直接更新到指定位置
    void RetrieveUntil(const char* end);
    
    // 初始化读写指针
    void RetrieveAll();
    
    // 将缓冲区数据转为字符串
    std::string RetrieveAllToStr();
    
    // 获取当前写指针
    const char* BeginWriteConst() const;
    char* BeginWrite();
    
    // 处理http连接过程中需要的读写接口
    void Append(const std::string& str);
    void Append(const char* str, size_t len);
    void Append(const void* data, size_t len);
    void Append(const Buffer& buff);
    
    // 与客户端直接IO的读写接口
    ssize_t ReadFd(int fd, int* Errno);
    ssize_t WriteFd(int fd, int* Errno);

private:
    // 返回指向缓冲区初始位置的指针
    char* BeginPtr_();
    const char* BeginPtr_() const;
    // 不够时候扩容
    void MakeSpace_(size_t len);

    std::vector<char> buffer_;  // 存储的实体
    std::atomic<std::size_t> readPos_;  // 当前读的位置
    std::atomic<std::size_t> writePos_; // 当前写的位置
};

#endif //BUFFER_H