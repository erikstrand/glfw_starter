add_library(graphics
    glfw_app.cpp
    glfw_app.h
    shader_program.cpp
    shader_program.h
)
target_link_libraries(graphics PRIVATE shared_settings)
target_link_libraries(graphics PUBLIC glad_and_glfw3 imgui_glfw)
target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
    DEPENDS shader.vs
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shader.vs
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy shader.vs ${CMAKE_CURRENT_BINARY_DIR}/shader.vs
    COMMENT "Copying 2D vertex shader to build directory"
)

add_custom_command(
    DEPENDS shader.fs
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shader.fs
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy shader.fs ${CMAKE_CURRENT_BINARY_DIR}/shader.fs
    COMMENT "Copying 2D fragment shader to build directory"
)

add_executable(main_executable main.cpp)
target_link_libraries(main_executable shared_settings graphics)

add_custom_target(main ALL
    DEPENDS
        main_executable
        ${CMAKE_CURRENT_BINARY_DIR}/shader.vs
        ${CMAKE_CURRENT_BINARY_DIR}/shader.fs
)
