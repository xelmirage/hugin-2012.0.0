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
public:

};

DECLARE_APP(ARLabStitcherwxApp)
IMPLEMENT_APP(ARLabStitcherwxApp)

	
//	enum
//{
//    ID_Quit=1,
//	ID_About,
//};


//BEGIN_EVENT_TABLE(ARLabStitcherwxMainFrame,wxFrame)
//	EVT_MENU(ID_Quit,)
//WND_EVENT_TABLE()

BEGIN_EVENT_TABLE(ARLabStitcherwxMainFrame,MainFrame)
	EVT_MENU(wxID_New,ARLabStitcherwxMainFrame::newProcess)
	EVT_MENU(wxID_Process,ARLabStitcherwxMainFrame::processcmd)
	EVT_LISTBOX(wxID_ListBoxPicList,ARLabStitcherwxMainFrame::ListBoxPicListClick )
	EVT_TIMER(wxID_TimerProcess,ARLabStitcherwxMainFrame::count_time)
	EVT_END_PROCESS(-1, ARLabStitcherwxMainFrame::end_process)
END_EVENT_TABLE()
