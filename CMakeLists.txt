cmake_minimum_required(VERSION 3.2)
project(stem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(stem ${SOURCE_FILES} src/stemcell/Model.cpp src/stemcell/Model.h src/stemcell/elements/Element.cpp src/stemcell/elements/Element.h src/stemcell/limits/Limit.cpp src/stemcell/limits/Limit.h)

# GiNaC and CLN
find_library(LIBCLN cln)
find_library(LIBGINAC ginac)

# xml2
find_package(LibXml2)
include_directories(${LIBXML2_INCLUDE_DIR})

target_link_libraries(stem cln ginac xml2)


