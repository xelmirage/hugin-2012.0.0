




add_executable(ARLabStitcherwx WIN32 ARLabStitcherwx.cpp ARLabStitcherwx.h ARLabStitcherwx_app.cpp ARLabStitcherwx_app.h ARLabStitcherwxMainFrame.h ARLabStitcherwxMainFrame.cpp ARLabStitcherwx_rc.rc ARLabStitcherwxNewProjectWizard.cpp ARLabStitcherwxNewProjectWizard.h SuperOverlay.cpp SuperOverlay.h pyramidNode.cpp
ARLabStitcherwxGPSFrame.h ARLabStitcherwxGPSFrame.cpp
ARLabStitcherwxControlPointFrame.cpp ARLabStitcherwxControlPointFrame.h
CommandHistory.cpp CommandHistory.h
config_defaults.h
CPDetectorDialog.cpp CPDetectorDialog.h 
CPEditorPanel.h CPEditorPanel.cpp
CPImageCtrl.h CPImageCtrl.cpp
CPImagesComboBox.h CPImagesComboBox.cpp
CPListFrame.h CPListFrame.cpp
fake_huginApp.h
HFOVDialog.h HFOVDialog.cpp
hugin_rc.rc
TextKillFocusHandler.h  TextKillFocusHandler.cpp
wxPanoCommand.h wxPanoCommand.cpp)

TARGET_LINK_LIBRARIES(ARLabStitcherwx 
huginbase huginbasewx 
${Boost_LIBRARIES}
${wxWidgets_LIBRARIES}
icpfindlib
)
if(WIN32)
   set_target_properties(ARLabStitcherwx PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")

   set_target_properties(ARLabStitcherwx PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")

   set_target_properties(ARLabStitcherwx PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(ARLabStitcherwx PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)
install(TARGETS ARLabStitcherwx DESTINATION ${BINDIR})