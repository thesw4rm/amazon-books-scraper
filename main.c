#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <zconf.h>
#include "lib/http/http.h"
#include "main.h"

int main() {
    printf("Hi\n\n");
    printf("%ld", (long)getpid());
    char ha[11];
    fgets(ha, 10, stdin);

    printf(getHTMLBody(getScrapeLink()));
    printf("\n\n\n");


}