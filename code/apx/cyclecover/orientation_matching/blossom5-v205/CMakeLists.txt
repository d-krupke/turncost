# This file defines the library `blossom5-bgl'.
# You need to add
#   add_subdirectory(./blossom5-v205)
#   target_link_libraries(matching_example blossom5-bgl)
# to your CMakeLists.txt with `./blossom5-v205' being the blossom-V directory and `matching_example' being your project.

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/PerfectMatching.h)
    message(FATAL_ERROR "Needs blossom5 algorithm! You can download it from http://pub.ist.ac.at/~vnk/software.html\n Put the files into ${CMAKE_CURRENT_SOURCE_DIR}")
endif ()

add_library(blossom5-bgl STATIC blossom5-bgl.h
        block.h
        LCA.h
        misc.cpp
        PerfectMatching.h
        PMduals.cpp
        PMexpand.cpp
        PMimplementation.h
        PMinit.cpp
        PMinterface.cpp
        PMmain.cpp
        PMrepair.cpp
        PMshrink.cpp
        PQ.h
        timer.h
        MinCost/MinCost.h
        MinCost/MinCost.cpp)

