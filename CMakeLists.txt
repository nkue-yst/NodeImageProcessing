cmake_minimum_required(VERSION 3.20)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings.")
endif()

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

### ImGuiFileDialog ###
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/ImGuiFileDialog)
set(IMGUI_FILE_DIALOG_SOURCES ${CMAKE_SOURCE_DIR}/thirdparty/ImGuiFileDialog/ImGuiFileDialog.cpp)

### OpenGL ###
find_package(OpenGL REQUIRED)

### OpenCV ###
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

### SDL2 ###
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

### stb ###
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/stb)

### NodeImageProcessing ###
include_directories(${CMAKE_SOURCE_DIR}/source)
set(NIP_SOURCES
	${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/BinarizationNode.cpp
    ${CMAKE_SOURCE_DIR}/source/EdgeDetectionNode.cpp
    ${CMAKE_SOURCE_DIR}/source/GrayScalingNode.cpp
    ${CMAKE_SOURCE_DIR}/source/ImageNode.cpp
	${CMAKE_SOURCE_DIR}/source/ImageSourceNode.cpp
    ${CMAKE_SOURCE_DIR}/source/InformationNode.cpp
    ${CMAKE_SOURCE_DIR}/source/NodeEditor.cpp
    ${CMAKE_SOURCE_DIR}/source/VideoNode.cpp
    ${CMAKE_SOURCE_DIR}/source/VideoSourceNode.cpp
)
add_executable(${PROJECT_NAME} ${NIP_SOURCES} ${IMGUI_SOURCES} ${IMNODES_SOURCES} ${IMGUI_FILE_DIALOG_SOURCES})
target_link_libraries(${PROJECT_NAME} OpenGL::GL ${OpenCV_LIBRARIES} SDL2::Core SDL2::Main)

if(WIN32)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif(WIN32)

### Copy sample images ###
file(COPY ${CMAKE_SOURCE_DIR}/sample DESTINATION ${CMAKE_BINARY_DIR})
