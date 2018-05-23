//
// Created by ytpillai on 5/13/18.
//

#ifndef WEBSCRAPER_SCRAPER_H
#define WEBSCRAPER_SCRAPER_H

typedef struct Element {
    char *tagName;
    char *tagContent;
    int tagLevel;
    char *tagAttr[];
} Element;

extern Element *extractedElements[];

void parseHTML(char *html);
static void addElement(Element *element);
void searchElement(char *tagName, char *tagAttr[]);
void freeAll();



#endif //WEBSCRAPER_SCRAPER_H
