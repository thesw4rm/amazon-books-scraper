cmake_minimum_required(VERSION 3.10)
project(WebScraper)

set(CMAKE_CXX_STANDARD 17)
add_executable(MAINEXE main.cpp main.hpp lib/http/http.cpp lib/scraper/scraper.cpp lib/user_input/user_input.cpp)


target_link_libraries(MAINEXE ssl)
target_link_libraries(MAINEXE tidy)







