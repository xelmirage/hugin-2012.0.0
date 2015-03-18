///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ARLABSTITCHERWX_H__
#define __ARLABSTITCHERWX_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/statbmp.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>
#include <wx/gauge.h>
#include <wx/timer.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/wizard.h>
#include <wx/dynarray.h>
WX_DEFINE_ARRAY_PTR( wxWizardPageSimple*, WizardPages );
#include <wx/choice.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_New 1000
#define wxID_Exit 1001
#define wxID_menuItemProcess 1002
#define wxID_menuItemPreProcess 1003
#define wxID_menuItemFindCP 1004
#define wxID_menuItemOptimise 1005
#define wxID_menuItemAutoCrop 1006
#define wxID_menuItemMerge 1007
#define wxID_menuItemSuperOverlay 1008
#define wxID_menuItemMSPrerocess 1009
#define wxID_menuItemMSSecond 1010
#define wxID_Panel6 1011
#define wxID_ListBoxPicList 1012
#define wxID_panelPreview 1013
#define wxID_NoteBookProgressOut 1014
#define wxID_TimerProcess 1015
#define wxID_toolBarMain 1016
#define wxID_toolNew 1017
#define wxID_ToolStart 1018
#define wxID_toolPause 1019
#define wxID_toolStop 1020
#define wxID_toolShowTrack 1021
#define wxID_toolSuperOverLay 1022
#define wxID_ShowKML 1023
#define wxID_NewProjectWizard 1024
#define wxID_WizardPage01 1025
#define wxID_SourceDir 1026
#define wxID_InputDirHint 1027
#define wxID_OpenSourceDir 1028
#define wxID_WizardPage02 1029
#define wxID_GPSFile 1030
#define wxID_GPSHint 1031
#define wxID_OpenGPSFile 1032
#define wxID_WizardPage03 1033
#define wxID_OutputFile 1034
#define wxID_OutputFileHint 1035
#define wxID_OpenOutputFile 1036
#define wxID_choiceLeft 1037
#define wxID_bitmapLeft 1038
#define wxID_choiceRight 1039
#define wxID_bitmapRight 1040

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* m_menubarmain;
		wxMenu* m_menufile;
		wxMenu* m_menuEdit;
		wxMenu* m_menu3;
		wxMenu* m_menuMultiSpec;
		wxPanel* m_panel6;
		wxSplitterWindow* m_splitter4;
		wxPanel* m_panel15;
		wxSplitterWindow* m_splitter3;
		wxPanel* m_panel13;
		wxNotebook* m_notebook4;
		wxPanel* m_panel71;
		wxListBox* m_listBoxPicList;
		wxPanel* m_panel8;
		wxListBox* m_listBoxExif;
		wxPanel* m_panel14;
		wxStaticBitmap* m_bitmappreview;
		wxPanel* m_panel5;
		wxNotebook* m_notebookProgressOut;
		wxPanel* m_panel7;
		wxTextCtrl* m_textCtrlProgress;
		wxGauge* m_gauge3;
		wxTimer m_timerprocess;
		wxToolBar* m_toolBarMain;
		wxToolBarToolBase* m_toolNew; 
		wxToolBarToolBase* m_tool2; 
		wxToolBarToolBase* m_toolStart; 
		wxToolBarToolBase* m_toolPause; 
		wxToolBarToolBase* m_toolStop; 
		wxToolBarToolBase* m_toolShowTrack; 
		wxToolBarToolBase* m_toolSuperOverLay; 
		wxToolBarToolBase* m_toolShowKML; 
		wxStatusBar* m_statusBar;
	
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

///////////////////////////////////////////////////////////////////////////////
/// Class NewProjectWizard
///////////////////////////////////////////////////////////////////////////////
class NewProjectWizard : public wxWizard 
{
	private:
	
	protected:
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrlSourceDir;
		wxStaticText* m_staticTextInputDirHint;
		wxButton* m_buttonOpenSourceDir;
		wxStaticText* m_staticText31;
		wxTextCtrl* m_textCtrlGPSFile;
		wxStaticText* m_staticTextGPSHint;
		wxButton* m_buttonOpenGPSFile;
		wxStaticText* m_staticText32;
		wxTextCtrl* m_textCtrlOutputFile;
		wxStaticText* m_staticTextOutputFileHint;
		wxButton* m_buttonOpenOutputFile;
	
	public:
		
		NewProjectWizard( wxWindow* parent, wxWindowID id = wxID_NewProjectWizard, const wxString& title = _("新建工程向导"), const wxBitmap& bitmap = wxNullBitmap, const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_DIALOG_STYLE );
		WizardPages m_pages;
		~NewProjectWizard();
	
		bool isMS;
};

///////////////////////////////////////////////////////////////////////////////
/// Class GPSFrame
///////////////////////////////////////////////////////////////////////////////
class GPSFrame : public wxFrame 
{
	private:
	
	protected:
	
	public:
		
		GPSFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("GPS_Points"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_NO_TASKBAR|wxTAB_TRAVERSAL );
		
		~GPSFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ControlPointFrame
///////////////////////////////////////////////////////////////////////////////
class ControlPointFrame : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panelLeft;
		wxChoice* m_choiceLeft;
		wxStaticBitmap* m_bitmapLeft;
		wxPanel* m_panelRight;
		wxChoice* m_choiceRight;
		wxStaticBitmap* m_bitmapRight;
	
	public:
		
		ControlPointFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 573,393 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~ControlPointFrame();
	
};

#endif //__ARLABSTITCHERWX_H__
