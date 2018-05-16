/**
    * Header file to lists structs and functions related to get response from scrape URL
*/


#ifndef WEBSCRAPER_HTTP_H
#define WEBSCRAPER_HTTP_H

#include "../user_input/user_input.h"

#define REQUEST_HEADER_MAX_LENGTH 30000
#define RESPONSE_BUFFER_SIZE 6000
#define RESPONSE_MAX_LEN 2000000


/**
 *
 * @param host- hostname of website to access
 * @param path- path after hostname
 * @return char*- formed HTTP request header (GET request). Assume $path as path and $host as host, 'GET $path HTTP/1.1 Host: $host' Headers are appended in format 'Header:Value'
 */
char *craftRequestHeader(REQUEST_HEADER_INFO *request_header_info);


/**
 *
 * @param URL- URL to be retrieved
 * @param bool- whether ssl should be used
 * @return HTML body of response from HTTP/HTTPS request to URL
 */
char *getHTMLBody(char *URL);


#endif //WEBSCRAPER_HTTP_H




