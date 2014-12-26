
#ifndef _ARLABSTITCHERWXAPP_H
#define _ARLABSTITCHERWXAPP_H

#include "config.h"
#include "wx/wxprec.h"
#include "hugin_utils/utils.h"
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
	

	/** temporary working directory */
	wxString m_workDir;
	
	//// the model
	//Panorama pano;

	// folder for xrc (GUI resources)
	wxString m_xrcPrefix;
	// folder for data shared by CLI and GUI to enable separate packaging of CLI tools
	wxString m_DataDir;
	// folder for CLI tools
	wxString m_utilsBinDir;
public:
	static ARLabStitcherwxApp * Get();
	static ARLabStitcherwxMainFrame* getMainFrame();
	const wxString & GetXRCPath()
	{
		return m_xrcPrefix;
	}

	ARLabStitcherwxMainFrame* frame;

	static ARLabStitcherwxApp* m_this;
	
};

DECLARE_APP(ARLabStitcherwxApp)



//	enum
//{
//    ID_Quit=1,
//	ID_About,
//};







void RestoreFramePosition(wxTopLevelWindow * frame, const wxString & basename);
void StoreFramePosition(wxTopLevelWindow * frame, const wxString & basename);
bool str2double(wxString s, double & d);





#endif 