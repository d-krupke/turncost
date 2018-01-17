include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/pcst_fast.h)
    message(FATAL_ERROR "Needs pcst_fast algorithm! You can download it from https://github.com/fraenkel-lab/pcst_fast/tree/master/src\n Put the files into ${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

add_library(pcstfast STATIC
        ./pairing_heap
        ./pcst_fast
        ./priority_queue)
