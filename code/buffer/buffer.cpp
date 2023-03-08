#include "buffer.h"

Buffer::Buffer(int initBuffSize) : buffer_(initBuffSize), readPos_(0), writePos_(0) {}

// 缓存区还可以读入入的字节数
size_t Buffer::ReadableBytes() const {
    return writePos_ - readPos_;
}

// 缓存区还可以写入的字节数
size_t Buffer::WritableBytes() const {
    return buffer_.size() - writePos_;
}

// 缓存区已经读取的字节数
size_t Buffer::PrependableBytes() const {
    return readPos_;
}

// 获取当前读指针
const char* Buffer::Peek() const {
    return BeginPtr_() + readPos_;
}

// 更新读指针
void Buffer::Retrieve(size_t len) {
    assert(len <= ReadableBytes());
    readPos_ += len;
}

// 将读指针直接更新到指定位置
void Buffer::RetrieveUntil(const char* end) {
    assert(Peek() <= end );
    Retrieve(end - Peek());
}

// 初始化读写指针
void Buffer::RetrieveAll() {
    bzero(&buffer_[0], buffer_.size());
    readPos_ = 0;
    writePos_ = 0;
}

// 将缓冲区数据转为字符串
std::string Buffer::RetrieveAllToStr() {
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}

// 获取当前写指针
const char* Buffer::BeginWriteConst() const {
    return BeginPtr_() + writePos_;
}
char* Buffer::BeginWrite() {
    return BeginPtr_() + writePos_;
}

// 更新写指针
void Buffer::HasWritten(size_t len) {
    writePos_ += len;
} 

// 处理http连接过程中需要的读写接口
void Buffer::Append(const std::string& str) {
    Append(str.data(), str.length());
}
void Buffer::Append(const void* data, size_t len) {
    assert(data);
    Append(static_cast<const char*>(data), len);
}
void Buffer::Append(const char* str, size_t len) {
    assert(str);
    EnsureWriteable(len);
    std::copy(str, str + len, BeginWrite());
    HasWritten(len);
}
void Buffer::Append(const Buffer& buff) {
    Append(buff.Peek(), buff.ReadableBytes());
}

// 确保将数据写入缓冲区
void Buffer::EnsureWriteable(size_t len) {
    if(WritableBytes() < len) {
        MakeSpace_(len);
    }
    assert(WritableBytes() >= len);
}
// 与客户端直接IO的读接口
ssize_t Buffer::ReadFd(int fd, int* saveErrno) {
    char buff[65535];
    struct iovec iov[2];
    const size_t writable = WritableBytes();
    /* 分散读， 保证数据全部读完 */
    iov[0].iov_base = BeginPtr_() + writePos_;
    iov[0].iov_len = writable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    const ssize_t len = readv(fd, iov, 2);
    if(len < 0) {
        *saveErrno = errno;
    }
    else if(static_cast<size_t>(len) <= writable) {
        writePos_ += len;
    }
    else {
        writePos_ = buffer_.size();
        Append(buff, len - writable);
    }
    return len;
}
// 与客户端直接IO的写接口
ssize_t Buffer::WriteFd(int fd, int* saveErrno) {
    size_t readSize = ReadableBytes();
    ssize_t len = write(fd, Peek(), readSize);
    if(len < 0) {
        *saveErrno = errno;
        return len;
    } 
    readPos_ += len;
    return len;
}
// 返回指向缓冲区初始位置的指针
char* Buffer::BeginPtr_() {
    return &*buffer_.begin();
}
const char* Buffer::BeginPtr_() const {
    return &*buffer_.begin();
}
// 不够时候扩容
void Buffer::MakeSpace_(size_t len) {
    if(WritableBytes() + PrependableBytes() < len) {
        buffer_.resize(writePos_ + len + 1);
    } 
    else {
        size_t readable = ReadableBytes();
        std::copy(BeginPtr_() + readPos_, BeginPtr_() + writePos_, BeginPtr_());
        readPos_ = 0;
        writePos_ = readPos_ + readable;
        assert(readable == ReadableBytes());
    }
}