cmake_minimum_required(VERSION 3.29)
project(MyDepTree)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXE_LINKER_FLAGS "-static")

find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)



add_executable(MyDepTree main.cpp
        utils/InternetWorker.h
        utils/Indicators.cpp
        utils/Indicators.h
        utils/JsonManager.cpp
        utils/JsonManager.h)

target_link_libraries(MyDepTree PRIVATE CURL::libcurl)
target_link_libraries(MyDepTree PRIVATE nlohmann_json::nlohmann_json)
