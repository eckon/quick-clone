cmake_minimum_required(VERSION 3.10)

project(qc)

add_compile_options(-Wall -Wextra -Wpedantic)

file(GLOB CPP_FILE_PATHS src/*.cpp src/**/*.cpp)
add_executable(${PROJECT_NAME} ${CPP_FILE_PATHS})


target_link_libraries(
  ${PROJECT_NAME}
  ncurses
  curl
)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY lib
  LIBRARY_OUTPUT_DIRECTORY lib
  RUNTIME_OUTPUT_DIRECTORY bin
)
