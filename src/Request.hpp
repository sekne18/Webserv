#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>

class Request
{
  public:
    Request();
    Request(const std::string &rawRequest);
    std::string getMethod() const;
    std::string getUrl() const;
    std::string getHeader(const std::string &key) const;
    std::string getBody() const;
  
  private:
    void parseRequest(const std::string &rawRequest);
    std::string _method;
    std::string _url;
    std::map<std::string, std::string> _headers;
    std::string _body;
};

#endif // REQUEST_HPP
