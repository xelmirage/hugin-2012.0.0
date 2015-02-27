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
	wxMenuItem* m_menuItemProcess;
	m_menuItemProcess = new wxMenuItem( m_menuEdit, wxID_menuItemProcess, wxString( _("自动处理") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItemProcess );
	
	m_menuEdit->AppendSeparator();
	
	wxMenuItem* m_menuItemPreProcess;
	m_menuItemPreProcess = new wxMenuItem( m_menuEdit, wxID_menuItemPreProcess, wxString( _("预处理") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItemPreProcess );
	
	wxMenuItem* m_menuItemFindCP;
	m_menuItemFindCP = new wxMenuItem( m_menuEdit, wxID_menuItemFindCP, wxString( _("寻找特征点") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuEdit->Append( m_menuItemFindCP );
	
	m_menubarmain->Append( m_menuEdit, _("处理") ); 
	
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
	m_panel8 = new wxPanel( m_notebook4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_listBoxExif = new wxListBox( m_panel8, wxID_ListBoxPicList, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE ); 
	bSizer12->Add( m_listBoxExif, 0, wxALL|wxEXPAND|wxSHAPED, 5 );
	
	
	m_panel8->SetSizer( bSizer12 );
	m_panel8->Layout();
	bSizer12->Fit( m_panel8 );
	m_notebook4->AddPage( m_panel8, _("a page"), true );
	
	bSizer26->Add( m_notebook4, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel13->SetSizer( bSizer26 );
	m_panel13->Layout();
	m_panel14 = new wxPanel( m_splitter3, wxID_panelPreview, wxDefaultPosition, wxDefaultSize, wxALWAYS_SHOW_SB|wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );
	
	m_bitmappreview = new wxStaticBitmap( m_panel14, wxID_ANY, wxNullBitmap, wxPoint( 0,0 ), wxDefaultSize, 0 );
	bSizer27->Add( m_bitmappreview, 1, wxALIGN_LEFT|wxALIGN_TOP|wxALL, 5 );
	
	
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
	m_tool_new = m_toolBar1->AddTool( wxID_NEW_PROJECT_TOOL, _("新建工程"), wxBitmap( wxT("images/new.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_tool2 = m_toolBar1->AddTool( wxID_ANY, _("tool"), wxBitmap( wxT("images/save.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolStart = m_toolBar1->AddTool( wxID_ToolStart, _("tool"), wxBitmap( wxT("images/start1.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolShowTrack = m_toolBar1->AddTool( wxID_toolShowTrack, _("tool"), wxBitmap( wxT("images/airportsxia.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolSuperOverLay = m_toolBar1->AddTool( wxID_SuperOverLay, _("tool"), wxBitmap( wxT("images/Dotriangulation .bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolShowKML = m_toolBar1->AddTool( wxID_ShowKML, _("tool"), wxBitmap( wxT("images/stereoimage.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
	
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->Realize(); 
	
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
}

MainFrame::~MainFrame()
{
}

NewProjectWizard::NewProjectWizard( wxWindow* parent, wxWindowID id, const wxString& title, const wxBitmap& bitmap, const wxPoint& pos, long style ) 
{
	this->Create( parent, id, title, bitmap, pos, style );
	this->SetSizeHints( wxSize( 600,400 ), wxDefaultSize );
	
	wxWizardPageSimple* m_wizPage01 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage01 );
	
	m_wizPage01->SetMinSize( wxSize( 600,400 ) );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	bSizer10->SetMinSize( wxSize( 600,400 ) ); 
	m_staticText3 = new wxStaticText( m_wizPage01, wxID_ANY, _("待拼接图像路径"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer10->Add( m_staticText3, 0, wxALL, 5 );
	
	m_textCtrlSourceDir = new wxTextCtrl( m_wizPage01, wxID_SourceDir, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxDOUBLE_BORDER );
	m_textCtrlSourceDir->SetMinSize( wxSize( 350,-1 ) );
	
	bSizer10->Add( m_textCtrlSourceDir, 0, wxALL, 5 );
	
	m_staticTextInputDirHint = new wxStaticText( m_wizPage01, wxID_InputDirHint, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	m_staticTextInputDirHint->Wrap( -1 );
	m_staticTextInputDirHint->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT ) );
	
	bSizer10->Add( m_staticTextInputDirHint, 0, wxALL, 5 );
	
	m_buttonOpenSourceDir = new wxButton( m_wizPage01, wxID_OpenSourceDir, _("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_buttonOpenSourceDir, 0, wxALL, 5 );
	
	
	m_wizPage01->SetSizer( bSizer10 );
	m_wizPage01->Layout();
	bSizer10->Fit( m_wizPage01 );
	wxWizardPageSimple* m_wizPage02 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage02 );
	
	m_wizPage02->SetMinSize( wxSize( 600,400 ) );
	
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxVERTICAL );
	
	bSizer101->SetMinSize( wxSize( 600,400 ) ); 
	m_staticText31 = new wxStaticText( m_wizPage02, wxID_ANY, _("GPS记录文件路径"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	bSizer101->Add( m_staticText31, 0, wxALL, 5 );
	
	m_textCtrlGPSFile = new wxTextCtrl( m_wizPage02, wxID_GPSFile, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxDOUBLE_BORDER );
	m_textCtrlGPSFile->SetMinSize( wxSize( 350,-1 ) );
	
	bSizer101->Add( m_textCtrlGPSFile, 0, wxALL, 5 );
	
	m_staticTextGPSHint = new wxStaticText( m_wizPage02, wxID_GPSHint, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGPSHint->Wrap( -1 );
	bSizer101->Add( m_staticTextGPSHint, 0, wxALL, 5 );
	
	m_buttonOpenGPSFile = new wxButton( m_wizPage02, wxID_OpenGPSFile, _("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( m_buttonOpenGPSFile, 0, wxALL, 5 );
	
	
	m_wizPage02->SetSizer( bSizer101 );
	m_wizPage02->Layout();
	bSizer101->Fit( m_wizPage02 );
	wxWizardPageSimple* m_wizPage03 = new wxWizardPageSimple( this );
	m_pages.Add( m_wizPage03 );
	
	m_wizPage03->SetMinSize( wxSize( 600,400 ) );
	
	wxBoxSizer* bSizer102;
	bSizer102 = new wxBoxSizer( wxVERTICAL );
	
	bSizer102->SetMinSize( wxSize( 600,400 ) ); 
	m_staticText32 = new wxStaticText( m_wizPage03, wxID_ANY, _("目标文件名"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText32->Wrap( -1 );
	bSizer102->Add( m_staticText32, 0, wxALL, 5 );
	
	m_textCtrlOutputFile = new wxTextCtrl( m_wizPage03, wxID_OutputFile, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxDOUBLE_BORDER );
	m_textCtrlOutputFile->SetMinSize( wxSize( 350,-1 ) );
	
	bSizer102->Add( m_textCtrlOutputFile, 0, wxALL, 5 );
	
	m_staticTextOutputFileHint = new wxStaticText( m_wizPage03, wxID_OutputFileHint, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOutputFileHint->Wrap( -1 );
	bSizer102->Add( m_staticTextOutputFileHint, 0, wxALL, 5 );
	
	m_buttonOpenOutputFile = new wxButton( m_wizPage03, wxID_OpenOutputFile, _("浏览"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer102->Add( m_buttonOpenOutputFile, 0, wxALL, 5 );
	
	
	m_wizPage03->SetSizer( bSizer102 );
	m_wizPage03->Layout();
	bSizer102->Fit( m_wizPage03 );
	
	this->Centre( wxBOTH );
	
	for ( unsigned int i = 1; i < m_pages.GetCount(); i++ )
	{
		m_pages.Item( i )->SetPrev( m_pages.Item( i - 1 ) );
		m_pages.Item( i - 1 )->SetNext( m_pages.Item( i ) );
	}
}

NewProjectWizard::~NewProjectWizard()
{
	m_pages.Clear();
}

GPSFrame::GPSFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	
	this->Centre( wxBOTH );
}

GPSFrame::~GPSFrame()
{
}

ControlPointFrame::ControlPointFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxHORIZONTAL );
	
	m_panelLeft = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerLeft;
	bSizerLeft = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_choiceLeftChoices;
	m_choiceLeft = new wxChoice( m_panelLeft, wxID_choiceLeft, wxDefaultPosition, wxDefaultSize, m_choiceLeftChoices, 0 );
	m_choiceLeft->SetSelection( 0 );
	bSizerLeft->Add( m_choiceLeft, 0, wxALL, 5 );
	
	m_bitmapLeft = new wxStaticBitmap( m_panelLeft, wxID_bitmapLeft, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerLeft->Add( m_bitmapLeft, 0, wxALL, 5 );
	
	
	m_panelLeft->SetSizer( bSizerLeft );
	m_panelLeft->Layout();
	bSizerLeft->Fit( m_panelLeft );
	bSizerMain->Add( m_panelLeft, 1, wxEXPAND | wxALL, 5 );
	
	m_panelRight = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerRight;
	bSizerRight = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_choiceRightChoices;
	m_choiceRight = new wxChoice( m_panelRight, wxID_choiceRight, wxDefaultPosition, wxDefaultSize, m_choiceRightChoices, 0 );
	m_choiceRight->SetSelection( 0 );
	bSizerRight->Add( m_choiceRight, 0, wxALL, 5 );
	
	m_bitmapRight = new wxStaticBitmap( m_panelRight, wxID_bitmapRight, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerRight->Add( m_bitmapRight, 0, wxALL, 5 );
	
	
	m_panelRight->SetSizer( bSizerRight );
	m_panelRight->Layout();
	bSizerRight->Fit( m_panelRight );
	bSizerMain->Add( m_panelRight, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizerMain );
	this->Layout();
	
	this->Centre( wxBOTH );
}

ControlPointFrame::~ControlPointFrame()
{
}
