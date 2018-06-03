//
// Created by ytpillai on 5/13/18.
//

#ifndef WEBSCRAPER_SCRAPER_H
#define WEBSCRAPER_SCRAPER_H

#define DEFAULT_TAG_NAME_SIZE 10
#define DEFAULT_TAG_CONTENT_SIZE 100
#define DEFAULT_TAG_ATTR_SIZE 40


typedef struct Element {
    char *tagName;
    char *tagContent;
    int tagLevel;
    char *tagAttr[];
} Element;

extern Element *extractedElements[];



void parseHTML(const char *html);
char * doubleSize(char *charPointer);
void searchElement(char *tagName, char *tagAttr[]);
void freeAll();



#endif //WEBSCRAPER_SCRAPER_H
