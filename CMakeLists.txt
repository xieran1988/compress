project(compress)
include_directories(${CMAKE_FIND_ROOT_PATH}/usr/include/python2.7)
add_library(cmdusr SHARED cmdusr.c packet.c conn.c)
add_library(cap SHARED packet.c conn.c cap.c)
TARGET_LINK_LIBRARIES(cmdusr -lpython2.7)


