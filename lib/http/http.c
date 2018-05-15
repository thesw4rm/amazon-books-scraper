#include <stdio.h>
#include <malloc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>
#include "http.h"
#include <unistd.h>

char *craftRequestHeader(const char *URL, REQUEST_HEADER_INFO *request_header_info) {
    char *requestHeader = malloc(
            sizeof(char) * REQUEST_HEADER_MAX_LENGTH); //Create empty request header to fill with sprintf

    snprintf(requestHeader, REQUEST_HEADER_MAX_LENGTH, "GET %s HTTP/1.1 Host: %s", request_header_info->path,
             request_header_info->host);
    printf("Request header is %s", requestHeader);
    return requestHeader;
}

/**
 * TODO: Implement function
 */
char *getHTMLBody(const char *URL) {
    int sockAddr = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Only supporting IPV4 right now

    if (sockAddr == -1) {
        fprintf(stderr, "Error, could not open socket in http.c getHTMLBody(). Reason for error %s", strerror(errno));
        exit(-1);
    }
    printf(ANSI_COLOR_GREEN "Reserved socket space file descriptor %d" ANSI_COLOR_RESET, sockAddr);


    if (close(sockAddr) == -1) {
        fprintf(stderr, "Error, could not close socket in http.c getHTMLBody(). Reason for error %s", strerror(errno));
        exit(-1);
    }

    return "";
}
