#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include <errno.h>     
#include <mysql/mysql.h>  //mysql

#include "../buffer/buffer.h"
#include "../log/log.h"
#include "../pool/sqlconnpool.h"
#include "../pool/sqlconnRAII.h"

class HttpRequest {
public:
    enum PARSE_STATE {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,        
    };

    enum HTTP_CODE {
        NO_REQUEST = 0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURSE,
        FORBIDDENT_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION,
    };
    
    HttpRequest() { Init(); }
    ~HttpRequest() = default;

    // 初始化
    void Init();
    // 有限状态自动机解析 http 请求
    bool parse(Buffer& buff);

    // 获取 http 信息接口
    std::string path() const;
    std::string& path();
    std::string method() const;
    std::string version() const;
    // post 下获取信息
    std::string GetPost(const std::string& key) const;
    std::string GetPost(const char* key) const;

    // http 连接是否存活
    bool IsKeepAlive() const;

    /* 
    todo 
    void HttpConn::ParseFormData() {}
    void HttpConn::ParseJson() {}
    */

private:
    // 解析请求行
    bool ParseRequestLine_(const std::string& line);
    // 解析请求头
    void ParseHeader_(const std::string& line);
    // 解析数据体
    void ParseBody_(const std::string& line);

    // 处理路径信息
    void ParsePath_();
    // 解析 post 报文
    void ParsePost_();
    void ParseFromUrlencoded_();

    // 用户验证
    static bool UserVerify(const std::string& name, const std::string& pwd, bool isLogin);

    PARSE_STATE state_;                                     // 自动机状态
    std::string method_, path_, version_, body_;            //  http 方式、路径、版本和数据体
    std::unordered_map<std::string, std::string> header_;   // 请求头
    std::unordered_map<std::string, std::string> post_;     // post已经解析出的信息

    static const std::unordered_set<std::string> DEFAULT_HTML;           // 默认的网页名称
    static const std::unordered_map<std::string, int> DEFAULT_HTML_TAG; 
    static int ConverHex(char ch);                                       // 转换十六进制
};


#endif //HTTP_REQUEST_H