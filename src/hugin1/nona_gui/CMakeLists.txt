
IF (WIN32)
  add_executable(nona_gui WIN32 nona_gui.cpp nona_gui_rc.rc)
ELSE (WIN32)
  add_executable(nona_gui nona_gui.cpp)
ENDIF (WIN32)


target_link_libraries(nona_gui ${common_libs} ${image_libs} huginbasewx ${wxWidgets_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARIES})

install(TARGETS nona_gui DESTINATION ${BINDIR})

