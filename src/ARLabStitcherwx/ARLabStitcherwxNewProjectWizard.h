﻿#ifndef __ARLabStitcherwxNewProjectWizard__
#define __ARLabStitcherwxNewProjectWizard__

/**
@file
Subclass of NewProjectWizard, which is generated by wxFormBuilder.
*/

#include "ARLabStitcherwx.h"
#include "base_wx/huginConfig.h"
#include "base_wx/wxPlatform.h"
#include "base_wx/platform.h"
#include <wx/wxprec.h>
#include<wx/dir.h>
#include<wx/process.h>
#include<boost/lexical_cast.hpp>
#include <hugin1/base_wx/MyExternalCmdExecDialog.h>
//// end generated include

/** Implementing NewProjectWizard */
class ARLabStitcherwxNewProjectWizard : public NewProjectWizard
{
	public:
		/** Constructor */
		ARLabStitcherwxNewProjectWizard( wxWindow* parent );
	//// end generated class members
		wxString sdir;
		wxString gpsfileName;
		wxString gpath;
		wxString outfileName;
private:
	DECLARE_EVENT_TABLE()
	void OpenSourceDir(wxCommandEvent& WXUNUSED(event));
	void OpenGPSFile(wxCommandEvent& WXUNUSED(event));
	void OpenOutputFile(wxCommandEvent& WXUNUSED(event));

	void DisableNextFirst(wxWizardEvent& WXUNUSED(event));
	
	void ValidateInputDir(wxCommandEvent& WXUNUSED(event));
	void ValidateGPSFile(wxCommandEvent& WXUNUSED(event));
	void ValidateOutFile(wxCommandEvent& WXUNUSED(event));
};

#endif // __ARLabStitcherwxNewProjectWizard__