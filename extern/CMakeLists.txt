# GLAD
add_subdirectory(glad)

# GLFW3
find_package(glfw3 3.3 REQUIRED)

# Combine GLAD and GLFW3 since we always use them together
add_library(glad_and_glfw3 INTERFACE)
target_link_libraries(glad_and_glfw3 INTERFACE glad glfw3)
if (UNIX)
    if (APPLE)
        target_link_libraries(glad_and_glfw3 INTERFACE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
        )
    else()
        target_link_libraries(glad_and_glfw3 INTERFACE
            pthread
            "-lrt"
            "-lm"
            "-ldl"
            "-lX11"
        )
    endif()
endif()

# Dear ImGui
add_subdirectory(imgui)
