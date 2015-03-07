#ifndef __ARLabStitcherwxControlPointFrame__
#define __ARLabStitcherwxControlPointFrame__

/**
@file
Subclass of ControlPointFrame, which is generated by wxFormBuilder.
*/

#include "ARLabStitcherwx.h"
#include "base_wx/huginConfig.h"
#include "base_wx/wxPlatform.h"
#include "base_wx/platform.h"
#include<boost/lexical_cast.hpp>
#include <hugin_basic.h>

//// end generated include
#include <iostream>
#include<vector>
#include <wx\string.h>
#include "wx\msw\spinctrl.h"
//// end generated include
using namespace PT;
using namespace AppBase;

/** Implementing ControlPointFrame */
class ARLabStitcherwxControlPointFrame : public ControlPointFrame
{
	enum ImageRotation { ROT0 = 0, ROT90, ROT180, ROT270 };
public:
	/** Constructor */
	ARLabStitcherwxControlPointFrame(wxWindow* parent);
	void setPTO(wxString s);

private:
	wxString ptoFile;
	Panorama pano;
	HuginBase::CPointVector cpForLeftImg;
	std::vector<unsigned int> imgRightNrs;
	//// end generated class members
	DECLARE_EVENT_TABLE()
public:
	int getReady();
	int UpdateDisplay_disposed();
	void choiceLeftChanged(wxCommandEvent& ee);
	void choiceRightChanged(wxCommandEvent & ee);


private:
	void OnResize(wxSizeEvent& e);
	void setImage(wxStaticBitmap* m_staticBitmap, wxString imagePath, wxImage preimg);
	wxString imgLeftPath;
	wxString imgRightPath;
	bool isLeftImgReady;
	bool isRightImgReady;
	wxImage imgLeft, imgRight;

public:
	void UpdatePreview();
	void OnPaint(wxPaintEvent& ee);
	void paintCP(wxDC* dcLeft, wxDC* dcRight);
private:
	HuginBase::CPointVector CPToDraw;
	unsigned int imgLeftNr;
	unsigned int imgRightNr;
	std::vector<wxPoint> scaledCPLeft;
	std::vector<wxPoint> scaledCPRight;
	HuginBase::CPointVector scaledCP;
public:
	void OnMoveEnd(wxMoveEvent& ee);
	bool isReady;
	ARLabStitcherwxControlPointFrame::ImageRotation GetRot(double yaw, double pitch, double roll);
private:
	unsigned int bitmapSide;

};

#endif // __ARLabStitcherwxControlPointFrame__
