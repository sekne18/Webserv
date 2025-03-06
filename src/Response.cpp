#include "Response.hpp"
#include "CGI.hpp"

/*
* Generates an HTTP response based on the request and sends it to the client.
* The response may be a static file, a CGI script, or an error message.
 * */
Response::Response(const Config& config) : _config(config)
{
}

void Response::processRequest(const Request& request, int clientSocket)
{
  std::string method = request.getMethod();
  std::string url = request.getUrl();
  
  if (method == "GET") {
    if (url == "/")
      serveStaticFile(_config.getDocumentRoot() + "/index.html", clientSocket);
    else if (url.find("/cgi-bin/") == 0)
    {
      // Execute a CGI script
      std::string scriptPath = _config.getDocumentRoot() + url;
      std::string queryString = request.getHeader("Query-String");
      
      CGI cgi(_config);
      cgi.executeScript(scriptPath, queryString, clientSocket);
    } else {
      // Serve a static file
      serveStaticFile(_config.getDocumentRoot() + url, clientSocket);
    }
  } else if (method == "POST") {
    if (url == "/upload") {
      // Handle file upload
      handleFileUpload(request.getBody(), clientSocket);
    } else {
      // Unsupported POST request
      sendErrorResponse(405, "Method Not Allowed", clientSocket);
    }
  } else if (method == "DELETE") {
    handleDeleteResponse(_config.getDocumentRoot() + url, clientSocket);
  } else {
    // Unsupported HTTP method
    sendErrorResponse(405, "Method Not Allowed", clientSocket);
  }
}

void Response::handleDeleteResponse(const std::string& filePath, int clientSocket)
{
  // Check if the file exists and is accessible
  if (access(filePath.c_str(), F_OK) != 0) {
    sendErrorResponse(404, "Not Found", clientSocket);
    return;
  }

  // Check if the file is a regular file
  struct stat fileStat;
  if (stat(filePath.c_str(), &fileStat) != 0) {
    sendErrorResponse(500, "Internal Server Error", clientSocket);
    return;
  }

  // Don't allow deleting directories
  if (!S_ISREG(fileStat.st_mode)) {
    sendErrorResponse(400, "Bad Request", clientSocket);
    return;
  }

  // Fork a child process
  pid_t pid = fork();
  if (pid == -1) {
    sendErrorResponse(500, "Internal Server Error", clientSocket);
    return;
  }

  if (pid == 0) {
    // Child process
    char* args[] = {const_cast<char*>("/bin/rm"), const_cast<char*>(filePath.c_str()), NULL};
    char *envp[] = {NULL};
    
    execve("/bin/rm", args, envp);

    // If execvp fails
    std::cerr << "Error: Failed to execute 'rm' command. " << strerror(errno) << "\n";
    exit(1);
  }
  
  // Parent process
  int status;
  waitpid(pid, &status, 0);

  if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
    // Determine MIME type based on file extension
    std::string mimeType = getMimeType(filePath);
  
    // Success - send 200 OK Response
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << mimeType << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << "Resource successfully deleted\r\n"; 

    write(clientSocket, response.str().c_str(), response.str().size());
  }  else {
    // Error - send 500 Internal Server Error Response
    sendErrorResponse(500, "Internal Server Error", clientSocket);
  }
}

void Response::serveStaticFile(const std::string& filePath, int clientSocket)
{
  // Open the file in binary mode which is necessary for non-text files
  std::ifstream file(filePath.c_str(), std::ios::binary);
  if (!file) {
    sendErrorResponse(404, "Not Found", clientSocket);
    return;
  }

  // Determine MIME type based on file extension
  std::string mimeType = getMimeType(filePath);

  std::ostringstream response;
  response << "HTTP/1.1 200 OK\r\n";
  response << "Content-Type: " << mimeType << "\r\n";
  
  // Get file size
  file.seekg(0, std::ios::end);
  size_t fileSize = file.tellg();
  file.seekg(0, std::ios::beg);
  
  response << "Content-Length: " << fileSize << "\r\n";
  response << "Connection: close\r\n";
  response << "\r\n";
  
  // Write headers
  write(clientSocket, response.str().c_str(), response.str().size());
  
  // Write file content
  char buffer[4096];
  while (file.read(buffer, sizeof(buffer)).gcount() > 0) {
    write(clientSocket, buffer, file.gcount());
  }
}

void Response::handleFileUpload(const std::string& body, int clientSocket)
{
  // Extract the file name and content from the body
  size_t filenameStart = body.find("filename=\"");
  if (filenameStart == std::string::npos) {
    std::cerr << "Error: 'filename' not found in request body." << "\n";
    sendErrorResponse(400, "Bad Request", clientSocket);
    return;
  }
  filenameStart += 10; // Skip "filename=\""
  size_t filenameEnd = body.find("\"", filenameStart);
  std::string filename = body.substr(filenameStart, filenameEnd - filenameStart);

  size_t fileContentStart = body.find("\r\n\r\n", filenameEnd);
  if (fileContentStart == std::string::npos) {
    std::cerr << "Error: File content not found in request body." << "\n";
    sendErrorResponse(400, "Bad Request", clientSocket);
    return;
  }
  fileContentStart += 4; // Skip "\r\n\r\n"
  
  // Find the boundary to properly extract the file content
  size_t boundaryStart = body.find("--", fileContentStart);
  std::string fileContent;
  if (boundaryStart != std::string::npos) {
    fileContent = body.substr(fileContentStart, boundaryStart - fileContentStart - 2); // -2 for \r\n before boundary
  } else {
    fileContent = body.substr(fileContentStart);
  }

  // Save the file to the uploads directory
  std::string filePath = _config.getUploadsDir() + "/" + filename;
  
  std::ofstream file(filePath.c_str(), std::ios::binary);
  if (!file) {
    std::cerr << "Error: Failed to open file for writing: " << filePath << "\n";
    sendErrorResponse(500, "Internal Server Error", clientSocket);
    return;
  }
  file.write(fileContent.c_str(), fileContent.size());
  file.close();

  // Send a success response
  std::ostringstream response;
  response << "HTTP/1.1 200 OK\r\n";
  response << "Content-Type: text/html\r\n";
  response << "\r\n";
  response << "<html><body><h1>File uploaded successfully!</h1></body></html>";

  write(clientSocket, response.str().c_str(), response.str().size());
}

void Response::sendErrorResponse(int statusCode, const std::string& statusMessage, int clientSocket)
{
  std::ostringstream response;
  response << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";
  response << "Content-Type: text/html\r\n";
  response << "Connection: close\r\n";
  response << "\r\n";
  response << "<html><body><h1>" << statusCode << " " << statusMessage << "</h1></body></html>";
  
  write(clientSocket, response.str().c_str(), response.str().size());
}

std::string Response::getMimeType(const std::string& filePath)
{
  // Extract the file extension
  size_t dotPos = filePath.find_last_of('.');
  if (dotPos == std::string::npos) {
    return "application/octet-stream"; // Default MIME type
  }
  
  std::string extension = filePath.substr(dotPos + 1);
  
  // Convert to lowercase for case-insensitive comparison
  std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
  
  // Map common extensions to MIME types
  if (extension == "html" || extension == "htm") return "text/html";
  if (extension == "css") return "text/css";
  if (extension == "js") return "application/javascript";
  if (extension == "json") return "application/json";
  if (extension == "txt") return "text/plain";
  if (extension == "jpg" || extension == "jpeg") return "image/jpeg";
  if (extension == "png") return "image/png";
  if (extension == "gif") return "image/gif";
  if (extension == "svg") return "image/svg+xml";
  if (extension == "pdf") return "application/pdf";
  
  // Default MIME type
  return "application/octet-stream";
}
