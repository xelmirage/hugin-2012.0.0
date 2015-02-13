#pragma once
#include"ARLabStitcherwx.h"
#include <iostream>
#include<vector>
#include <string>
#include"hugin_utils/utils.h"
#include "base_wx/huginConfig.h"
#include "base_wx/wxPlatform.h"
class ARCPImageCtrl : public wxScrolledWindow
{
public:
	ARCPImageCtrl()
		: scaleFactor(1), fitToWindow(false)
	{};
	~ARCPImageCtrl();


	bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxT("panel"));






private:

	wxBitmap bitmap;
	std::string imageFilename;
	// size of displayed (probably scaled) image
	wxSize imageSize;
	// size of real image
	wxSize m_realSize;

	std::vector<hugin_utils::FDiff2D> points;

	// position of the point labels (in screen coordinates)
	std::vector<wxRect> m_labelPos;


	// this is only valid during MOVE_POINT
	unsigned int selectedPointNr;
	// valid during MOVE_POINT and CREATE_POINT
	hugin_utils::FDiff2D point;
	hugin_utils::FDiff2D newPoint;

	// only valid during SELECT_REGION
	wxRect region;
	// only valid during SELECT_DELETE_REGION
	hugin_utils::FDiff2D rectStartPos;
	enum EditorState { NO_IMAGE = 0, NO_SELECTION, KNOWN_POINT_SELECTED, NEW_POINT_SELECTED, SELECT_REGION, SELECT_DELETE_REGION };
	EditorState editState;
	bool m_showSearchArea;
	int m_searchRectWidth;

	hugin_utils::FDiff2D m_mousePos;
	wxPoint m_mouseScrollPos;

	bool m_showTemplateArea;
	int m_templateRectWidth;

	bool m_tempZoom;
	double m_savedScale;

	/// check if p is over a known point, if it is, pointNr contains
	/// the point
	EditorState isOccupied(wxPoint mousePos, const hugin_utils::FDiff2D & point, unsigned int & pointNr) const;





	std::vector<wxColour> pointColors;
	std::vector<wxColour> textColours;
	double scaleFactor;
	bool fitToWindow;

};

