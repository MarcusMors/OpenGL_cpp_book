project(my_lib LANGUAGES CXX)

# ----------------------------------------------------------------------------
# Collect your library sources
# ----------------------------------------------------------------------------
file(GLOB MY_LIB_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

file(GLOB MY_LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

# If you prefer to list files explicitly (recommended once the project grows):
# set(MY_LIB_SOURCES
#     ${CMAKE_CURRENT_SOURCE_DIR}/my_renderer.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/my_shader.cpp
# )
# set(MY_LIB_HEADERS
#     ${CMAKE_CURRENT_SOURCE_DIR}/my_renderer.hpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/my_shader.hpp
# )

add_library(my_lib STATIC ${MY_LIB_SOURCES} ${MY_LIB_HEADERS})

# ----------------------------------------------------------------------------
# Everyone who links my_lib gets access to its headers automatically
# ----------------------------------------------------------------------------
target_include_directories(my_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ----------------------------------------------------------------------------
# Link every dependency as PUBLIC so that any executable linking my_lib
# automatically pulls in all required symbols / headers.
# ----------------------------------------------------------------------------
target_link_libraries(my_lib
    PUBLIC
        glad
        ${GLFW_TARGET}
        ${GLEW_TARGET}
        $<IF:$<TARGET_EXISTS:glm::glm>,glm::glm,glm>
        SOIL2
        OpenGL::GL
)

# ----------------------------------------------------------------------------
# Platform-specific tweaks
# ----------------------------------------------------------------------------
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # On Linux, some linkers need -ldl for dynamic loading (used by GLEW/GLAD).
    target_link_libraries(my_lib PUBLIC ${CMAKE_DL_LIBS})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Silence common MSVC warnings for OpenGL C headers included from C++.
    target_compile_definitions(my_lib PUBLIC NOMINMAX _CRT_SECURE_NO_WARNINGS)
endif()
