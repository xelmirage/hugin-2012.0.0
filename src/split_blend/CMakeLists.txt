


add_executable(Split_blend split_blend.cpp split_blend.h)

TARGET_LINK_LIBRARIES(Split_blend 
huginbase huginbasewx 
${Boost_LIBRARIES}
${wxWidgets_LIBRARIES})

install(TARGETS Split_blend DESTINATION ${BINDIR})