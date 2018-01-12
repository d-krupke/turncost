
file(GLOB_RECURSE GTEST_SOURCES "../**/*_gtest.cpp")
MESSAGE("Including the following gtest files: ${GTEST_SOURCES}")

file(GLOB_RECURSE GTEST_ASSETS "./assets/*")
MESSAGE("Including the following gtest assets: ${GTEST_ASSETS}")
FILE(MAKE_DIRECTORY "./assets")
FILE(COPY ${GTEST_ASSETS} DESTINATION "./assets")

add_executable(unit_tester
        ${GTEST_SOURCES}
        ./gtest_main.cpp
        ../shortest_path/grid_graph
        ../shortest_path/turn_cost_grid_dijkstra
        ../apx/approximation_gtest.cpp
        ../ip/general_cc_solver/ip_solver_gtest.cpp ../ip/subset_separation/voronoi_extender_gtest.cpp.cpp ../ip/fc_separation/full_coverage_gtest.cpp ../ip/subset_separation/subset_coverage_gtest.cpp ../ip/penalty_separation/penalty_coverage_gtest.cpp ../arkin-apx/strip_cover/Strip_gtest.cpp ../arkin-apx/strip_cover/StripExtractor_gtest.cpp ../arkin-apx/strip_matching/StripMatchingSolver_gtest.cpp ../arkin-apx/approximation_gtest.cpp ../arkin-apx/strip_cover/StripCover_gtest.cpp)

target_link_libraries(unit_tester
        gtest
        blossom5-bgl
        pcstfast
        lapxalg
        linstance
        l_ip
        l_arkinapx)

target_include_directories(unit_tester PRIVATE ${INCLUDE_DIRS})
target_link_libraries(unit_tester ${LIBRARIES})