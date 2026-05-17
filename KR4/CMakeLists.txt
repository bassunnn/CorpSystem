cmake_minimum_required(VERSION 3.14)

project(TimeTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(time_tracker_lib)

target_sources(time_tracker_lib
    PRIVATE
        src/Task.cpp
        src/TimeSession.cpp
        src/TaskManager.cpp
        src/TimeTracker.cpp
        src/Storage.cpp
        src/ReportService.cpp
        src/ConsoleUI.cpp
)

target_include_directories(time_tracker_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(time_tracker src/main.cpp)
target_link_libraries(time_tracker PRIVATE time_tracker_lib)

enable_testing()
add_subdirectory(tests)
