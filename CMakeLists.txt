cmake_minimum_required(VERSION 3.10)
project(WebScraper C)

set(CMAKE_C_STANDARD 11)
add_executable(WEBSCRAPER main.c main.h lib/http/http.c lib/scraper/scraper.c lib/user_input/user_input.c)



include_directories(/home/CLionProjects/WebScraper/)





