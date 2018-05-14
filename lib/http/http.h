/**
    * Header file to lists structs and functions related to get response from scrape URL
*/


#ifndef WEBSCRAPER_HTTP_H
#define WEBSCRAPER_HTTP_H


#include "../types/types.h
/**
    * Stores IP information of a domain after resolution
*/
struct domaininfo hints, *res;

/**
 *
 * @param host- hostname of website to access
 * @param path- path after hostname
 * @return char*- formed HTTP request header (GET request). Assume $path as path and $host as host, 'GET $path HTTP/1.1 Host: $host' Headers are appended in format 'Header:Value'
 */
char *craftRequestHeader(const char *host, const char *path);


/**
 *
 * @param URL- URL to be retrieved
 * @return Response from HTTP/HTTPS request to URL
 */
char *getHTMLBody(const char *URL, bool ssl);


#endif //WEBSCRAPER_HTTP_H




