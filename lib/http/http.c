#include <stdio.h>
#include <malloc.h>
#include "http.h"


char* craftRequestHeader(const char *host, const char *path){
    char *requestHeader = malloc(sizeof(char) * REQUEST_HEADER_MAX_LENGTH); //Create empty request header to fill with sprintf
    snprintf(requestHeader, REQUEST_HEADER_MAX_LENGTH, "GET %s HTTP/1.1 Host: %s", path, host);
    return requestHeader;
}

/**
 * TODO: Implement function
 */
char* getHTMLBody(const char *URL, bool ssl){
    return "";
}
