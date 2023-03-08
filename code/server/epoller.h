#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/epoll.h> //epoll_ctl()
#include <fcntl.h>  // fcntl()
#include <unistd.h> // close()
#include <assert.h> // close()
#include <vector>
#include <errno.h>

class Epoller {
public:
    explicit Epoller(int maxEvent = 1024);

    ~Epoller();

    // 将描述符fd加入epoll监控
    bool AddFd(int fd, uint32_t events);
    // 修改描述符fd对应的事件
    bool ModFd(int fd, uint32_t events);
    // 将描述符删除epoll的监控
    bool DelFd(int fd);
    // 用于返回监控的结果，成功时返回就绪的文件名描述符个数
    int Wait(int timeoutMs = -1);
    // 获取fd
    int GetEventFd(size_t i) const;
    // 获取events
    uint32_t GetEvents(size_t i) const;
        
private:
    int epollFd_;   // epoll的描述符

    std::vector<struct epoll_event> events_;    // 就绪的事件
};

#endif //EPOLLER_H