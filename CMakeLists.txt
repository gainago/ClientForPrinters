cmake_minimum_required(VERSION 3.8)

project(main)
file(GLOB
                SOURCES "${PROJECT_SOURCE_DIR}/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(json-c CONFIG)

target_link_libraries(${PROJECT_NAME} PRIVATE json-c::json-c)

find_package(CURL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)