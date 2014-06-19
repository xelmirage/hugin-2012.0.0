///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ARLabStitcherwx.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menubarmain = new wxMenuBar( 0 );
	m_menufile = new wxMenu();
	wxMenuItem* m_menuItemNew;
	m_menuItemNew = new wxMenuItem( m_menufile, wxID_New, wxString( _("&New") ) , wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menuItemNew );
	
	m_menufile->AppendSeparator();
	
	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menufile, wxID_Exit, wxString( _("E&xit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menuItemExit );
	
	m_menubarmain->Append( m_menufile, _("&File") ); 
	
	m_menuEdit = new wxMenu();
	wxMenuItem* m_menuItemPorcess;
	m_menuItemPorcess = new wxMenuItem( m_menuEdit, wxID_Process, wxString( _("&Process") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItemPorcess );
	
	m_menubarmain->Append( m_menuEdit, _("&Edit") ); 
	
	this->SetMenuBar( m_menubarmain );
	
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter4 = new wxSplitterWindow( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter4->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::m_splitter4OnIdle ), NULL, this );
	
	m_panel15 = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_splitter3 = new wxSplitterWindow( m_panel15, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_3DBORDER );
	m_splitter3->SetSashGravity( 0.5 );
	m_splitter3->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::m_splitter3OnIdle ), NULL, this );
	
	m_panel13 = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxSize( 200,300 ), wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	m_listBoxPicList = new wxListBox( m_panel13, wxID_ListBoxPicList, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE ); 
	bSizer26->Add( m_listBoxPicList, 1, wxALL|wxEXPAND, 5 );
	
	m_listCtrlPicList = new wxListCtrl( m_panel13, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer26->Add( m_listCtrlPicList, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel13->SetSizer( bSizer26 );
	m_panel13->Layout();
	m_panel14 = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxALWAYS_SHOW_SB|wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmappreview = new wxStaticBitmap( m_panel14, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( m_bitmappreview, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	m_panel14->SetSizer( bSizer27 );
	m_panel14->Layout();
	bSizer27->Fit( m_panel14 );
	m_splitter3->SplitVertically( m_panel13, m_panel14, 0 );
	bSizer9->Add( m_splitter3, 1, wxEXPAND, 5 );
	
	
	m_panel15->SetSizer( bSizer9 );
	m_panel15->Layout();
	bSizer9->Fit( m_panel15 );
	m_panel5 = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter5 = new wxSplitterWindow( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter5->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrame::m_splitter5OnIdle ), NULL, this );
	
	m_panel7 = new wxPanel( m_splitter5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );
	
	m_textCtrlProgress = new wxTextCtrl( m_panel7, wxID_ANY, _("asdf"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer81->Add( m_textCtrlProgress, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel7->SetSizer( bSizer81 );
	m_panel7->Layout();
	bSizer81->Fit( m_panel7 );
	m_splitter5->Initialize( m_panel7 );
	bSizer6->Add( m_splitter5, 1, wxEXPAND, 5 );
	
	
	m_panel5->SetSizer( bSizer6 );
	m_panel5->Layout();
	bSizer6->Fit( m_panel5 );
	m_splitter4->SplitHorizontally( m_panel15, m_panel5, 0 );
	bSizer8->Add( m_splitter4, 1, wxEXPAND, 5 );
	
	
	m_panel6->SetSizer( bSizer8 );
	m_panel6->Layout();
	bSizer8->Fit( m_panel6 );
	bSizer7->Add( m_panel6, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

MainFrame::~MainFrame()
{
}
