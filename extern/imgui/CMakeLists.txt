add_library(imgui_glfw
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imconfig.h
    imgui/imstb_textedit.h
    imgui/imstb_rectpack.h
    imgui/imstb_truetype.h
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_glfw.h
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_opengl3.h
)
target_include_directories(imgui_glfw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(imgui_glfw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_link_libraries(imgui_glfw PRIVATE shared_settings)
target_link_libraries(imgui_glfw PUBLIC glad)
target_compile_definitions(imgui_glfw PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
