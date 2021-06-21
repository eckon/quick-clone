cmake_minimum_required(VERSION 3.5)

project(quick-clone)

add_compile_options(-Wall -Wextra -Wpedantic)

file(GLOB folderPath src/*.cpp)
file(GLOB subFolderPath src/**/*.cpp)
add_executable(qc ${folderPath} ${subFolderPath})

target_link_libraries(qc ncurses curl)

set_target_properties(qc
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY lib
  LIBRARY_OUTPUT_DIRECTORY lib
  RUNTIME_OUTPUT_DIRECTORY bin
)
