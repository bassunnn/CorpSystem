include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.3
)
FetchContent_MakeAvailable(Catch2)

add_executable(unit_tests
    test_task.cpp
    test_time_session.cpp
    test_task_manager.cpp
    test_time_tracker.cpp
    test_storage.cpp
    test_report_service.cpp
    test_console_ui.cpp
)

target_link_libraries(unit_tests
    PRIVATE
        time_tracker_lib
        Catch2::Catch2WithMain
)

set_target_properties(unit_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

add_test(NAME unit_tests COMMAND unit_tests)

add_executable(scenario_basic_workflow scenarios/scenario_basic_workflow.cpp)
target_link_libraries(scenario_basic_workflow PRIVATE time_tracker_lib)
set_target_properties(scenario_basic_workflow PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/scenarios
)

add_executable(scenario_persistence scenarios/scenario_persistence.cpp)
target_link_libraries(scenario_persistence PRIVATE time_tracker_lib)
set_target_properties(scenario_persistence PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/scenarios
)

add_executable(scenario_reporting scenarios/scenario_reporting.cpp)
target_link_libraries(scenario_reporting PRIVATE time_tracker_lib)
set_target_properties(scenario_reporting PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests/scenarios
)
