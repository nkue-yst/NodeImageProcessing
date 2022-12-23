cmake_minimum_required(VERSION 3.20)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

project(NodeImageProcessing CXX)

### OPTIONS ###
option(NIP_TESTS "Build tests" OFF)

### cmake module ###
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/thirdparty/cmake/sdl2)

### imgui ###
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/thirdparty/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
set(IMGUI_SOURCES
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
	${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

### imnodes ###
set(IMNODES_DIR ${CMAKE_SOURCE_DIR}/thirdparty/imnodes)
include_directories(${IMNODES_DIR})
set(IMNODES_SOURCES ${IMNODES_DIR}/imnodes.cpp)

### OpenGL ###
find_package(OpenGL REQUIRED)

### SDL2 ###
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

### stb ###
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/stb)

### NodeImageProcessing ###
include_directories(${CMAKE_SOURCE_DIR}/source)
set(NIP_SOURCES
	${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/ImageNode.cpp
    ${CMAKE_SOURCE_DIR}/source/NodeEditor.cpp
)
add_executable(${PROJECT_NAME} ${NIP_SOURCES} ${IMGUI_SOURCES} ${IMNODES_SOURCES})
target_link_libraries(${PROJECT_NAME} OpenGL::GL SDL2::Core SDL2::Main)

if(WIN32)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif(WIN32)

if(NIP_TESTS)
	add_executable(test_SimpleWindow ${CMAKE_SOURCE_DIR}/source/test.cpp ${IMGUI_SOURCES} ${IMNODES_SOURCES})
	target_link_libraries(test_SimpleWindow ${SDL2_LIBRARIES})
endif()

### Copy sample images ###
file(COPY ${CMAKE_SOURCE_DIR}/sample DESTINATION ${CMAKE_BINARY_DIR})
