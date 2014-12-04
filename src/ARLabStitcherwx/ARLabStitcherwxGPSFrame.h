#ifndef __ARLabStitcherwxGPSFrame__
#define __ARLabStitcherwxGPSFrame__

/**
@file
Subclass of GPSFrame, which is generated by wxFormBuilder.
*/

#include "ARLabStitcherwx.h"
#include "base_wx/huginConfig.h"
#include "base_wx/wxPlatform.h"
#include "base_wx/platform.h"
#include <wx/wxprec.h>
#include<wx/dir.h>
#include<wx/process.h>
#include<boost/lexical_cast.hpp>
//// end generated include

/** Implementing GPSFrame */
class ARLabStitcherwxGPSFrame : public GPSFrame
{
public:
	/** Constructor */
	ARLabStitcherwxGPSFrame(wxWindow* parent);
	//// end generated class members
	void OnPaint(wxPaintEvent& event)
	{
		wxPaintDC dc(this);
		dc.SetPen(*wxBLACK_PEN);
		dc.SetBrush(*wxRED_BRUSH);
		dc.DrawLine(40, 30, 40, 120);
	}
	void process();

	DECLARE_EVENT_TABLE()
};


#endif // __ARLabStitcherwxGPSFrame__