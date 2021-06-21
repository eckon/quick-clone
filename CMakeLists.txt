cmake_minimum_required(VERSION 3.5)

project(quick-clone)

file(GLOB filepaths src/*.cpp)
file(GLOB filepaths2 src/**/*.cpp)
add_executable(qc ${filepaths} ${filepaths2})

target_link_libraries(qc ncurses curl)

set_target_properties(qc
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY lib
  LIBRARY_OUTPUT_DIRECTORY lib
  RUNTIME_OUTPUT_DIRECTORY bin
)
