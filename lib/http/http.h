/**
    * Header file to lists structs and functions related to get response from scrape URL
*/


#ifndef WEBSCRAPER_HTTP_H
#define WEBSCRAPER_HTTP_H
#define REQUEST_HEADER_MAX_LENGTH 3000

#include "../types/types.h"


/**
 *
 * @param host- hostname of website to access
 * @param path- path after hostname
 * @return char*- formed HTTP request header (GET request). Assume $path as path and $host as host, 'GET $path HTTP/1.1 Host: $host' Headers are appended in format 'Header:Value'
 */
char* craftRequestHeader(const char *host, const char *path);


/**
 *
 * @param URL- URL to be retrieved
 * @param bool- whether ssl should be used
 * @return HTML body of response from HTTP/HTTPS request to URL
 */
char* getHTMLBody(const char *URL, bool ssl);


#endif //WEBSCRAPER_HTTP_H




