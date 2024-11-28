cmake_minimum_required(VERSION 3.11)
project("Language")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(lang_front_srcs
    Frontend/src/main.cpp
    Frontend/src/input_output.cpp
    Frontend/src/parcer.cpp
    Frontend/src/simplifier.cpp
)
set(lang_back_srcs
    Backend/src/main.cpp 
    Backend/src/input_output.cpp 
    Backend/stack/src/stack.cpp 
    Backend/src/translator.cpp
)
set(lang_asm_srcs
    Backend/processor/Assembler/src/assembler.cpp  
    Backend/processor/Onegin/src/file_size.cpp  
    Backend/processor/Onegin/src/string_read.cpp  
    Backend/processor/Onegin/src/destruct.cpp  
    Backend/processor/Onegin/src/str_print.cpp
    Backend/processor/Stack/src/stack.cpp
)
set(lang_proc_srcs
    Backend/processor/CPU/src/CPU.cpp  
    Backend/processor/Onegin/src/file_size.cpp  
    Backend/processor/Onegin/src/string_read.cpp  
    Backend/processor/Onegin/src/destruct.cpp  
    Backend/processor/Onegin/src/str_print.cpp  
    Backend/processor/Stack/src/stack.cpp
)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(FLAGS   "-D NDEBUG")

add_executable(front.x ${lang_front_srcs})
add_executable(back.x  ${lang_back_srcs})
add_executable(asm.x  ${lang_asm_srcs})
add_executable(proc.x  ${lang_proc_srcs})
# add_executable(test.x  ${test_srcs})

target_include_directories(front.x PRIVATE 
            ${CMAKE_SOURCE_DIR}/Frontend/include  
            ${CMAKE_SOURCE_DIR}/Frontend/library  
)
target_include_directories(back.x PRIVATE 
            ${CMAKE_SOURCE_DIR}/Backend/include  
            ${CMAKE_SOURCE_DIR}/Backend/stack/include 
            ${CMAKE_SOURCE_DIR}/Backend/library
)
target_include_directories(asm.x PRIVATE 
            ${CMAKE_SOURCE_DIR}/Backend/processor/include  
            ${CMAKE_SOURCE_DIR}/Backend/processor/Library
            ${CMAKE_SOURCE_DIR}/Backend/processor/Onegin/include
            ${CMAKE_SOURCE_DIR}/Backend/processor/Stack/include
            ${CMAKE_SOURCE_DIR}/Backend/processor/Assembler/include
            ${CMAKE_SOURCE_DIR}/Backend/processor/CPU/include
)
target_include_directories(proc.x PRIVATE 
            ${CMAKE_SOURCE_DIR}/Backend/processor/include  
            ${CMAKE_SOURCE_DIR}/Backend/processor/Library
            ${CMAKE_SOURCE_DIR}/Backend/processor/Onegin/include
            ${CMAKE_SOURCE_DIR}/Backend/processor/Stack/include
            ${CMAKE_SOURCE_DIR}/Backend/processor/Assembler/include
            ${CMAKE_SOURCE_DIR}/Backend/processor/CPU/include
)

target_compile_options(front.x PRIVATE "$<$<CONFIG:RELEASE>:${CMAKE_CXX_FLAGS_RELEASE}>" ${FLAGS})
target_compile_options(back.x  PRIVATE "$<$<CONFIG:RELEASE>:${CMAKE_CXX_FLAGS_RELEASE}>" ${FLAGS})
target_compile_options(asm.x   PRIVATE "$<$<CONFIG:RELEASE>:${CMAKE_CXX_FLAGS_RELEASE}>" ${FLAGS})
target_compile_options(proc.x  PRIVATE "$<$<CONFIG:RELEASE>:${CMAKE_CXX_FLAGS_RELEASE}>" ${FLAGS})

# cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
# cmake --build build
# cmake -DCMAKE_BUILD_TYPE=Release
# ./build/matrix.x
#
# cmake .. -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=./../../../../include-what-you-use/build/bin/include-what-you-use
# make
