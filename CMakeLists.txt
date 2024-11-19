cmake_minimum_required(VERSION 3.29)
project(MyDepTree)

set(CMAKE_CXX_STANDARD 23)

find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(libzippp CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(MyDepTree main.cpp
        utils/InternetWorker.h
        utils/Indicators.cpp
        utils/Indicators.h
        utils/JsonManager.cpp
        utils/JsonManager.h
        utils/ZipWorker.h
        utils/AdditionalFileLoader.h
        DepTree/PkgNode.cpp
        DepTree/PkgNode.h
        DepTree/DepTree.h
        DepTree/DepTree.cpp)

target_link_libraries(MyDepTree PRIVATE CURL::libcurl)
target_link_libraries(MyDepTree PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(MyDepTree PRIVATE libzippp::libzippp)


add_executable(TESTs
        test/PlantUmlDepTEST.cpp
        DepTree/PkgNode.cpp
        DepTree/PkgNode.h

        DepTree/DepTree.h
        DepTree/DepTree.cpp
)
target_link_libraries(TESTs PRIVATE GTest::gtest GTest::gtest_main)