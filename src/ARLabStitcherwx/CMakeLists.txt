include_directories( ${CMAKE_SOURCE_DIR}/src/foreign/zthread/include )




add_executable(ARLabStitcherwx WIN32 ARLabStitcherwx.cpp ARLabStitcherwx.h ARLabStitcherwx_app.cpp ARLabStitcherwx_app.h ARLabStitcherwxMainFrame.h ARLabStitcherwxMainFrame.cpp ARLabStitcherwx_rc.rc ARLabStitcherwxNewProjectWizard.cpp ARLabStitcherwxNewProjectWizard.h SuperOverlay.cpp SuperOverlay.h pyramidNode.cpp
ARLabStitcherwxGPSFrame.h ARLabStitcherwxGPSFrame.cpp)

TARGET_LINK_LIBRARIES(ARLabStitcherwx 
huginbase huginbasewx 
${Boost_LIBRARIES}
${wxWidgets_LIBRARIES}
${common_libs}
${image_libs}
${ZTHREAD_LIBRARIES}
)
if(WIN32)
   set_target_properties(ARLabStitcherwx PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")

   set_target_properties(ARLabStitcherwx PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")

   set_target_properties(ARLabStitcherwx PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(ARLabStitcherwx PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)
install(TARGETS ARLabStitcherwx DESTINATION ${BINDIR})