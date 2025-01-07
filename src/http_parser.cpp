#include "tiny_http/http_parser.h"

#include <sstream>
#include <stdexcept>

namespace tiny_http{

// 构造函数
HttpRequestParser::HttpRequestParser() 
    : state_(State::kStart), content_length_(0) {}

// 解析输入行
void HttpRequestParser::Parse(const std::string& line) {
  switch (state_) {
    case State::kStart:
      ParseRequestLine(line);
      break;
    case State::kHeaderKey:
    case State::kHeaderValue:
      ParseHeader(line);
      break;
    case State::kBody:
      ParseBody(line);
      break;
    case State::kDone:
    case State::kError:
      throw std::runtime_error("Cannot parse in current state.");
    default:
      throw std::logic_error("Invalid state.");
  }
}

// Getter 方法
const std::string& HttpRequestParser::method() const { return method_; }
const std::string& HttpRequestParser::url() const { return url_; }
const std::string& HttpRequestParser::version() const { return version_; }
const std::unordered_map<std::string, std::string>& HttpRequestParser::headers() const { 
  return headers_; 
}
const std::string& HttpRequestParser::body() const { return body_; }
bool HttpRequestParser::IsDone() const { return state_ == State::kDone; }

// 解析请求行
void HttpRequestParser::ParseRequestLine(const std::string& line) {
  std::istringstream stream(line);
  if (!(stream >> method_ >> url_ >> version_)) {
    state_ = State::kError;
    throw std::runtime_error("Failed to parse request line.");
  }
  state_ = State::kHeaderKey;
}

// 解析头部字段
void HttpRequestParser::ParseHeader(const std::string& line) {
  if (line.empty()) {
    if (headers_.count("Content-Length") > 0) {
      content_length_ = std::stoi(headers_["Content-Length"]);
      state_ = content_length_ > 0 ? State::kBody : State::kDone;
    } else {
      state_ = State::kDone;
    }
    return;
  }

  auto colon_pos = line.find(':');
  if (colon_pos == std::string::npos) {
    state_ = State::kError;
    throw std::runtime_error("Malformed header line: " + line);
  }

  std::string key = Trim(line.substr(0, colon_pos));
  std::string value = Trim(line.substr(colon_pos + 1));
  headers_[key] = value;
}

// 解析请求体
void HttpRequestParser::ParseBody(const std::string& line) {
  body_ += line;
  if (body_.size() >= static_cast<size_t>(content_length_)) {
    state_ = State::kDone;
  }
}

// 去除字符串首尾空白
std::string HttpRequestParser::Trim(const std::string& str) {
  size_t start = str.find_first_not_of(" \t");
  size_t end = str.find_last_not_of(" \t");
  return (start == std::string::npos || end == std::string::npos)
             ? ""
             : str.substr(start, end - start + 1);
}

}