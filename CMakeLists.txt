cmake_minimum_required(VERSION 3.22)
project(SerialPortProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost 1.65.1 REQUIRED COMPONENTS system)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Wconversion )
# add_compile_options(-Werror)
# add_compile_options(-fsanitize=undefined)

endif()

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

include_directories(
   ${PROJECT_SOURCE_DIR}/inc
)



# ---- Create executable ----

add_executable(SerialPortProject ${sources})

target_link_libraries(SerialPortProject Boost::system)

if(APPLE)
  # macOS-specific configuration
  message(STATUS "Building for macOS")
  target_compile_definitions(SerialPortProject PRIVATE MACOS)
else()
  # Non-macOS configuration
  message(STATUS "Not building for macOS")
endif()

# optional testing

if (EXISTS ${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt)
  enable_testing()
  add_subdirectory(tests)
endif()
