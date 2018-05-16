#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include "lib/http/http.h"
#include "main.h"

int main() {
    printf("Hi\n\n");

    printf(getHTMLBody(getScrapeLink()));
    printf("\n\n\n");


}