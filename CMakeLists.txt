cmake_minimum_required(VERSION 3.10)
project(zenith LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find cmark-gfm
find_library(CMARK_GFM_LIB NAMES cmark-gfm libcmark-gfm)
find_path(CMARK_GFM_INCLUDE_DIR cmark-gfm.h)

# Find tinyxml2
find_library(TINYXML2_LIB NAMES tinyxml2 libtinyxml2)
find_path(TINYXML2_INCLUDE_DIR tinyxml2.h)

include_directories(${CMARK_GFM_INCLUDE_DIR} ${TINYXML2_INCLUDE_DIR})

add_executable(zenith
    src/main.cpp
    src/markdown_parser.cpp
    src/page_generator.cpp
    src/rss_generator.cpp
    src/file_utils.cpp
)

target_link_libraries(zenith PRIVATE ${CMARK_GFM_LIB} ${TINYXML2_LIB})