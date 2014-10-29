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



BEGIN_EVENT_TABLE(ARLabStitcherwxMainFrame,MainFrame)
	EVT_MENU(wxID_New,ARLabStitcherwxMainFrame::newProcess)
	EVT_MENU(wxID_Process,ARLabStitcherwxMainFrame::processcmd)
	EVT_LISTBOX(wxID_ListBoxPicList,ARLabStitcherwxMainFrame::ListBoxPicListClick )
	EVT_TIMER(wxID_TimerProcess,ARLabStitcherwxMainFrame::count_time)
	EVT_END_PROCESS(-1, ARLabStitcherwxMainFrame::end_process)
	EVT_TOOL(wxID_NEW_PROJECT_TOOL,ARLabStitcherwxMainFrame::newProcessTool)
END_EVENT_TABLE()



BEGIN_EVENT_TABLE(ARLabStitcherwxNewProjectWizard, NewProjectWizard)
	EVT_BUTTON(wxID_OpenSourceDir, ARLabStitcherwxNewProjectWizard::OpenSourceDir)
	EVT_WIZARD_BEFORE_PAGE_CHANGED(wxID_NewProjectWizard, ARLabStitcherwxNewProjectWizard::DisableNextFirst)
	EVT_TEXT(wxID_SourceDir, ARLabStitcherwxNewProjectWizard::ValidateInputDir)
	
END_EVENT_TABLE()