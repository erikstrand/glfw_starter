#---------------------------------------------------------------------------------------------------
add_library(shared_settings INTERFACE)

# Performance related flags for C++
target_compile_options(shared_settings INTERFACE $<$<COMPILE_LANGUAGE:CXX>:
    -march=native
    -mtune=native
    #-ffast-math
>)

# Warnings
target_compile_options(shared_settings INTERFACE $<$<COMPILE_LANGUAGE:CXX>:
    -Wall
    -Wcast-align
    #-Wcast-qual
    -Wextra
    -Wundef
    #-Wuseless-cast
    #-Wzero-as-null-pointer-constant
    -pedantic
>)
