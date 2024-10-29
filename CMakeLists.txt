cmake_minimum_required(VERSION 3.29)
project(MyDepTree)

set(CMAKE_CXX_STANDARD 23)


find_package(CURL REQUIRED)

add_executable(MyDepTree main.cpp
        utils/InternetWorker.h
        utils/Indicators.cpp
        utils/Indicators.h)

target_link_libraries(MyDepTree PRIVATE CURL::libcurl)
