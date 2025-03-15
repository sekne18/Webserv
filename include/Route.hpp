#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <vector>

struct Route {
  std::string path;
  std::string destination;
  std::vector<std::string> allowedMethods;
  
  bool allowsMethod(const std::string& method) const {
    for (std::vector<std::string>::const_iterator it = allowedMethods.begin(); 
         it != allowedMethods.end(); ++it) {
      if (*it == method || *it == "ALL") {
        return true;
      }
    }
    return false;
  }
};

#endif // ROUTE_HPP
