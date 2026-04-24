project(Project_1 LANGUAGES CXX)

# ----------------------------------------------------------------------------
# Collect this scene / application's source files
# ----------------------------------------------------------------------------
file(GLOB PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

file(GLOB PROJECT_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

# Explicit file listing (recommended once you know your file set):
# set(PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
# set(PROJECT_HEADERS )

add_executable(Project_1 ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# ----------------------------------------------------------------------------
# Link my_lib — it re-exports all OpenGL / windowing / math dependencies.
# ----------------------------------------------------------------------------
target_link_libraries(Project_1 PRIVATE my_lib)

# ----------------------------------------------------------------------------
# If you need extra project-specific preprocessor flags, add them here.
# ----------------------------------------------------------------------------
# target_compile_definitions(Project_1 PRIVATE MY_SCENE_DEFINE=1)

# ----------------------------------------------------------------------------
# Windows: copy DLLs next to the .exe so it runs without modifying PATH.
# ----------------------------------------------------------------------------
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # GLFW creates glfw.dll when built as shared; if it is a DLL, copy it.
    if(TARGET glfw AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET Project_1 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:glfw>
                $<TARGET_FILE_DIR:Project_1>
        )
    endif()
endif()
