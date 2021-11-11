cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(HA)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set (CMAKE_CXX_STANDARD 11)

if( ${CMAKE_BINARY_DIR} STREQUAL ${PROJECT_SOURCE_DIR} )
  Message( FATAL_ERROR "Source and build directories are the same!")
endif()

find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} -O0")

set(CMAKE_EXE_LINKER_FLAGS  
    "${CMAKE_EXE_LINKER_FLAGS}")

file(GLOB SOURCES "src/*.cpp")

set(PROJECT_SOURCES
    ${SOURCES} 
)

set(PROJECT_INCLUDES
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/lib/include
)

include_directories(${PROJECT_INCLUDES})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} 
                      pthread
                      )

option(X_LIB "Activates usage of X11 library"  ON)
option(CIMG_LIB "Activates usage of png library" ON)

if(${CIMG_LIB})
    message(STATUS "Looking for libpng ...")

    # Define a prepro variable to control the c++ code
    add_definitions(-DCIMG_LIB)

    # Flags for Cool Image to enable native support for PNG image files, using the PNG library.
    set(CIMG_PNG_CCFLAGS  -Dcimg_use_png)

    # More details in third lecture
    find_package(PNG REQUIRED)

    #
    include_directories(${PNG_INCLUDE_DIR})
    target_link_libraries (${PROJECT_NAME} ${PNG_LIBRARY})
    target_compile_definitions(${PROJECT_NAME} PRIVATE cimg_use_png=1)
endif()

if(${X_LIB})
    message(STATUS "Looking for libx11 ...")

    add_definitions(-DX_LIB)
    find_package(X11)
    target_link_libraries(${PROJECT_NAME} X11)
endif()

if(LOG)
    add_definitions(-DLOGGING)
endif()

include(run)
include(gdb)
include(deliver)

set_property (DIRECTORY PROPERTY ADDITIONAL_CLEAN_FILES
    "${PROJECT_SOURCE_DIR}/build/CMakeFiles"
    "${PROJECT_SOURCE_DIR}/build/CMakeCache.txt"
    "${PROJECT_SOURCE_DIR}/build/Makefile"
    "${PROJECT_SOURCE_DIR}/build/cmake_install.cmake"
    "${PROJECT_SOURCE_DIR}/build/compile_commands.json"
    )
