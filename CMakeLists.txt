cmake_minimum_required(VERSION 2.8)

project(scrabble)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

include_directories(${SFML_INCLUDE_DIR}
	include)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

find_package(SFML 2 REQUIRED graphics window audio system)

target_link_libraries(scrabble
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES})