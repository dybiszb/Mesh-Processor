cmake_minimum_required(VERSION 3.6)
project(PIMP)
set(CMAKE_CXX_STANDARD 11)

# --------------------- #
# ----- wxWidgets ----- #
# --------------------- #
FIND_PACKAGE(wxWidgets REQUIRED html adv core base net aui xrc qa richtext gl)
if (wxWidgets_FOUND)
    MESSAGE("wxWidgets Found!")
    INCLUDE(${wxWidgets_USE_FILE})
    LINK_LIBRARIES(${wxWidgets_LIBRARIES})
else (wxWidgets_FOUND)
    message(FATAL_ERROR "wxWidgets NOT Found")
endif (wxWidgets_FOUND)


# ------------------ #
# ----- OpenGL ----- #
# ------------------ #
FIND_PACKAGE(OpenGL REQUIRED)
if (OPENGL_FOUND)
    MESSAGE("OpenGL Found!")
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})
    LINK_LIBRARIES(${OPENGL_LIBRARIES})
else (OPENGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL NOT Found")
endif (OPENGL_FOUND)


# ---------------- #
# ----- GLEW ----- #
# ---------------- #
FIND_PACKAGE(GLEW REQUIRED)
if (GLEW_FOUND)
    MESSAGE("GLEW Found!")
    INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
    LINK_LIBRARIES(${GLEW_LIBRARIES})
    ADD_DEFINITIONS(${GLEW_DEFINITIONS})
else (GLEW_FOUND)
    MESSAGE(FATAL_ERROR "GLEW NOT Found")
endif (GLEW_FOUND)

include_directories(include)
set(SOURCE_FILES
        src/main.cpp
        src/application/main_frame.cpp
        src/application/gl_plane.cpp
        src/rendering/shader_program.cpp
        )

add_executable(PIMP ${SOURCE_FILES})
target_link_libraries(PIMP ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})