/**
 * Header file to list functions that mess with user input
 */

#ifndef WEBSCRAPER_MAIN_INPUTS_H
#define WEBSCRAPER_MAIN_INPUTS_H

#include "../types/types.hpp"

/**
 * @param host- Request host
 * @param path- Request path
 * @param ssl- whether to use plain http or negotiate an SSL cert
 */
typedef struct requestHeaderInfo {
    char *host;
    char *path;
    bool ssl;
} REQUEST_HEADER_INFO;

/**
         * Gets user input for URL to scrape.
         * TODO: Validate input URL
         *
         * @return char*
         * Returns input URL to scrape
         */
char* getScrapeLink();

/**
 *
 * Frees a request header info struct
 */
void freeRequestHeaderInfo(REQUEST_HEADER_INFO* req);
/**
 * Extracts info of request header from URL
 * @param URL- input the user's URL
 * @return struct requestHeaderInfo after extracting relevant information
 */
REQUEST_HEADER_INFO* extractRequestDataFromURL(char* URL);


/**
 *
 * @param URL- user input url
 * @return verification boolean
 */
bool verifyURL(char* URL);

#endif //WEBSCRAPER_MAIN_INPUTS_H
