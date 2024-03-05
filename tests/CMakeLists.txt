include(ExternalProject)
find_package(Git REQUIRED)

# ExternalProject_Add(
#     doctest
#     PREFIX ${CMAKE_BINARY_DIR}/doctest
#     GIT_REPOSITORY https://github.com/doctest/doctest.git
#     TIMEOUT 20
#     UPDATE_COMMAND ${GIT_EXECUTABLE} pull
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND ""
#     INSTALL_COMMAND ""
#     LOG_DOWNLOAD ON
# )

# # Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
# ExternalProject_Get_Property(doctest source_dir)
# set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")

# include(FetchContent)
# FetchContent_Declare(
#     doctest
#     GIT_REPOSITORY https://github.com/onqtam/doctest.git
#     GIT_TAG        2.4.6
# )
# FetchContent_MakeAvailable(doctest)

# find_package(Doctest REQUIRED)

find_package(GTest REQUIRED)
include(GoogleTest)


# add_compile_options(--coverage)

include_directories(${DOCTEST_INCLUDE_DIR})
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/inc
)


################################################################

# set(CMAKE_CROSSCOMPILING_EMULATOR "${CMAKE_SOURCE_DIR}/run-qemu.sh" CACHE FILEPATH "Path to the emulator for the target system.")


################################################################

# add_executable(tests test_myCobot.cpp ${CMAKE_SOURCE_DIR}/src/GPIO.cpp)
add_executable(tests test_myCobot.cpp)

add_test(NAME all_tests COMMAND tests)
# add_test(NAME all_tests COMMAND ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:tests> --nographic)


################################################################

# target_link_libraries(tests PRIVATE --coverage GTest::gtest_main pthread)
target_link_libraries(tests GTest::gtest_main pthread)


################################################################

# doctest_discover_tests(tests)
gtest_discover_tests(tests)