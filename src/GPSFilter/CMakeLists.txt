


add_executable(GPSFilter GPSFilter.cpp)

TARGET_LINK_LIBRARIES(GPSFilter
huginbase huginbasewx 
${Boost_LIBRARIES}
${wxWidgets_LIBRARIES})

install(TARGETS GPSFilter DESTINATION ${BINDIR})