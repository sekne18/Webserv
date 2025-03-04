#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Route.hpp"

class Config {
private:
  int _port;
  std::string _serverName;
  std::string _documentRoot;
  std::string _uploadsDir;
  std::vector<Route> _routes;
  
  void parseRoute(const std::string& routeConfig);
  std::string trim(const std::string& str);
  bool matchesPath(const std::string& requestPath, const std::string& routePath) const;

public:
  Config();
  Config(const std::string& configFile);
  
  void loadFromFile(const std::string& configFile);
  
  int getPort() const;
  std::string getServerName() const;
  std::string getDocumentRoot() const;
  std::string getUploadsDir() const;
  const std::vector<Route>& getRoutes() const;
  Route getRouteForPath(const std::string& path) const;
};

#endif
