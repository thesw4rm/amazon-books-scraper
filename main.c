#include <stdio.h>
#include <memory.h>
#include "lib/http/http.h"
#include "main.h"

int main() {
    printf("Hi\n\n");

    getHTMLBody(getScrapeLink());
    printf("\n\n\n");


}