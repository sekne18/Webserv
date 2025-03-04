#include "Config.hpp"
#include "Utils.hpp"
#include <fstream>
#include <sstream>

/*
 * Manages server configuration.
 * Default values:
 *   - Port: 8080
 *   - Server name: localhost
 *   - Document root: www
 *   - Uploads directory: www/uploads
 *   - Routes: empty
 *   - Routes are defined in a config file with the following format:
 *      port=8080
 *      server_name=localhost
 *      document_root=www
 *      uploads_dir=www/uploads
 *      route=/uploads:www/uploads:POST
 *      route=/api/ *:www/api:GET,POST
 *   - Each route is defined by a path, a destination directory, and a list of allowed methods.
 *   - The path can contain a wildcard '*' at the end for prefix matching.
 *   - The destination directory is relative to the document root.
 *   - The list of allowed methods is comma-separated.
 *   - If no route matches a request, the default route is used (path='/', destination=document_root, methods='GET').
 *   - The config file is loaded in the constructor.
 *   - The config file is optional. If not found, default values are used.
 * */
Config::Config() : _port(8080), _serverName("localhost"), _documentRoot("www"), _uploadsDir("www/uploads")
{
}

Config::Config(const std::string& configFile) : _port(8080), _serverName("localhost"), _documentRoot("www"), _uploadsDir("www/uploads")
{
  loadFromFile(configFile);
}

void Config::loadFromFile(const std::string& configFile)
{
  std::ifstream file(configFile.c_str());
  if (!file) {
    std::cerr << "Warning: Config file not found: " << configFile << ". Using default values.\n";
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') {
      continue;
    }

    // Parse key-value pairs
    size_t delimiterPos = line.find('=');
    if (delimiterPos != std::string::npos) {
      std::string key = trim(line.substr(0, delimiterPos));
      std::string value = trim(line.substr(delimiterPos + 1));

      if (key == "port") {
        _port = Utils::stringToInt(value.c_str());
      } else if (key == "server_name") {
        _serverName = value;
      } else if (key == "document_root") {
        _documentRoot = value;
      } else if (key == "uploads_dir") {
        _uploadsDir = value;
      } else if (key == "route") {
        parseRoute(value);
      }
    }
  }
}

void Config::parseRoute(const std::string& routeConfig)
{
  // Format: path:destination:methods
  std::istringstream iss(routeConfig);
  std::string path, destination, methodList;
  
  std::getline(iss, path, ':');
  std::getline(iss, destination, ':');
  std::getline(iss, methodList);
  
  Route route;
  route.path = trim(path);
  route.destination = trim(destination);
  
  // Parse methods (comma-separated)
  std::istringstream methodsStream(methodList);
  std::string method;
  while (std::getline(methodsStream, method, ',')) {
    route.allowedMethods.push_back(trim(method));
  }
  
  _routes.push_back(route);
}

std::string Config::trim(const std::string& str)
{
  const std::string whitespace = " \t\n\r\f\v";
  size_t start = str.find_first_not_of(whitespace);
  
  if (start == std::string::npos) {
    return ""; // String contains only whitespace
  }
  
  size_t end = str.find_last_not_of(whitespace);
  return str.substr(start, end - start + 1);
}

int Config::getPort() const
{
  return _port;
}

std::string Config::getServerName() const
{
  return _serverName;
}

std::string Config::getDocumentRoot() const
{
  return _documentRoot;
}

std::string Config::getUploadsDir() const
{
  return _uploadsDir;
}

const std::vector<Route>& Config::getRoutes() const
{
  return _routes;
}

Route Config::getRouteForPath(const std::string& path) const
{
  for (std::vector<Route>::const_iterator it = _routes.begin(); it != _routes.end(); ++it) {
    if (matchesPath(path, it->path)) {
      return *it;
    }
  }
  
  // Return default route
  Route defaultRoute;
  defaultRoute.path = "/";
  defaultRoute.destination = _documentRoot + "/";
  defaultRoute.allowedMethods.push_back("GET");
  return defaultRoute;
}

bool Config::matchesPath(const std::string& requestPath, const std::string& routePath) const
{
  // Simple direct match
  if (requestPath == routePath) {
    return true;
  }
  
  // Check if route path ends with '*' for prefix matching
  if (!routePath.empty() && routePath[routePath.size() - 1] == '*') {
    std::string prefix = routePath.substr(0, routePath.size() - 1);
    return requestPath.substr(0, prefix.size()) == prefix;
  }
  
  return false;
}
