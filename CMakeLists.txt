# Cmake setup
cmake_minimum_required (VERSION 3.28)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

include (cmake/CPM.cmake)
include (cmake/WindowsConfig.cmake)

# Solution
set (PROJECT_NAME "3D Controller Overlay")

project (${PROJECT_NAME} CXX)
set (CMAKE_CXX_EXTENSIONS ON)
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_VS_JUST_MY_CODE_DEBUGGING 1)
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)

# Declare Executable
set (BINARY_NAME "3dco")
add_executable (
    ${BINARY_NAME} WIN32
)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY 
    VS_STARTUP_PROJECT ${BINARY_NAME})

# Add SDL2 Dependency
set(SDL_HIDAPI ON)
set(SDL_SHARED ON)
set(SDL_STATIC OFF)
set(SDL_FORCE_STATIC_VCRT ON)
set(SDL2_DISABLE_SDL2MAIN ON)
set(SDL2_DISABLE_UNINSTALL ON)
set(SDL_TEST OFF)
CPMAddPackage (
    NAME SDL2
    GITHUB_REPOSITORY libsdl-org/SDL
    GIT_TAG release-2.28.5
)
set_target_properties(sdl_headers_copy PROPERTIES FOLDER "SDL2")
set_target_properties(SDL2 PROPERTIES FOLDER "SDL2")
add_custom_command(
    TARGET ${BINARY_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:SDL2>"
        "$<TARGET_FILE_DIR:${BINARY_NAME}>/$<TARGET_FILE_NAME:SDL2>"
)
install (
    TARGETS ${BINARY_NAME} SDL2 
    RUNTIME DESTINATION ${PACKAGE_DIR}
)

# GLFW
set(GLFW_BUILD_DOCS  OFF)
set(GLFW_BUILD_WIN32 ON)
set(GLFW_LIBRARY_TYPE STATIC)
CPMAddPackage (
    NAME glfw
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.3
)
add_custom_command(
    TARGET ${BINARY_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:glfw>"
        "$<TARGET_FILE_DIR:${BINARY_NAME}>/$<TARGET_FILE_NAME:glfw>"
)
install (
    TARGETS ${BINARY_NAME} glfw 
    RUNTIME DESTINATION ${PACKAGE_DIR}
)

# OpenGL
find_package(OpenGL REQUIRED)

# Dear ImGUI
CPMAddPackage (
    NAME imgui
    GITHUB_REPOSITORY ocornut/imgui
    GIT_TAG v1.89.9-docking
)
CPMAddPackage (
    NAME imgui_filebrowser
    GITHUB_REPOSITORY AirGuanZ/imgui-filebrowser
    GIT_TAG a83245244bf8d8a85edc23af9fe6f28c032d7838 # dockable
)
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imconfig.h ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imgui_tables.cpp ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imstb_rectpack.h ${imgui_SOURCE_DIR}/imstb_textedit.h
    ${imgui_SOURCE_DIR}/imstb_truetype.h
    ${imgui_filebrowser_SOURCE_DIR}/imfilebrowser.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${glfw_SOURCE_DIR}/include ${imgui_filebrowser_SOURCE_DIR})
target_link_libraries(imgui PUBLIC SDL2::SDL2)

# Define 3dco project
target_sources (${BINARY_NAME}
    PRIVATE
    src/main.cpp
    src/glad.c
    include/glad/glad.h
    include/stb_image.h
    include/KHR/khrplatform.h
    src/cube_info.h
    src/shaders.h
    src/strings.h
    3dco.rc
)
target_sources(${BINARY_NAME}
  PRIVATE
    FILE_SET CXX_MODULES FILES
      src/controller_window.ixx
      src/settings_window.ixx
      src/settings.ixx
      src/model.ixx
      src/shader.ixx
)      
target_include_directories(${BINARY_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${OPENGL_INCLUDE_DIRS}
)
target_compile_definitions (
    ${BINARY_NAME} PRIVATE
    -DAPPLICATION_NAME="${PROJECT_NAME}"
    -DAPPLICATION_RDN="com.github."
    $<$<CONFIG:Debug>:DEBUG>
)

# Post Build copy commands of resource files and dirs
add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/icon.png" "$<TARGET_FILE_DIR:${BINARY_NAME}>/icon.png"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/models" "$<TARGET_FILE_DIR:${BINARY_NAME}>/models"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:${BINARY_NAME}>/shaders"
)

target_link_libraries (
    ${BINARY_NAME} PRIVATE
    Platform::Dependencies
    SDL2
    glfw
    imgui
    OpenGL::GL
)
set_target_properties(${BINARY_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)/$(Configuration)/"
    VS_DEBUGGER_COMMAND_ARGUMENTS ""
)
# Install