cmake_minimum_required(VERSION 3.22)
project(luscbackend)

OPTION(BUILD_TEST "build testing?" OFF)
IF(BUILD_TEST)
        add_compile_definitions(LUSC_TESTING)
ENDIF(BUILD_TEST)
unset(BUILD_TEST CACHE)

set(CMAKE_CXX_STANDARD 20)
find_package( fmt REQUIRED )

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package( Boost REQUIRED COMPONENTS filesystem )
include_directories(${Boost_INCLUDE_DIRS})
find_package( Catch2 REQUIRED )
find_package( pybind11 REQUIRED )

set(PY_C_FLAGS "-I/opt/homebrew/opt/python@3.10/Frameworks/Python.framework/Versions/3.10/include/python3.10 -I/opt/homebrew/opt/python@3.10/Frameworks/Python.framework/Versions/3.10/include/python3.10 -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -dynamic -DDEBUG -g -fwrapv -O0 -Wall -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX12.sdk")
set(PY_L_FLAGS "-L/opt/homebrew/opt/python@3.10/Frameworks/Python.framework/Versions/3.10/lib/python3.10/config-3.10-darwin -ldl -framework CoreFoundation")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)

add_library(luscbackend
        backend.h
        backend.cpp
        wrapper.h
        wrapper.cpp
        pyek.h
        pyek.cpp
        cppmain.cpp

        tests/testmain.cpp)

target_compile_options(luscbackend PRIVATE ${PY_C_FLAGS})
target_include_directories( luscbackend PRIVATE
        /opt/homebrew/opt/python@3.10/Frameworks/Python.framework/Versions/3.10/include/python3.10 )

add_definitions(${PY_C_FLAGS} ${PY_L_FLAGS})

target_link_libraries(
        luscbackend
        PRIVATE

        -static-libgcc
        -static-libstdc++

        fmt
        ${Boost_LIBRARIES}
        Catch2::Catch2
        pybind11::embed
)
add_compile_definitions(FMT_HEADER_ONLY)
