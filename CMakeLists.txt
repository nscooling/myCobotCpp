cmake_minimum_required(VERSION 3.22)
project(myCobotSimple_Serial)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost 1.65.1 REQUIRED COMPONENTS system)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Wconversion )
    # add_compile_options(-Werror)
    # add_compile_options(-fsanitize=undefined)
    # add_link_options(-fsanitize=undefined)
endif()


# ---- Dependencies ----.
# set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
#   CACHE STRING "Vcpkg toolchain file")
# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

include_directories(
   ${PROJECT_SOURCE_DIR}/inc
)



# ---- Create executable ----

add_executable(myCobotSimple-Serial ${sources})

target_link_libraries(myCobotSimple-Serial Boost::system)

# ---- Create library ----

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("Building for Linuxd")
    # Linux specific code
    set(PLATFORM LINUX)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    message("Building for macOS")
    # macOS specific code
    set(PLATFORM MACOS)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("Windows detected")
    # Windows specific code
endif()

target_compile_definitions(myCobotSimple-Serial PRIVATE ${PLATFORM})

# optional testing

# Conditionally enable testing; to use, pass -DBUILD_TESTING=ON to CMake, e.g.,
# cmake -S . -B build -DBUILD_TESTING=ON
# ctest --test-dir build --output-on-failure
if(BUILD_TESTING)
# New in version 3.29.
# If CMAKE_SKIP_TEST_ALL_DEPENDENCY is explicitly set to FALSE, 
# then the test target will depend on the all target. 
# When the test target is built, e.g., via make test, the all target will be built first, 
# and then the tests will run.
  set(CMAKE_SKIP_TEST_ALL_DEPENDENCY false)
  enable_testing()
  add_subdirectory(tests)
endif()
