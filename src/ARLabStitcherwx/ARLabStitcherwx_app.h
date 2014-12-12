#include "wx/wxprec.h"
#include"ARLabStitcherwxMainFrame.h"
#ifdef __BORLANDC__
   #pragma hdrstop
#endif
#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif
#ifndef __WXMSW__
   #include "wxWidgets.xpm"
#endif

class ARLabStitcherwxApp: public wxApp
{
    virtual bool OnInit();
};

DECLARE_APP(ARLabStitcherwxApp)
IMPLEMENT_APP(ARLabStitcherwxApp)


//	enum
//{
//    ID_Quit=1,
//	ID_About,
//};



BEGIN_EVENT_TABLE(ARLabStitcherwxMainFrame, MainFrame)
EVT_MENU(wxID_New, ARLabStitcherwxMainFrame::newProcessTool)
EVT_TOOL(wxID_ToolStart, ARLabStitcherwxMainFrame::processcmd)
EVT_LISTBOX(wxID_ListBoxPicList, ARLabStitcherwxMainFrame::ListBoxPicListClick)
EVT_TIMER(wxID_TimerProcess, ARLabStitcherwxMainFrame::count_time)
EVT_END_PROCESS(-1, ARLabStitcherwxMainFrame::end_process)
EVT_TOOL(wxID_NEW_PROJECT_TOOL, ARLabStitcherwxMainFrame::newProcessTool)
EVT_TOOL(wxID_toolShowTrack, ARLabStitcherwxMainFrame::showTrack)
EVT_TOOL(wxID_ShowKML, ARLabStitcherwxMainFrame::showTrack)
EVT_TOOL(wxID_SuperOverLay, ARLabStitcherwxMainFrame::generateSuperOverlay)
EVT_MENU(wxID_menuItemPreProcess, ARLabStitcherwxMainFrame::preProcess)


END_EVENT_TABLE()



BEGIN_EVENT_TABLE(ARLabStitcherwxNewProjectWizard, NewProjectWizard)
EVT_BUTTON(wxID_OpenSourceDir, ARLabStitcherwxNewProjectWizard::OpenSourceDir)
EVT_BUTTON(wxID_OpenGPSFile, ARLabStitcherwxNewProjectWizard::OpenGPSFile)
EVT_BUTTON(wxID_OpenOutputFile, ARLabStitcherwxNewProjectWizard::OpenOutputFile)
EVT_TEXT(wxID_SourceDir, ARLabStitcherwxNewProjectWizard::ValidateInputDir)
EVT_TEXT(wxID_GPSFile, ARLabStitcherwxNewProjectWizard::ValidateGPSFile)
EVT_TEXT(wxID_OutputFile, ARLabStitcherwxNewProjectWizard::ValidateOutFile)
EVT_WIZARD_BEFORE_PAGE_CHANGED(wxID_NewProjectWizard, ARLabStitcherwxNewProjectWizard::DisableNextFirst)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(ARLabStitcherwxGPSFrame, GPSFrame)
EVT_PAINT(ARLabStitcherwxGPSFrame::OnPaint)
END_EVENT_TABLE()