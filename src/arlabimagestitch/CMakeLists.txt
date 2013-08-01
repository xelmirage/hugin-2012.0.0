#Find QT4
FIND_PACKAGE(QT4 REQUIRED)
    
# Make the headers visible to everything
INCLUDE_DIRECTORIES(
    ${QT_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

LINK_DIRECTORIES(
    ${QT_LIBRARY_DIR}
    ${QT_PLUGINS_DIR}
)

ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(ImageStitch_SOURCES
	multiview.cpp
	main.cpp)

SET(ImageStitch_HEADERS
	multiview.h)

QT4_WRAP_CPP(ImageStitch_HEADERS_MOC ${ImageStitch_HEADERS})

ADD_EXECUTABLE(ARLab_ImageStitch
	${ImageStitch_HEADERS_MOC}
	${ImageStitch_SOURCES} )
	
TARGET_LINK_LIBRARIES(ARLab_ImageStitch
	localfeatures
	qtmaind
	qtcored4
	qtguid4
)

install(TARGETS ARLab_ImageStitch DESTINATION ${BINDIR})
