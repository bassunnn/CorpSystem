cmake_minimum_required(VERSION 3.10)

project(TimeTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(time_tracker)

target_sources(time_tracker
    PRIVATE
        src/main.cpp
        src/Task.cpp
        src/TimeSession.cpp
        src/TaskManager.cpp
        src/TimeTracker.cpp
        src/Storage.cpp
        src/ReportService.cpp
        src/ConsoleUI.cpp
)

target_include_directories(time_tracker
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
