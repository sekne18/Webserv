# Webserv

Simple web server in C++.
Supports **GET**, **POST** and **DELETE**

## Initialiation
make
<be />
./webserv

## Server
A successful server initialization should display something similar to this output:
```
Loading configuration from: config/default.conf
Starting server on port 8080
Server initiated on port: 8080
Socket created successfully.
Socket bound to port 8080
Server is listening on port 8080...
Epoll set up successfully.
```
## Client
Client can send 3 different requests (GET, POST, DELETE).

### GET (with curl example)

#### Retrieving a default html page
curl http://localhost:8080
OR
curl -v http://localhost:8080 -> To get back the header, etc.

#### Executing a CGI script
curl -v http://localhost:8080/cgi-bin/campus19.py

### POST (with curl example)

#### uploading a file
curl -v -X POST http://localhost:8080/upload -F "file=@test.txt"

### DELETE (with curl example)
curl -v -X DELETE http://localhost:8080/uploads/test.txt
