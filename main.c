#include <stdio.h>
#include "lib/http/http.h"
#include "main.h"

int main() {
    printf("Hi\n\n");
    getHTMLBody("https://www.google.com");
    printf("\n\n\n");


}