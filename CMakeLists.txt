cmake_minimum_required(VERSION 3.10)
project(WebScraper)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH cmake-build-debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY cmake-build-debug/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY cmake-build-debug/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY cmake-build-debug/bin)
add_executable(MAINEXE main.cpp main.hpp lib/http/http.cpp lib/scraper/scraper.cpp lib/user_input/user_input.cpp)


target_link_libraries(MAINEXE ssl)







