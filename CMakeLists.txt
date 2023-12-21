cmake_minimum_required(VERSION 3.18)
project(Wii-Tac-Toe)

set(CMAKE_CXX_STANDARD 20)

find_library(AESND aesnd
  PATHS "${OGC_ROOT}/lib/${OGC_SUBDIR}"
  REQUIRED
)
find_library(FAT fat
  PATHS "${OGC_ROOT}/lib/${OGC_SUBDIR}"
  REQUIRED
)
find_library(GRRMOD grrmod
  PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib"
  REQUIRED
)
find_library(PNGU pngu
  PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib"
  REQUIRED
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PNG REQUIRED libpng)
pkg_check_modules(JPEG REQUIRED libjpeg)
pkg_check_modules(FREETYPE REQUIRED freetype2)
pkg_check_modules(MXML REQUIRED mxml)

include(FetchContent)
FetchContent_Declare(fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/10.1.1.tar.gz
)
FetchContent_MakeAvailable(fmt)

file(GLOB_RECURSE SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/fonts/*.cpp"
)

file(GLOB_RECURSE BIN_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/audio/*.*"
  "${CMAKE_CURRENT_SOURCE_DIR}/gfx/*.png"
  "${CMAKE_CURRENT_SOURCE_DIR}/languages/*.xml"
)

add_executable(Wii-Tac-Toe)

target_compile_options(Wii-Tac-Toe PRIVATE
  -Werror
  -Wall
  -Wunused
)

target_sources(Wii-Tac-Toe PRIVATE
  ${SRC_FILES}
)

target_include_directories(Wii-Tac-Toe PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/source"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/grrlib"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/grrmod"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/pngu"
  ${PNG_INCLUDE_DIRS}
  ${JPEG_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${MXML_INCLUDE_DIRS}
)

dkp_add_embedded_binary_library(data
  ${BIN_FILES}
)

target_link_libraries(Wii-Tac-Toe PRIVATE
  data
  fmt::fmt
  ${PNG_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${MXML_LIBRARIES}
  ${FAT}
  ${AESND}
  ${PNGU}
  ${GRRMOD}
)

ogc_create_dol(Wii-Tac-Toe)
