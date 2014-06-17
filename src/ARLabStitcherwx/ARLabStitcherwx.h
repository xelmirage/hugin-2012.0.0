///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ARLABSTITCHERWX_H__
#define __ARLABSTITCHERWX_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/listbox.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/splitter.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_New 1000
#define wxID_Exit 1001
#define wxID_Process 1002
#define wxID_ListBoxPicList 1003

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxStatusBar* m_statusBar;
		wxMenuBar* m_menubarmain;
		wxMenu* m_menufile;
		wxMenu* m_menuEdit;
		wxPanel* m_panel6;
		wxSplitterWindow* m_splitter4;
		wxPanel* m_panel15;
		wxSplitterWindow* m_splitter3;
		wxPanel* m_panel13;
		wxListBox* m_listBoxPicList;
		wxListCtrl* m_listCtrlPicList;
		wxPanel* m_panel14;
		wxStaticBitmap* m_bitmappreview;
	
	public:
		wxBoxSizer* bSizer7;
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("ARLabStitcher"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 798,613 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrame();
		
		void m_splitter4OnIdle( wxIdleEvent& )
		{
			m_splitter4->SetSashPosition( 0 );
			m_splitter4->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::m_splitter4OnIdle ), NULL, this );
		}
		
		void m_splitter3OnIdle( wxIdleEvent& )
		{
			m_splitter3->SetSashPosition( 0 );
			m_splitter3->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::m_splitter3OnIdle ), NULL, this );
		}
	
};

#endif //__ARLABSTITCHERWX_H__
