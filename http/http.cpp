#include "Http.h"
extern "C" {
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
}
#include <iostream>
#include <string>
#include <unistd.h>

#define BUFFER_SIZE 80000

void Http::connect(std::string url) {
    extern int h_errno;
    const char *nameC = url.c_str();
    struct hostent host = *gethostbyname(nameC);
    if (h_errno != 0) std::cout<<"gethostbyname failure\n";

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    bcopy((char*)host.h_addr, (char*)&addr.sin_addr.s_addr, host.h_length);

    Http::sck = socket(AF_INET, SOCK_STREAM, 0);
    if (Http::sck == -1) std::cout<<"socket creation failure\n";
    if (::connect(Http::sck, (const struct sockaddr*)&addr, sizeof(addr)) != 0) {
        std::cout<<"connection failure\n";
    } else {
        std::cout<<"successful connection\n";
    }
}

std::string Http::makeRequest(const char *req) {
    send(Http::sck, req, std::string(req).length(), 0);
    const size_t cBUFFER_SIZE = 80192;
    size_t BUFFER_SIZE = ;
    char buffer[(int)BUFFER_SIZE];
    std::string response = "";
    std::cout<<recv(Http::sck, &buffer, (size_t)BUFFER_SIZE, 0);
    std::cout<<buffer;
    response = std::string(buffer);
    char last = buffer[0];
    int i;
    for (i = 1; i < BUFFER_SIZE; i++)
        if (last == '\n' && buffer[i] == '\n') break;
    response.erase(0, i + 1);
    std::cout<<response<<"------------------------------------------------------\n";
    return response;
}
