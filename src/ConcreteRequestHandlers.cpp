/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteRequestHandlers.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmol <fmol@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:52:16 by fmol              #+#    #+#             */
/*   Updated: 2025/04/29 20:07:46 by fmol             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConcreteRequestHandlers.hpp"

RedirectHandler::RedirectHandler(size_t code, const std::string &path)
    : _code(code), _path(path)
{
}

RedirectHandler::~RedirectHandler()
{
}

IResponse *RedirectHandler::handle(IRequestParser &request, IHandlerContext *ctx)
{
    (void)request;
    (void)ctx;
    if (_code >= 300 && _code < 400)
    {
        IResponse *response = new ConcreteResponse(_code, "redirect");
        response->addHeader("Location", _path);
        return response;
    }
    else
        return new ConcreteResponse(_code, getGenericStatusMessage(_code));
}

CgiHandler::CgiHandler(const std::string &extension)
    : _extension(extension)
{
}

CgiHandler::~CgiHandler()
{
}

IResponse *CgiHandler::execute(IRequestParser &request, IHandlerContext *ctx)
{
    (void)request;
    (void)ctx;

    std::vector<std::string> envStrings;
    envStrings.push_back("REQUEST_METHOD=" + request.getMethod());
    if (request.getMethod() == "POST")
    {
        std::multimap<std::string, std::string> headers = request.getHeaders();
        std::multimap<std::string, std::string>::iterator it = headers.find("Content-Type");
        if (it != headers.end())
        {
            envStrings.push_back("CONTENT_TYPE=" + it->second);
        }
        return new ConcreteResponse(400, "Bad Request");
    }
    else if (request.getMethod() == "GET")
    {
        // envStrings.push_back("QUERY_STRING=" + request.getQuery());
    }
    envStrings.push_back("SERVER_SOFTWARE=WebServ/1.0");
    envStrings.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envStrings.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envStrings.push_back("CONTENT_LENGTH=" + toString(request.getBody().size()));

    // Convert to char* array for execve
    char **envp = new char *[envStrings.size() + 1];
    for (size_t i = 0; i < envStrings.size(); i++)
    {
        envp[i] = const_cast<char *>(envStrings[i].c_str());
    }
    envp[envStrings.size()] = NULL; // NULL terminator

    int in[2]; //[1] = write, [0] = read
    int out[2];

    if (pipe(in) == -1)
        return new ConcreteResponse(500, "Internal Server Error");
    if (pipe(out) == -1)
    {
        close(in[0]);
        close(in[1]);

        return new ConcreteResponse(500, "Internal Server Error");
    }
    int pid = fork();
    if (pid == -1)
    {
        close(in[0]);
        close(in[1]);
        close(out[0]);
        close(out[1]);

        return new ConcreteResponse(500, "Internal Server Error");
    }
    else if (pid == 0)
    {
        dup2(in[0], STDIN_FILENO);
        dup2(out[1], STDOUT_FILENO);
        close(in[0]);
        close(in[1]);
        close(out[0]);
        close(out[1]);

        std::string target = request.getTarget();
        if (target[0] == '/')
            target.erase(0, 1);
        char *args[2] = {const_cast<char *>(target.c_str()), NULL};
        execve(args[0], args, envp);
        throw std::runtime_error("execve failed");
    }
    else
    {
        close(in[0]);
        close(out[1]);
        write(in[1], request.getBody().c_str(), request.getBody().size());
        close(in[1]);

        std::string output;
        char buffer[1024];
        ssize_t bytesRead;

        while ((bytesRead = read(out[0], buffer, sizeof(buffer))) > 0)
            output.append(buffer, bytesRead);
        close(out[0]);
        IResponse *response = 0;
        if (bytesRead == -1)
        {
            response = new ConcreteResponse(500, "Internal Server Error");
        }
        if (output.empty())
        {
            response = new ConcreteResponse(204, "No Content");
        }
        else
        {
            response = new ConcreteResponse(200, "OK");
            response->setBody(output);
            response->addHeader("Content-Type", "text/html");
        }
        waitpid(pid, NULL, 0);
        return response;
    }
    return new ConcreteResponse(500, "Internal Server Error");
}

IResponse *CgiHandler::handle(IRequestParser &request, IHandlerContext *ctx)
{
    (void)request;
    (void)ctx;
    if (request.getMethod() != "GET" || request.getMethod() != "POST")
        return new ConcreteResponse(405, "Method Not Allowed");
    if (request.getTarget().find(_extension) == std::string::npos)
        return new ConcreteResponse(415, "Unsupported Media Type");
    struct stat st;
    if (stat(request.getTarget().c_str(), &st) == -1)
    {
        int error = errno;
        if (error == ENOENT)
            return new ConcreteResponse(404, "Not Found");
        else if (error == EACCES)
            return new ConcreteResponse(403, "Forbidden");
        else
            return new ConcreteResponse(500, "Internal Server Error");
    }
    else if (S_ISREG(st.st_mode))
    {
        if (st.st_mode & S_IXUSR)
            return execute(request, ctx);
        else
            return new ConcreteResponse(403, "Forbidden");
        return new ConcreteResponse(200, "OK");
    }
    return new ConcreteResponse(400, "Bad Request");
}

StaticFileHandler::StaticFileHandler()
{
}

StaticFileHandler::~StaticFileHandler()
{
}

IResponse *StaticFileHandler::handle(IRequestParser &request, IHandlerContext *ctx)
{
    if (request.getMethod() == "GET")
        return handleGET(request, ctx);
    else if (request.getMethod() == "POST")
        return handlePOST(request, ctx);
    else if (request.getMethod() == "DELETE")
        return handleDELETE(request, ctx);
    else
        return new ConcreteResponse(405, "Method Not Allowed");
}

IResponse *StaticFileHandler::handleGET(IRequestParser &request, IHandlerContext *ctx)
{
    (void)request;
    (void)ctx;
    if (isDirectory(request.getTarget()))
        return 0;
    std::ifstream file(request.getTarget().c_str(), std::ios::in);
    if (!file.is_open())
    {
        int error = errno;
        if (file.bad())
        {
            return new ConcreteResponse(500, "Internal Server Error");
        }
        if (file.fail())
        {
            if (error == ENOENT)
            {
                return new ConcreteResponse(404, "Not Found");
            }
            else if (error == EACCES)
            {
                return new ConcreteResponse(403, "Forbidden");
            }
        }
        return new ConcreteResponse(400, "Bad request");
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    IResponse *response = new ConcreteResponse(200, "OK");
    response->setBody(content);
    response->addHeader("Content-Type", "text/html");
    return response;
}

IResponse *StaticFileHandler::handlePOST(IRequestParser &request, IHandlerContext *ctx)
{
    (void)request;
    (void)ctx;
    if (isDirectory(request.getTarget()))
        return new ConcreteResponse(409, "Conflict");
    std::ofstream file(request.getTarget().c_str(), std::ios::out | std::ios::trunc);
    if (!file.is_open())
    {
        int error = errno;
        if (file.bad())
        {
            return new ConcreteResponse(500, "Internal Server Error");
        }
        if (file.fail())
        {
            if (error == ENOENT)
            {
                return new ConcreteResponse(404, "Not Found");
            }
            else if (error == EACCES)
            {
                return new ConcreteResponse(403, "Forbidden");
            }
        }
        return new ConcreteResponse(400, "Bad request");
    }
    file << request.getBody();
    if (file.bad())
    {
        return new ConcreteResponse(500, "Internal Server Error");
    }
    file.close();
    return new ConcreteResponse(200, "OK");
}

IResponse *StaticFileHandler::handleDELETE(IRequestParser &request, IHandlerContext *ctx)
{
    (void)request;
    (void)ctx;
    if (isDirectory(request.getTarget()))
        return new ConcreteResponse(409, "Conflict");
    if (remove(request.getTarget().c_str()) != 0)
    {
        int error = errno;
        if (error == ENOENT)
        {
            return new ConcreteResponse(404, "Not Found");
        }
        else if (error == EACCES)
        {
            return new ConcreteResponse(403, "Forbidden");
        }
        return new ConcreteResponse(500, "Internal Server Error");
    }
    return new ConcreteResponse(200, "OK");
}