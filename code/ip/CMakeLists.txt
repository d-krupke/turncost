include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(l_ip
        general_cc_solver/ip_solver_cplex_base
        general_cc_solver/ip_solver.cpp
        general_cc_solver/ip_solver.h
        fc_separation/full_coverage_simple_separator
        fc_separation/full_coverage_sufficient_separator
        subset_separation/subset_sufficient_separator.h
        subset_separation/subset_simple_separator
        penalty_separation/penalty_simple_separator.cpp penalty_separation/penalty_simple_separator.h penalty_separation/penalty_sufficient_separator.cpp penalty_separation/penalty_sufficient_separator.h subset_separation/subset_sufficient_separator.cpp subset_separation/subset_voronoi_separation.cpp subset_separation/subset_voronoi_separation.h subset_separation/voronoi_extender.cpp subset_separation/voronoi_extender.h penalty_separation/penalty_voronoi_separator.cpp penalty_separation/penalty_voronoi_separator.h penalty_separation/max_min_fields.cpp penalty_separation/max_min_fields.h integer_programming.cpp integer_programming.h fc_separation/full_coverage_sufficient_separator.cpp)
target_include_directories(l_ip PRIVATE ${INCLUDE_DIRS})
target_link_libraries(l_ip ${LIBRARIES})
target_link_libraries(l_ip lapxalg)
target_link_libraries(l_ip linstance)