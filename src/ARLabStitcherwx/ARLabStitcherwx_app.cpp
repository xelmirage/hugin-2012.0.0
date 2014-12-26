#include"ARLabStitcherwx_app.h"
#include "base_wx/huginConfig.h"
bool ARLabStitcherwxApp:: OnInit()
{
	wxString ExeDir = getExePath(argv[0]);
	frame=new ARLabStitcherwxMainFrame((wxWindow*)NULL,ExeDir);
	frame->Show(true);
	SetTopWindow(frame);
	wxImage::AddHandler(new ::wxJPEGHandler);
	wxImage::InitStandardHandlers();
	m_this = this;
	
	wxString huginExeDir = getExePath(argv[0]);

	wxString huginRoot;
	wxFileName::SplitPath(huginExeDir, &huginRoot, NULL, NULL);

	m_xrcPrefix = huginRoot + wxT("/share/hugin/xrc/");
	m_DataDir = huginRoot + wxT("/share/hugin/data/");
	m_utilsBinDir = huginRoot + wxT("/bin/");
	
	
	
	
	return true;
	

}
ARLabStitcherwxApp * ARLabStitcherwxApp::Get()
{
	if (m_this) {
		return m_this;
	}
	else {
		DEBUG_FATAL("ARLabStitcherwxApp not yet created");
		DEBUG_ASSERT(m_this);
		return 0;
	}
}

ARLabStitcherwxMainFrame* ARLabStitcherwxApp::getMainFrame()
{
	if (m_this) {
		return m_this->frame;
	}
	else {
		return 0;
	}
}
ARLabStitcherwxApp* ARLabStitcherwxApp::m_this= 0;
IMPLEMENT_APP(ARLabStitcherwxApp)

void RestoreFramePosition(wxTopLevelWindow * frame, const wxString & basename)
{
}
void StoreFramePosition(wxTopLevelWindow * frame, const wxString & basename)
{
}
bool str2double(wxString s, double & d)
{
	try
	{
		d = boost::lexical_cast<double>(s);
	}
	catch (bad_lexical_cast &)
	{

		return false;
	}

	return true;
}

