#include <stdio.h>
#include <malloc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <openssl/ssl.h>
#include "http.hpp"

char *craftRequestHeader(REQUEST_HEADER_INFO *request_header_info) {
    auto *requestHeader = static_cast<char *>(calloc(REQUEST_HEADER_MAX_LENGTH,
                                                     sizeof(char))); //Create empty request header to fill with snprintf


    snprintf(requestHeader, REQUEST_HEADER_MAX_LENGTH, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", request_header_info->path,
             request_header_info->host);

    printf(ANSI_COLOR_GREEN "\nLOG: Request header is %s" ANSI_COLOR_RESET, requestHeader);
    return requestHeader;
}

char *noSSLRequest(REQUEST_HEADER_INFO *request_header_info);

int domainToIP(char *domainName, char *ip_string);

char *SSLRequest(REQUEST_HEADER_INFO *request_header_info);


char *getHTMLBody(char *URL) {
    REQUEST_HEADER_INFO *request_header_info = extractRequestDataFromURL(getScrapeLink());
    if (request_header_info->ssl)
        return SSLRequest(request_header_info);
    else
        return noSSLRequest(request_header_info);
}

char *SSLRequest(REQUEST_HEADER_INFO *request_header_info) {
    struct sockaddr_in serverAddress{};
    char *requestHeader;
    unsigned short serverPort;
    char serverIP[16];
    domainToIP(request_header_info->host, serverIP);
    auto *response = static_cast<char *>(calloc(0, 0));
    ssize_t bytesReceived = 0;


    int sockFD; //Only supporting IPV4 right now, returns file descriptor for socket

    if ((sockFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < -1) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not open socket in http.c getHTMLBody(). Reason for error %s", strerror(errno));
        exit(-1);
    }
    int optionState = 1;
    setsockopt(sockFD, IPPROTO_TCP, TCP_NODELAY, &optionState, sizeof(optionState));
    printf(ANSI_COLOR_GREEN "LOG: Socket file descriptor is %d" ANSI_COLOR_RESET, sockFD);
    serverPort = 443;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    inet_aton(serverIP, &serverAddress.sin_addr);

    if (connect(sockFD, (const struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not connect socket in http.c SSLRequest(). Reason for error %s",
                strerror(errno));
        exit(-1);
    }

    printf(ANSI_COLOR_GREEN "\nLOG: Connected socket at descriptor %d to IP %s and port %d" ANSI_COLOR_RESET, sockFD,
           serverIP, serverPort);


    requestHeader = craftRequestHeader(request_header_info);
    SSL_load_error_strings();
    SSL_library_init();
    SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());

    SSL *conn = SSL_new(ssl_ctx);
    SSL_set_fd(conn, sockFD);
    int errCode;
    if ((errCode = SSL_connect(conn)) != 1) {
        fprintf(stderr,
                ANSI_COLOR_RED "Error: could not negotiate SSL connection %i. Reason for error %s" ANSI_COLOR_RESET,
                errCode,
                strerror(SSL_get_error(conn, errCode)));
        close(sockFD);

        exit(-1);
    }
    errCode = SSL_write(conn, requestHeader, (int) strlen(requestHeader));
    if (SSL_get_error(conn, errCode) != 0) {
        fprintf(stderr, ANSI_COLOR_RED "Error: could not send request %i. Reason for error %i" ANSI_COLOR_RESET,
                errCode,
                SSL_get_error(conn, errCode));
        exit(-1);
    }


    free(requestHeader);
    printf(ANSI_COLOR_GREEN "\nLOG: Sent HTTPS request from socket at descriptor %d to IP %s and port %d." ANSI_COLOR_RESET,
           sockFD,
           serverIP, serverPort);
    printf(ANSI_COLOR_GREEN "\nLOG: Starting receive operation" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN "\nLOG: Calculating content length" ANSI_COLOR_RESET);
    ssize_t totalBytesInResponse = 0;
    char buffer[RESPONSE_BUFFER_SIZE + 1];
    bytesReceived += SSL_read(conn, buffer, RESPONSE_BUFFER_SIZE);
    buffer[strlen(buffer)] = '\0';
    response = static_cast<char *>(realloc(response, strlen(buffer) * sizeof(char)));
    strcpy(response, buffer); //Append to the end, safe because recv takes care of limiting buffer size

    char *contentLengthStart = strstr(response, "Content-Length:");
    while (!isdigit(*contentLengthStart))
        contentLengthStart++;
    while (isdigit(*contentLengthStart)) {
        totalBytesInResponse *= 10;
        totalBytesInResponse += *contentLengthStart - '0';
        contentLengthStart++;
    }
    totalBytesInResponse *= sizeof(char);
    totalBytesInResponse +=
            (strlen(response) - strlen(strstr(response, "<!DOCTYPE html>"))) * sizeof(char); //Adding size of header
    while (bytesReceived < totalBytesInResponse) {
        bytesReceived += SSL_read(conn, buffer, RESPONSE_BUFFER_SIZE);
        buffer[strlen(buffer)] = '\0';
        response = static_cast<char *>(realloc(response, (strlen(response) + strlen(buffer)) * sizeof(char)));
        strcat(response, buffer); //Append to the end, safe because recv takes care of limiting buffer size
    }

    response = static_cast<char *>(realloc(response, strlen(response) * sizeof(char) + sizeof(char)));
    response[strlen(response)] = '\0';
    char *htmlBodyEnd = strstr(response, "</html>") + 7;
    *htmlBodyEnd = '\0';
    SSL_shutdown(conn);
    printf(ANSI_COLOR_GREEN "\nLOG: Received HTTPS response from socket at descriptor %d from IP %s and port %d.\n\n\n\n\n" ANSI_COLOR_RESET,
           sockFD, serverIP, serverPort);
    if (close(sockFD) < 0) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not close socket in http.c getHTMLBody(). Reason for error %s", strerror(errno));
        exit(-1);
    }
    printf(ANSI_COLOR_GREEN "\nLOG: Closed socket at descriptor %d" ANSI_COLOR_RESET, sockFD);
    freeRequestHeaderInfo(request_header_info);
    response = static_cast<char *>(realloc(response, strlen(response) * sizeof(char) + sizeof(char)));
    *strstr(response, "</html") = '\0';
    return strstr(response, "<head>");
}

/**
 * TODO: Need to fix
 */
char *noSSLRequest(REQUEST_HEADER_INFO *request_header_info) {
    struct sockaddr_in serverAddress{};
    char *requestHeader;
    unsigned short serverPort;
    char serverIP[16];
    domainToIP(request_header_info->host, serverIP);
    auto *response = static_cast<char *>(calloc(0, 0));
    ssize_t bytesReceived = 0;


    int sockFD; //Only supporting IPV4 right now, returns file descriptor for socket

    if ((sockFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < -1) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not open socket in http.c getHTMLBody(). Reason for error %s", strerror(errno));
        exit(-1);
    }
    int optionState = 1;
    setsockopt(sockFD, IPPROTO_TCP, TCP_NODELAY, &optionState, sizeof(optionState));
    printf(ANSI_COLOR_GREEN "LOG: Socket file descriptor is %d" ANSI_COLOR_RESET, sockFD);
    serverPort = 80;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    inet_aton(serverIP, &serverAddress.sin_addr);

    if (connect(sockFD, (const struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not connect socket in http.c getHTMLBody(). Reason for error %s",
                strerror(errno));
        exit(-1);
    }

    printf(ANSI_COLOR_GREEN "\nLOG: Connected socket at descriptor %d to IP %s and port %d" ANSI_COLOR_RESET, sockFD,
           serverIP, serverPort);

    requestHeader = craftRequestHeader(request_header_info);

    if (send(sockFD, requestHeader, strlen(requestHeader), 0) < 0) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not send request. Reason for error %s",
                strerror(errno));
        exit(-1);
    }


    free(requestHeader);
    printf(ANSI_COLOR_GREEN "\nLOG: Sent HTTP request from socket at descriptor %d to IP %s and port %d." ANSI_COLOR_RESET,
           sockFD,
           serverIP, serverPort);
    printf(ANSI_COLOR_GREEN "\nLOG: Starting receive operation" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN "\nLOG: Calculating content length" ANSI_COLOR_RESET);
    ssize_t totalBytesInResponse = 0;
    char buffer[RESPONSE_BUFFER_SIZE + 1];
    bytesReceived += recv(sockFD, buffer, RESPONSE_BUFFER_SIZE, 0);
    buffer[strlen(buffer)] = '\0';
    response = static_cast<char *>(realloc(response, strlen(buffer) * sizeof(char)));
    strcpy(response, buffer); //Append to the end, safe because recv takes care of limiting buffer size

    char *contentLengthStart = strstr(response, "Content-Length:");
    while (!isdigit(*contentLengthStart))
        contentLengthStart++;
    while (isdigit(*contentLengthStart)) {
        totalBytesInResponse *= 10;
        totalBytesInResponse += *contentLengthStart - '0';
        contentLengthStart++;
    }
    totalBytesInResponse *= sizeof(char);
    totalBytesInResponse +=
            (strlen(response) - strlen(strstr(response, "<html>"))) * sizeof(char); //Adding size of header
    while (bytesReceived < totalBytesInResponse) {
        bytesReceived += recv(sockFD, buffer, RESPONSE_BUFFER_SIZE, 0);
        buffer[strlen(buffer)] = '\0';
        response = static_cast<char *>(realloc(response, (strlen(response) + strlen(buffer)) * sizeof(char)));
        strcat(response, buffer); //Append to the end, safe because recv takes care of limiting buffer size
    }

    response = static_cast<char *>(realloc(response, strlen(response) * sizeof(char) + sizeof(char)));
    response[response + strlen(response) - strstr(response, "</html")] = '\0';

    printf(ANSI_COLOR_GREEN "\nLOG: Received HTTP response from socket at descriptor %d from IP %s and port %d.\n\n\n\n\n" ANSI_COLOR_RESET,
           sockFD, serverIP, serverPort);
    if (close(sockFD) < 0) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not close socket in http.c getHTMLBody(). Reason for error %s", strerror(errno));
        exit(-1);
    }
    printf(ANSI_COLOR_GREEN "\nLOG: Closed socket at descriptor %d" ANSI_COLOR_RESET, sockFD);
    freeRequestHeaderInfo(request_header_info);

    return response;
}

int domainToIP(char *domainName, char *ip_string) {
    struct hostent *he;
    struct in_addr **addrlist;
    if ((he = gethostbyname(domainName)) == NULL) {
        // get the host info
        herror("gethostbyname");
        return 1;
    }

    addrlist = (struct in_addr **) he->h_addr_list;
    int i;
    for (i = 0; addrlist[i] != NULL; i++) {
        //Return the first one;
        strcpy(ip_string, inet_ntoa(*addrlist[i]));
        return 0;
    }
    ip_string[strlen(ip_string)] = '\0';

    return 1;

}




