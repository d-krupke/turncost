
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#add_subdirectory(./strip_cover/bipartite_vertex_cover)

set(SOURCE_FILES strip_cover/strip.cpp strip_cover/strip.h strip_cover/strip_extractor.cpp strip_cover/strip_extractor.h strip_matching/strip_matching_solver.cpp strip_matching/strip_matching_solver.h approximation.cpp approximation.h strip_cover/strip_cover.cpp strip_cover/strip_cover.h)
add_library(l_arkinapx ${SOURCE_FILES})
target_link_libraries(l_arkinapx linstance)