#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <unordered_map>
#include <fcntl.h>       // open
#include <unistd.h>      // close
#include <sys/stat.h>    // stat
#include <sys/mman.h>    // mmap, munmap

#include "../buffer/buffer.h"
#include "../log/log.h"

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();

    // 初始化
    void Init(const std::string& srcDir, std::string& path, bool isKeepAlive = false, int code = -1);
    // 生成响应报文的主函数
    void MakeResponse(Buffer& buff);
    // 共享内存扫尾函数
    void UnmapFile();
    
    // 返回文件信息
    char* File();
    size_t FileLen() const;

    // 请求打不开返回错误信息
    void ErrorContent(Buffer& buff, std::string message);
    // 返回状态码
    int Code() const { return code_; }

private:
    // 生成相应报文：请求行、请求头和数据体
    void AddStateLine_(Buffer &buff);
    void AddHeader_(Buffer &buff);
    void AddContent_(Buffer &buff);

    // 4XX 状态码分开考虑
    void ErrorHtml_();
    // 获取文件类型
    std::string GetFileType_();

    int code_;              // http 状态码
    bool isKeepAlive_;      // http 连接是否存活

    std::string path_;      // 解析得到的路径
    std::string srcDir_;    // 根目录
    
    // 共享内存相关信息
    char* mmFile_; 
    struct stat mmFileStat_;

    // 表示后缀名到文件类型的映射
    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
    // 表示状态码到相应状态 (字符串类型) 的映射
    static const std::unordered_map<int, std::string> CODE_STATUS;
    // 4XX 状态码到响应文件路径的映射
    static const std::unordered_map<int, std::string> CODE_PATH;
};


#endif //HTTP_RESPONSE_H