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


//BEGIN_EVENT_TABLE(ARLabStitcherwxMainFrame,wxFrame)
//	EVT_MENU(ID_Quit,)
//WND_EVENT_TABLE()
enum
{
	ID_Quit,
	ID_About,

};

BEGIN_EVENT_TABLE(ARLabStitcherwxMainFrame,MainFrame)
	EVT_MENU(wxID_New,ARLabStitcherwxMainFrame::newProcess)
END_EVENT_TABLE()