cmake_minimum_required(VERSION 3.1)

project(visoptslider CXX)

set(CMAKE_CXX_STANDARD 11)

# Options

option(VISOPTSLIDER_BUILD_TESTS "Build tests" OFF)

# Dependencies

if(APPLE AND EXISTS /usr/local/opt/qt5)
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()
if(APPLE AND EXISTS /usr/local/opt/qt)
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
endif()

set(TINYCOLORMAP_BUILD_TOOLS OFF CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/tinycolormap)

find_package(Eigen3 REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Library build

file(GLOB
	headers
	${CMAKE_CURRENT_SOURCE_DIR}/include/visoptslider/*.hpp
)

file(GLOB
	sources
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(visoptslider STATIC ${headers} ${sources})
target_link_libraries(visoptslider Eigen3::Eigen Qt5::Core Qt5::Gui Qt5::Widgets tinycolormap)
target_include_directories(visoptslider PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Library installation

install(FILES ${headers} DESTINATION include/visoptslider)
install(TARGETS visoptslider ARCHIVE DESTINATION lib)

# Build tests

if(VISOPTSLIDER_BUILD_TESTS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/optimization-test-functions)

	add_executable(simple-test ${CMAKE_CURRENT_SOURCE_DIR}/tests/simple_test.cpp)
	target_link_libraries(simple-test visoptslider optimization-test-functions)
endif()
