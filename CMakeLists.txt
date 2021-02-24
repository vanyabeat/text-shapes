cmake_minimum_required(VERSION 3.16)

project(shapes-project)

set(CMAKE_CXX_STANDARD 20)

include(ucm/ucm.cmake)


if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()

ucm_add_dirs(src TO SOURCES)

include_directories(src)

add_executable(shapes-project main.cpp ${SOURCES} src/texture.h src/shapes.h src/common.h src/canvas.h)

target_link_libraries(shapes-project ${CONAN_LIBS})
