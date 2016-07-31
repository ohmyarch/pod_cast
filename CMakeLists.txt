cmake_minimum_required(VERSION 3.6.1)

project(pod_cast VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

option(AUTO_REFORMAT_QML_FILES "Reformat all qml files at build time." OFF)

if(IOS OR ANDROID)
  find_package(Qt5 5.7.0 COMPONENTS WebView QuickControls2 CONFIG)
elseif(WIN32)
  find_package(Qt5 5.7.0 REQUIRED COMPONENTS Svg Widgets WebView QuickControls2 CONFIG)
else()
  find_package(Qt5 5.7.0 REQUIRED COMPONENTS Widgets WebView QuickControls2 CONFIG)
endif()

find_package(feed_parser REQUIRED CONFIG)

if(AUTO_REFORMAT_QML_FILES)
  file(GLOB_RECURSE QML_FILES qml/*)

  add_custom_target(reformat_qml_files)

  foreach(QML_FILE ${QML_FILES})
    add_custom_command(TARGET reformat_qml_files COMMAND qmlfmt -w ${QML_FILE})
  endforeach()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message("-- Setting clang options")

  set(WARNINGS "${WARNINGS} ${OSX_SUPPRESSIONS}")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wno-return-type-c-linkage -Wno-unneeded-internal-declaration")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-strict-aliasing")

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  message("-- Setting gcc options")

  set(WARNINGS "-Wall -Wextra -Wunused-parameter -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls -Wunreachable-code")

  set(LD_FLAGS "${LD_FLAGS} -Wl,-z,defs")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-strict-aliasing")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  message("-- Setting msvc options")
  set(WARNINGS)
else()
  message("-- Unknown compiler, success is doubtful.")
  message("CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
