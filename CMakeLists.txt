cmake_minimum_required(VERSION 3.10)
project(http_server)
set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
    "*.cpp"
)

add_executable(http_server ${SOURCES})

target_include_directories(http_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(http_server pthread) 