#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "user_input.h"

/**
* TODO: Implement
* @return request inputted URL
*/
char *getScrapeLink() {
    return "http://mirror.vcu.edu/";//"https://www.amazon.com/_/dp/1449355730?tag=oreilly20-20";
}

REQUEST_HEADER_INFO* extractRequestDataFromURL(char* URL) {
    printf(ANSI_COLOR_GREEN "LOG: Extracting request header data...\n");
    char *host;
    char *path;
    bool ssl = false;
    if (!verifyURL(URL)) {
        fprintf(stderr, "Error, URL is invalid");
        exit(-1);
    }

    //Check for HTTPS or HTTP
    char requestType[6];
    strncpy(requestType, URL, 5);
    requestType[5] = '\0'; //Set null terminating character
    if (strcmp(requestType, "https") == 0)
        ssl = true;



    //Extract host.
    char *hostStart = strstr(URL, "://") + 3;
    size_t hostLength = strcspn(hostStart, "/");
    host = malloc(sizeof(char) * hostLength);
    strncpy(host, hostStart, hostLength);

    //Extract path, use localPath as placeholder for mallocing the correct size
    char *localPath = hostStart + hostLength;
    path = malloc(sizeof(char) * strlen(localPath));
    strncpy(path, localPath, sizeof(char) * strlen(localPath));







    printf("\tExtracted host: Host = %s\n", host);
    printf("\tExtracted path: Path = %s\n", path);
    if (ssl)
        printf("\tExtracted ssl: ssl = true\n");
    else
        printf("\tExtracted ssl: ssl = false\n");
    printf(ANSI_COLOR_RESET);
    REQUEST_HEADER_INFO *requestInfo = (REQUEST_HEADER_INFO *) malloc(sizeof(struct requestHeaderInfo));
    requestInfo->host = host;
    requestInfo->path = path;
    requestInfo->ssl = malloc(sizeof(bool));
    *(requestInfo->ssl) = ssl;
    return requestInfo;
}

void freeRequestHeaderInfo(REQUEST_HEADER_INFO* req){
    free(req->path);
    free(req->ssl);
    free(req->host);
    free(req);
}
/**
 * TODO: Implement
 */
bool verifyURL(char *URL) {
    return true;
}