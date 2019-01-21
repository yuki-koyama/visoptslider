cmake_minimum_required(VERSION 3.1)

project(visopt-slider CXX)

set(CMAKE_CXX_STANDARD 11)

if(APPLE AND EXISTS /usr/local/opt/qt5)
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()
if(APPLE AND EXISTS /usr/local/opt/qt)
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
endif()

set(TINYCOLORMAP_BUILD_TOOLS OFF CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/tinycolormap)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

find_packageile(GLOB
	headers
	${CMAKE_CURRENT_SOURCE_DIR}/include/visopt-slider/*.hpp
)
file(GLOB
	sources
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(visopt-slider STATIC ${headers} ${sources})
target_link_libraries(visopt-slider Qt5::Core Qt5::Gui Qt5::Widgets tinycolormap)
target_include_directories(visopt-slider PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(FILES ${headers} DESTINATION include/visopt-slider)
install(TARGETS visopt-slider ARCHIVE DESTINATION lib)
