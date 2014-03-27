include_directories( ${CMAKE_SOURCE_DIR}/src/foreign/zthread/include )


add_executable(GPSConnector GPSConnector.cpp)

TARGET_LINK_LIBRARIES(GPSConnector
huginbase huginbasewx 
${Boost_LIBRARIES}

${common_libs}
${image_libs}
${ZTHREAD_LIBRARIES})

install(TARGETS GPSConnector DESTINATION ${BINDIR})