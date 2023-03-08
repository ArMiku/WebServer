#ifndef HEAP_TIMER_H
#define HEAP_TIMER_H

#include <queue>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <arpa/inet.h> 
#include <functional> 
#include <assert.h> 
#include <chrono>
#include "../log/log.h"

typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;

struct TimerNode {
    int id;                 // 标记定时器
    TimeStamp expires;      // 设置过期时间
    TimeoutCallBack cb;     //设置一个回调函数用来方便删除定时器时将对应的HTTP连接关闭
    // 需要的功能可以自己设定
    bool operator<(const TimerNode& t) {
        return expires < t.expires;
    }
};
class HeapTimer {
public:
    HeapTimer() { heap_.reserve(64); }

    ~HeapTimer() { clear(); }
    
    void adjust(int id, int newExpires);

    // 添加一个定时器
    void add(int id, int timeOut, const TimeoutCallBack& cb);

    // 删除制定id节点，并且用指针触发处理函数
    void doWork(int id);

    void clear();

    // 处理过期的定时器
    void tick();

    void pop();

    // 下一次处理过期定时器的时间
    int GetNextTick();

private:

/***********实现堆的方法 begin***********/
    // 添加一个定时器
    // void add(int id, int timeOut, const TimeoutCallBack& cb);
    // 删除指定定时器
    void del_(size_t i);
    // 向上调整
    void siftup_(size_t i);
    // 向下调整
    bool siftdown_(size_t index, size_t n);
    // 交换两个节点位置
    void SwapNode_(size_t i, size_t j);
/***********实现堆的方法 end***********/

    // 定时器的存储实体 小顶堆
    std::vector<TimerNode> heap_;
    
    // 
    std::unordered_map<int, size_t> ref_;
};

#endif //HEAP_TIMER_H