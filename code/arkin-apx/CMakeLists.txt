
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#add_subdirectory(./strip_cover/bipartite_vertex_cover)

set(SOURCE_FILES strip_cover/Strip.cpp strip_cover/Strip.h strip_cover/StripExtractor.cpp strip_cover/StripExtractor.h strip_matching/StripMatchingSolver.cpp strip_matching/StripMatchingSolver.h approximation.cpp approximation.h strip_cover/StripCover.cpp strip_cover/StripCover.h)
add_library(l_arkinapx ${SOURCE_FILES})
target_link_libraries(l_arkinapx linstance)