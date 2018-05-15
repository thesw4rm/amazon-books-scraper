#include <stdio.h>
#include <malloc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>
#include "http.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

char *craftRequestHeader(REQUEST_HEADER_INFO *request_header_info) {
    char *requestHeader = calloc(REQUEST_HEADER_MAX_LENGTH,
                                 sizeof(char)); //Create empty request header to fill with snprintf


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
    if (*(request_header_info->ssl))
        return SSLRequest(request_header_info);
    else
        return noSSLRequest(request_header_info);
}

char *SSLRequest(REQUEST_HEADER_INFO *request_header_info) {
    struct sockaddr_in serverAddress;
    char *requestHeader;
    unsigned short serverPort;
    char serverIP[13];
    domainToIP(request_header_info->host, serverIP);
    char *response = calloc(0, 0);
    ssize_t bytesReceived = 0;


    int sockFD; //Only supporting IPV4 right now, returns file descriptor for socket

    if ((sockFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < -1) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not open socket in http.c getHTMLBody(). Reason for error %s", strerror(errno));
        exit(-1);
    }
    printf(ANSI_COLOR_GREEN "LOG: Socket file descriptor is %d" ANSI_COLOR_RESET, sockFD);
    serverPort = 80;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(80);
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

    printf(ANSI_COLOR_GREEN "\nLOG: Sent HTTP request from socket at descriptor %d to IP %s and port %d." ANSI_COLOR_RESET,
           sockFD,
           serverIP, serverPort);


    free(requestHeader);
    printf(ANSI_COLOR_GREEN "\nLOG: Starting receive operation" ANSI_COLOR_RESET);
    ssize_t bytesReceivedPrevious = -1;
    char buffer[RESPONSE_BUFFER_SIZE];
    while (bytesReceived < (RESPONSE_MAX_LEN * sizeof(char)) && bytesReceived > bytesReceivedPrevious) {
        bytesReceivedPrevious = bytesReceived;
        bytesReceived = recv(sockFD, buffer, RESPONSE_BUFFER_SIZE, 0);
        response = realloc(response, sizeof(response) + RESPONSE_BUFFER_SIZE);
        strcat(response, buffer); //Append to the end, safe because recv takes care of limiting buffer size
    }

    printf(ANSI_COLOR_GREEN "\nLOG: Received HTTP response from socket at descriptor %d to IP %s and port %d." ANSI_COLOR_RESET,
           sockFD,
           serverIP, serverPort);
    if (close(sockFD) < 0) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not close socket in http.c getHTMLBody(). Reason for error %s", strerror(errno));
        exit(-1);
    }
    printf(ANSI_COLOR_GREEN "\nLOG: Closed socket at descriptor %d" ANSI_COLOR_RESET, sockFD);
    freeRequestHeaderInfo(request_header_info);

    return response;
}

/**
 * TODO: Need to fix
 */
char *noSSLRequest(REQUEST_HEADER_INFO *request_header_info) {
    struct sockaddr_in serverAddress;
    char *requestHeader;
    unsigned short serverPort;
    char serverIP[13];
    domainToIP(request_header_info->host, serverIP);
    char *response = calloc(0, 0);
    ssize_t bytesReceived = 0;


    int sockFD; //Only supporting IPV4 right now, returns file descriptor for socket

    if ((sockFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < -1) {
        freeRequestHeaderInfo(request_header_info);
        fprintf(stderr, "Error, could not open socket in http.c getHTMLBody(). Reason for error %s", strerror(errno));
        exit(-1);
    }
    printf(ANSI_COLOR_GREEN "LOG: Socket file descriptor is %d" ANSI_COLOR_RESET, sockFD);
    serverPort = 80;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(80);
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

    printf(ANSI_COLOR_GREEN "\nLOG: Sent HTTP request from socket at descriptor %d to IP %s and port %d." ANSI_COLOR_RESET,
           sockFD,
           serverIP, serverPort);


    free(requestHeader);
    printf(ANSI_COLOR_GREEN "\nLOG: Starting receive operation" ANSI_COLOR_RESET);
    ssize_t bytesReceivedPrevious = -1;
    char buffer[RESPONSE_BUFFER_SIZE];
    while (bytesReceived < (RESPONSE_MAX_LEN * sizeof(char)) && bytesReceived > bytesReceivedPrevious) {
        bytesReceivedPrevious = bytesReceived;
        bytesReceived = recv(sockFD, buffer, RESPONSE_BUFFER_SIZE, 0);
        response = realloc(response, sizeof(response) + RESPONSE_BUFFER_SIZE);
        strcat(response, buffer); //Append to the end, safe because recv takes care of limiting buffer size
    }

    printf(ANSI_COLOR_GREEN "\nLOG: Received HTTP response from socket at descriptor %d to IP %s and port %d.\n" ANSI_COLOR_RESET,
           sockFD,
           serverIP, serverPort);
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

    return 1;

}




