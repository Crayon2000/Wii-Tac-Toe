cmake_minimum_required(VERSION 3.24)
project(Wii-Tac-Toe)

find_library(AESND aesnd
  PATHS "${OGC_ROOT}/lib/${OGC_SUBDIR}"
  REQUIRED
)
find_library(FAT fat
  PATHS "${OGC_ROOT}/lib/${OGC_SUBDIR}"
  REQUIRED
)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PNG REQUIRED libpng)
pkg_check_modules(FREETYPE REQUIRED freetype2)
pkg_check_modules(MXML REQUIRED mxml)

include(FetchContent)
FetchContent_Declare(fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/11.2.0.tar.gz
)
FetchContent_Declare(grrlib
  GIT_REPOSITORY https://github.com/GRRLIB/GRRLIB.git
  GIT_TAG        aa61ecacc830deccf19ab91148ec9273a3d9f7a1
)
FetchContent_Declare(grrmod
  URL https://codeload.github.com/GRRLIB/GRRMOD/tar.gz/670f84601a5086c4d3eeb03bd3041e3c0a7f0ace
)
set(FMT_INSTALL OFF CACHE BOOL "Don't install" FORCE)
set(GRRLIB_INSTALL OFF CACHE BOOL "Don't install" FORCE)
set(GRRMOD_INSTALL OFF CACHE BOOL "Don't install" FORCE)
set(GRRMOD_USE_MP3 OFF CACHE BOOL "Don't use MP3" FORCE)
FetchContent_MakeAvailable(fmt grrlib grrmod)

file(GLOB_RECURSE SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/fonts/*.cpp"
)

file(GLOB_RECURSE BIN_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/audio/*.*"
  "${CMAKE_CURRENT_SOURCE_DIR}/gfx/*.png"
  "${CMAKE_CURRENT_SOURCE_DIR}/languages/*.xml"
)

add_executable(Wii-Tac-Toe)

target_compile_features(Wii-Tac-Toe PRIVATE cxx_std_23)

target_compile_options(Wii-Tac-Toe PRIVATE
  -Werror
  -Wall
  -Wunused
  -Wmisleading-indentation
  -Wduplicated-cond
  -Wduplicated-branches
)

target_sources(Wii-Tac-Toe PRIVATE
  ${SRC_FILES}
)

target_include_directories(Wii-Tac-Toe PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/source"
  ${MXML_INCLUDE_DIRS}
)

dkp_add_embedded_binary_library(data
  ${BIN_FILES}
)

target_link_libraries(Wii-Tac-Toe PRIVATE
  data
  fmt::fmt
  grrlib
  grrmod
  ${PNG_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${MXML_LIBRARIES}
  ${FAT}
  ${AESND}
)

ogc_create_dol(Wii-Tac-Toe)

get_target_property(DOL_FILE Wii-Tac-Toe DKP_FILE)
install(FILES "${DOL_FILE}" DESTINATION "Wii-Tac-Toe" RENAME "boot.dol")
install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/Wii-Tac-Toe/icon.png"
  "${CMAKE_CURRENT_SOURCE_DIR}/Wii-Tac-Toe/meta.xml"
  DESTINATION "Wii-Tac-Toe"
)

set(CPACK_PACKAGE_NAME "wii-tac-toe")
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}")
include(CPack)
