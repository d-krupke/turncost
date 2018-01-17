cmake_minimum_required(VERSION 3.6)
project(gridgraph)


set(CPLEX_ROOT "/opt/ibm/ILOG/CPLEX_Studio1271")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
# Boost and its components
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options graph REQUIRED)
find_package(CPLEX REQUIRED)
find_package(Threads REQUIRED)
if (NOT Boost_FOUND)
    message(STATUS "This project requires the Boost library, and will not be compiled.")
    return()
endif ()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

set(LIBRARIES ${CPLEX_LIBRARIES} ${Boost_LIBRARIES} Threads::Threads)
set(INCLUDE_DIRS ${CPLEX_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


add_subdirectory(./problem_instance)
add_subdirectory(./apx)
add_subdirectory(./tests)
add_subdirectory(./ip)
add_subdirectory(./arkin-apx)

add_executable(solver main.cpp)
target_include_directories(solver PRIVATE ${INCLUDE_DIRS})
target_link_libraries(solver    ${LIBRARIES})
target_link_libraries(solver
        blossom5-bgl
        pcstfast
        lapxalg
        linstance
        l_ip
        l_arkinapx)




