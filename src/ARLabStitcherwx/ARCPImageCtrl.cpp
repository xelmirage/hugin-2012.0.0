#include "ARCPImageCtrl.h"





ARCPImageCtrl::~ARCPImageCtrl()
{
}


bool ARCPImageCtrl::Create(wxWindow * parent, wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name)
{
	wxScrolledWindow::Create(parent, id, pos, size, style, name);
	selectedPointNr = 0;
	editState = NO_IMAGE;
	scaleFactor = 1;
	fitToWindow = false;
	m_showSearchArea = false;
	m_searchRectWidth = 0;
	m_showTemplateArea = false;
	m_templateRectWidth = 0;
	m_tempZoom = false;
	m_savedScale = 1;
	//m_editPanel = 0;
	//m_imgRotation = ROT0;
	wxString filename;

	// TODO: define custom, light background colors.
	pointColors.push_back(wxTheColourDatabase->Find(wxT("BLUE")));
	textColours.push_back(wxTheColourDatabase->Find(wxT("WHITE")));

	pointColors.push_back(wxTheColourDatabase->Find(wxT("GREEN")));
	textColours.push_back(wxTheColourDatabase->Find(wxT("WHITE")));

	pointColors.push_back(wxTheColourDatabase->Find(wxT("CYAN")));
	textColours.push_back(wxTheColourDatabase->Find(wxT("BLACK")));
	//    pointColors.push_back(wxTheColourDatabase->Find(wxT("MAGENTA")));
	pointColors.push_back(wxTheColourDatabase->Find(wxT("GOLD")));
	textColours.push_back(wxTheColourDatabase->Find(wxT("BLACK")));

	//    pointColors.push_back(wxTheColourDatabase->Find(wxT("ORANGE"));
	pointColors.push_back(wxTheColourDatabase->Find(wxT("NAVY")));
	textColours.push_back(wxTheColourDatabase->Find(wxT("WHITE")));

	//    pointColors.push_back(wxTheColourDatabase->Find(wxT("FIREBRICK")));
	//    pointColors.push_back(wxTheColourDatabase->Find(wxT("SIENNA")));

	pointColors.push_back(wxTheColourDatabase->Find(wxT("DARK TURQUOISE")));
	textColours.push_back(wxTheColourDatabase->Find(wxT("BLACK")));

	pointColors.push_back(wxTheColourDatabase->Find(wxT("SALMON")));
	textColours.push_back(wxTheColourDatabase->Find(wxT("BLACK")));

	pointColors.push_back(wxTheColourDatabase->Find(wxT("MAROON")));
	textColours.push_back(wxTheColourDatabase->Find(wxT("BLACK")));

	pointColors.push_back(wxTheColourDatabase->Find(wxT("KHAKI")));
	textColours.push_back(wxTheColourDatabase->Find(wxT("BLACK")));

	m_searchRectWidth = 120;
	m_mouseInWindow = false;
	m_forceMagnifier = false;
	m_timer.SetOwner(this);

	return true;
}