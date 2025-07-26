# c-sockets

Learning how sockets work :3

## Usage

```make server``` to build the server   
```make client``` to build the client   

```./build/bin/server``` to run the server   
```./build/bin/client``` to run the client   


## Notes

Firstly you need to create a server socket, ```socket_fd = socket(domain, type, protocol)``` returns -1 if error

1. Domain
- IPv4 = AF_INET
- IPv6 = AF_INET6

2. Type
- SOCK_STREAM for TCP
- SOCK_DGRAM for UDP

3. Protocol
- 0 

There are many other options available see [socket()](https://www.man7.org/linux/man-pages/man2/socket.2.html)

Then the socket needs to be binded to an IP and port, ```bind(fd, sockaddr, addr_len)``` returns - 1 if error
See more information [bind()](https://www.man7.org/linux/man-pages/man2/bind.2.html) 

After binding the socket can then begin listening on that port through ```listen(fd, backlog)``` returns -1 if error
See more information [listen()](https://www.man7.org/linux/man-pages/man2/listen.2.html) 

Then it can begin accepting clients, ```client_fd accept(fd, sockaddr, addr_len)``` returns -1 if error  
See more information [accept()](https://man7.org/linux/man-pages/man2/accept.2.html) 

Now you can create the client socket, the client socket does not need to bind, listen or accept, it only needs to connect to the server socket, ```connect(fd, sockaddr, addr_len)```   
See more information [connect()](https://www.man7.org/linux/man-pages/man2/connect.2.html) 

After this the two sockets can begin communicating, let your imagination run wild, text, media, you know it you can build it :3 In this basic implementation I made use of [read()](https://man7.org/linux/man-pages/man2/read.2.html) [write()](https://man7.org/linux/man-pages/man2/write.2.html) 

## Sources used

[A Beginners Guide to Socket Programming in C](https://dev.to/sanjayrv/a-beginners-guide-to-socket-programming-in-c-5an5)    
[TCP Server-Client implementation in C](https://www.geeksforgeeks.org/c/tcp-server-client-implementation-in-c/)   
