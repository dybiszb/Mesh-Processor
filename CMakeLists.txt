#===============================================================================
# Parallel Interactive Mesh Processor (PIMP)
#==============================================================================
# author: dybisz
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.4)
project(MeshProcessor)
set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_MODULE_PATH  "${CMAKE_SOURCE_DIR}/cmake-modules" )

#===============================================================================
# Find wxWidgets
#-------------------------------------------------------------------------------
FIND_PACKAGE(wxWidgets REQUIRED html adv core base net aui xrc qa richtext gl)
if (wxWidgets_FOUND)
    MESSAGE("wxWidgets Found!")
    INCLUDE(${wxWidgets_USE_FILE})
    LINK_LIBRARIES(${wxWidgets_LIBRARIES})
else (wxWidgets_FOUND)
    message(FATAL_ERROR "wxWidgets NOT Found")
endif (wxWidgets_FOUND)


#===============================================================================
# Find OpenGL
#-------------------------------------------------------------------------------
FIND_PACKAGE(OpenGL REQUIRED)
if (OPENGL_FOUND)
    MESSAGE("OpenGL Found!")
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})
    LINK_LIBRARIES(${OPENGL_LIBRARIES})
else (OPENGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL NOT Found")
endif (OPENGL_FOUND)


#===============================================================================
# Find GLEW
#-------------------------------------------------------------------------------
FIND_PACKAGE(GLEW REQUIRED)
if (GLEW_FOUND)
    MESSAGE("GLEW Found!")
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
    LINK_LIBRARIES(${GLEW_LIBRARIES})
    ADD_DEFINITIONS(${GLEW_DEFINITIONS})
else (GLEW_FOUND)
    MESSAGE(FATAL_ERROR "GLEW NOT Found")
endif (GLEW_FOUND)

#===============================================================================
# Find Assimp
#-------------------------------------------------------------------------------
FIND_PACKAGE(assimp REQUIRED)
if (assimp_FOUND)
    MESSAGE("GLEW Found!")
    INCLUDE_DIRECTORIES(${assimp_INCLUDE_DIRS})
    LINK_LIBRARIES(${assimp_LIBRARIES})
else (assimp_FOUND)
    MESSAGE(FATAL_ERROR "assimp NOT Found")
endif (assimp_FOUND)

#===============================================================================
# Include Sources
#-------------------------------------------------------------------------------
include_directories(include)
set(SOURCE_FILES
        src/main.cpp
        src/application/main_frame.cpp
        src/application/gl_plane.cpp
        src/rendering/shader_program.cpp
        )

#===============================================================================
# Compile & Link
#-------------------------------------------------------------------------------
add_executable(MeshProcessor ${SOURCE_FILES})
target_link_libraries(MeshProcessor
        ${wxWidgets_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        )

#===============================================================================
# Copy Resources
#-------------------------------------------------------------------------------
add_custom_command(
        TARGET MeshProcessor PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory   # Copy resources
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:MeshProcessor>/res
)