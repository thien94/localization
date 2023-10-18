ADD_LIBRARY(types_edge_se3range STATIC
	${G2O_LIB_TYPE}
 	types_edge_se3range.cpp
 	types_edge_se3range_offset.cpp
 )

SET_TARGET_PROPERTIES(types_edge_se3range PROPERTIES OUTPUT_NAME types_edge_se3range)

TARGET_LINK_LIBRARIES(
	types_edge_se3range
	g2o_core
	g2o_types_slam3d
)

# INSTALL(TARGETS types_se3_edge
#   RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
#   LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
#   ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
# )

FILE(GLOB ${PROJECT_NAME}_headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# INSTALL(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/g2o/types/sba)
