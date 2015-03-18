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


EVT_LISTBOX(wxID_ListBoxPicList, ARLabStitcherwxMainFrame::ListBoxClicked)
EVT_TIMER(wxID_TimerProcess, ARLabStitcherwxMainFrame::count_time)
EVT_END_PROCESS(-1, ARLabStitcherwxMainFrame::end_process)


EVT_TOOL(wxID_toolNew, ARLabStitcherwxMainFrame::newProjectTool)
EVT_TOOL(wxID_toolShowTrack, ARLabStitcherwxMainFrame::showTrack)
EVT_TOOL(wxID_ShowKML, ARLabStitcherwxMainFrame::showTrack)
EVT_TOOL(wxID_toolSuperOverLay, ARLabStitcherwxMainFrame::generateSuperOverlay)
EVT_TOOL(wxID_ToolStart, ARLabStitcherwxMainFrame::processcmd)
EVT_TOOL(wxID_toolPause, ARLabStitcherwxMainFrame::pauseProcess)
EVT_TOOL(wxID_toolStop, ARLabStitcherwxMainFrame::stopProcess)

EVT_MENU(wxID_menuItemProcess, ARLabStitcherwxMainFrame::menuProcess)
EVT_MENU(wxID_menuItemPreProcess, ARLabStitcherwxMainFrame::preProcess)
EVT_MENU(wxID_menuItemFindCP, ARLabStitcherwxMainFrame::findCP)
EVT_MENU(wxID_New, ARLabStitcherwxMainFrame::newProjectTool)
EVT_MENU(wxID_menuItemOptimise,ARLabStitcherwxMainFrame::menuOptimise)
EVT_MENU(wxID_menuItemAutoCrop,ARLabStitcherwxMainFrame::menuCrop)
EVT_MENU(wxID_menuItemMerge,ARLabStitcherwxMainFrame::blend)
EVT_MENU(wxID_menuItemSuperOverlay,ARLabStitcherwxMainFrame::menuSuperOverlay)
EVT_MENU(wxID_menuItemMSPrerocess,ARLabStitcherwxMainFrame::menuMSPreProcess)
EVT_MENU(wxID_menuItemMSSecond, ARLabStitcherwxMainFrame::menuMSSecond)
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
EVT_SIZE(ARLabStitcherwxGPSFrame::OnResize)
EVT_ERASE_BACKGROUND(ARLabStitcherwxGPSFrame::OnErase)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ARLabStitcherwxControlPointFrame, ControlPointFrame)
EVT_CHOICE(wxID_choiceLeft, ARLabStitcherwxControlPointFrame::choiceLeftChanged)
EVT_CHOICE(wxID_choiceRight, ARLabStitcherwxControlPointFrame::choiceRightChanged)
EVT_SIZE(ARLabStitcherwxControlPointFrame::OnResize)
EVT_PAINT(ARLabStitcherwxControlPointFrame::OnPaint)
EVT_MOVE_END(ARLabStitcherwxControlPointFrame::OnMoveEnd)
END_EVENT_TABLE()