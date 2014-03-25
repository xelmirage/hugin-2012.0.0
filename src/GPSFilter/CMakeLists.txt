include_directories( ${CMAKE_SOURCE_DIR}/src/foreign/zthread/include )


add_executable(GPSFilter GPSFilter.cpp)

TARGET_LINK_LIBRARIES(GPSFilter
huginbase huginbasewx 
${Boost_LIBRARIES}
${wxWidgets_LIBRARIES}
${common_libs}
${image_libs}
${ZTHREAD_LIBRARIES})

install(TARGETS GPSFilter DESTINATION ${BINDIR})