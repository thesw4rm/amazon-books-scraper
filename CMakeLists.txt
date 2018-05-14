cmake_minimum_required(VERSION 3.10)
project(WebScraper C)

set(CMAKE_C_STANDARD 11)
add_executable(WebScraper main.c main.h lib/http/http.h lib/scraper/scraper.h lib/types/types.h lib/user_input/user_input.h)
include_directories(/home/CLionProjects/WebScraper/)





