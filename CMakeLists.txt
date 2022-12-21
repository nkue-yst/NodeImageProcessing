cmake_minimum_required(VERSION 3.20)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(NodeImageProcessing CXX)

### OPTIONS ###
option(NIP_TESTS "Build tests" ON)

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
	${IMGUI_DIR}/backends/imgui_impl_sdlrenderer.cpp
)

### NodeImageProcessing ###
set(NIP_SOURCES
	${CMAKE_SOURCE_DIR}/source/main.cpp
)
add_executable(${PROJECT_NAME} ${NIP_SOURCES} ${IMGUI_SOURCES})

if(WIN32)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif(WIN32)

### SDL2 ###
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

if(NIP_TESTS)
	add_executable(test_SimpleWindow ${CMAKE_SOURCE_DIR}/source/test.cpp ${IMGUI_SOURCES})
	target_link_libraries(test_SimpleWindow ${SDL2_LIBRARIES})
endif()
