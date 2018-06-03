#include <malloc.h>
#include <string.h>
#include "scraper.hpp"
#include "../user_input/user_input.hpp"

Element *extractedElements[20000];


/**
 * Accepts input as html code (assumes validated). The elementLevel is equal to the DOM level of the current tag (not including the <html></html> tags).
 * For example, a link tag inside a head tag would have a level of 1.
 */
void parseHTML(const char *html) {
    html = strstr(html, "<body");
    int elementLevel = 0;
    int elementIndex = 0;

    int curCharacterIndex = 0;

    auto *curTagName = static_cast<char *>(malloc(DEFAULT_TAG_NAME_SIZE));
    auto *curTagContent = static_cast<char *>(malloc(DEFAULT_TAG_CONTENT_SIZE));
    auto *curTagAttr = static_cast<char *>(malloc(DEFAULT_TAG_ATTR_SIZE));


    int curTagNameIndex = 0;
    int curTagContentIndex = 0;
    int curTagAttrIndex = 0;

    auto *curElement = static_cast<Element *>(malloc(sizeof(Element)));

    auto responseLength = (int) strlen(html);
    while (curCharacterIndex < responseLength && html[curCharacterIndex] != '\0') {
        if (html[curCharacterIndex] == '<') {
            if (html[curCharacterIndex + 1] == '/') { //We are at an ending tag
                while (html[curCharacterIndex] != '>') {
                    ++curCharacterIndex;
                }
                ++curCharacterIndex; //index is at the > right now and we want it to be after
                --elementLevel; //Go up one element level



            } else if (html[curCharacterIndex + 1] == '!') {
                //Need to ignore comments for now, skip to next opening tag
                ++curCharacterIndex;
                while(curCharacterIndex < responseLength && html[curCharacterIndex] != '<')
                    ++curCharacterIndex;
            } else {
                ++curCharacterIndex; //ignore starting tag
                while (html[curCharacterIndex] != ' ') { //Tag name ends with space, then comes attributes
                    if (html[curCharacterIndex] == '>') //We have reached the end of the tag with no attributes
                        break;
                    *(curTagName + curTagNameIndex) = html[curCharacterIndex];
                    ++curTagNameIndex;
                    if (curTagNameIndex + 1 > strlen(curTagName))
                        curTagName = doubleSize(curTagName);
                    ++curCharacterIndex;
                }
                *(curTagName + curTagNameIndex) = '\0';
                curElement->tagName = curTagName;
                curElement->tagLevel = elementLevel;

                //Reset cur variables
                curTagAttrIndex = 0;
                curTagContentIndex = 0;
                curTagNameIndex = 0;

                curTagAttr = static_cast<char *>(malloc(DEFAULT_TAG_ATTR_SIZE));
                curTagContent = static_cast<char *>(malloc(DEFAULT_TAG_CONTENT_SIZE));
                curTagName = static_cast<char *>(malloc(DEFAULT_TAG_NAME_SIZE));

                //Add curElement to array of elements
                extractedElements[elementIndex] = curElement;
                ++elementIndex;

                curElement = static_cast<Element *>(malloc(sizeof(Element)));


                while (html[curCharacterIndex] !=
                       '>') //Get rid of any characters unnecessary and before the end of the opening tag
                    ++curCharacterIndex;
                ++elementLevel;

            }
        } else{
            ++curCharacterIndex;
        }
    }


}


char *doubleSize(char *charPointer) {
    return (char *) realloc(charPointer, strlen(charPointer));
}

char *doubleSizeArray() {

}

void searchElement(char *tagName, char **tagAttr) {

}

void freeElement(Element *element) {
    free(element->tagAttr);
    free(element->tagContent);
    free(element->tagName);
    free(element);
}

void freeAll() {
    int index;
    for (index = 0; index < sizeof(extractedElements) / sizeof(extractedElements[0]); ++index) {
        Element *e = extractedElements[index];
        freeElement(e);
    }
}

