///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ARLabStitcherwx.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menubarmain = new wxMenuBar( 0 );
	m_menufile = new wxMenu();
	wxMenuItem* m_menuItemNew;
	m_menuItemNew = new wxMenuItem( m_menufile, wxID_New, wxString( _("新建工程") ) + wxT('\t') + wxT("&n"), wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menuItemNew );
	
	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem( m_menufile, wxID_ANY, wxString( _("打开工程") ) , wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menuItem13 );
	
	wxMenuItem* m_menuItem14;
	m_menuItem14 = new wxMenuItem( m_menufile, wxID_ANY, wxString( _("保存工程") ) , wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menuItem14 );
	
	wxMenuItem* m_menuItem15;
	m_menuItem15 = new wxMenuItem( m_menufile, wxID_ANY, wxString( _("另存为") ) , wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menuItem15 );
	
	m_menufile->AppendSeparator();
	
	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menufile, wxID_Exit, wxString( _("退出") ) + wxT('\t') + wxT("&x"), wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menuItemExit );
	
	m_menubarmain->Append( m_menufile, _("文件") ); 
	
	m_menuEdit = new wxMenu();
	wxMenuItem* m_menuItemPorcess;
	m_menuItemPorcess = new wxMenuItem( m_menuEdit, wxID_Process, wxString( _("显示航迹") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItemPorcess );
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("图像关联") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem4 );
	
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menuEdit, wxID_ANY, wxString( _("图像增强") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItem5 );
	
	m_menubarmain->Append( m_menuEdit, _("预处理") ); 
	
	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("影像匹配") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem6 );
	
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("平差优化") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem7 );
	
	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("图像融合") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem8 );
	
	wxMenuItem* m_menuItem9;
	m_menuItem9 = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("生成瓦片") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem9 );
	
	m_menubarmain->Append( m_menu3, _("影像拼接") ); 
	
	m_menu4 = new wxMenu();
	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( m_menu4, wxID_ANY, wxString( _("加载多光谱数据") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem11 );
	
	wxMenuItem* m_menuItem12;
	m_menuItem12 = new wxMenuItem( m_menu4, wxID_ANY, wxString( _("多光谱数据转化") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu4->Append( m_menuItem12 );
	
	m_menubarmain->Append( m_menu4, _("多光谱") ); 
	
	this->SetMenuBar( m_menubarmain );
	
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_panel6 = new wxPanel( this, wxID_Panel6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
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
	
	m_notebook4 = new wxNotebook( m_panel13, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel71 = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer82;
	bSizer82 = new wxBoxSizer( wxVERTICAL );
	
	m_listBoxPicList = new wxListBox( m_panel71, wxID_ListBoxPicList, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE ); 
	bSizer82->Add( m_listBoxPicList, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel71->SetSizer( bSizer82 );
	m_panel71->Layout();
	bSizer82->Fit( m_panel71 );
	m_notebook4->AddPage( m_panel71, _("a page"), false );
	
	bSizer26->Add( m_notebook4, 1, wxEXPAND | wxALL, 5 );
	
	
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
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	m_notebookProgressOut = new wxNotebook( m_panel5, wxID_NoteBookProgressOut, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel7 = new wxPanel( m_notebookProgressOut, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );
	
	m_textCtrlProgress = new wxTextCtrl( m_panel7, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE|wxTE_NOHIDESEL|wxTE_READONLY );
	bSizer81->Add( m_textCtrlProgress, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel7->SetSizer( bSizer81 );
	m_panel7->Layout();
	bSizer81->Fit( m_panel7 );
	m_notebookProgressOut->AddPage( m_panel7, _("a page"), false );
	
	bSizer6->Add( m_notebookProgressOut, 1, wxEXPAND | wxALL, 5 );
	
	m_gauge3 = new wxGauge( m_panel5, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH|wxGA_VERTICAL );
	m_gauge3->SetValue( 0 ); 
	bSizer6->Add( m_gauge3, 0, wxALL, 5 );
	
	
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
	m_timerprocess.SetOwner( this, wxID_TimerProcess );
	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_tool1 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/new.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool2 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/save.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool5 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/start1.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool14 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/pause.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool6 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/preview.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool7 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/airportsxia.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool8 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/doreconstruction.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool9 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/Dotriangulation .bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool10 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/stereoimage.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool11 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/rectifyimg.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool12 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/triangle.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool13 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/ruler.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->Realize(); 
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
}

MainFrame::~MainFrame()
{
}
