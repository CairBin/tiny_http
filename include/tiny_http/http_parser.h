#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#include <string>
#include <unordered_map>

namespace tiny_http{

class HttpRequestParser {
 public:
  // 枚举解析状态
  enum class State {
    kStart,
    kMethod,
    kUrl,
    kVersion,
    kHeaderKey,
    kHeaderValue,
    kBody,
    kDone,
    kError
  };

  // 构造函数
  HttpRequestParser();

  // 解析输入行
  void Parse(const std::string& line);

  // Getter 方法
  const std::string& method() const;
  const std::string& url() const;
  const std::string& version() const;
  const std::unordered_map<std::string, std::string>& headers() const;
  const std::string& body() const;

  // 检查解析是否完成
  bool IsDone() const;

 private:
  // 内部解析函数
  void ParseRequestLine(const std::string& line);
  void ParseHeader(const std::string& line);
  void ParseBody(const std::string& line);

  // 辅助函数：去除字符串首尾空白
  std::string Trim(const std::string& str);

  // 成员变量
  State state_;
  std::string method_;
  std::string url_;
  std::string version_;
  std::unordered_map<std::string, std::string> headers_;
  std::string body_;
  int content_length_;
};

}

#endif // HTTP_PARSER_H_