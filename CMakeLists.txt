cmake_minimum_required(VERSION 3.24)
project(gtkhello)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
#set(CMAKE_EXE_LINKER_FLAGS "-static")
file(GLOB INCLUDE_FILES ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB GLSL_FILES ${PROJECT_SOURCE_SIR}/*.glsl)
file(COPY ${GLSL_FILES} DESTINATION ${CMAKE_BINARY_DIR})
FIND_PACKAGE(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
PKG_CHECK_MODULES(GTK4 REQUIRED gtk4)
PKG_CHECK_MODULES(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)
INCLUDE_DIRECTORIES(${GTK4_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
LINK_DIRECTORIES(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})
ADD_EXECUTABLE(${PROJECT_NAME} ${INCLUDE_FILES} ${SOURCE_FILES}) 
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GTK4_LIBRARIES} ${FFMPEG_LIBRARIES} ${OPENGL_LIBRARIES}) 