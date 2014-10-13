#include"ARLabStitcherwx_app.h"
#include "base_wx/huginConfig.h"
bool ARLabStitcherwxApp:: OnInit()
{
	wxString ExeDir = getExePath(argv[0]);
	frame=new ARLabStitcherwxMainFrame((wxWindow*)NULL,ExeDir);
	frame->Show(true);
	SetTopWindow(frame);
	wxImage::AddHandler(new ::wxJPEGHandler);

	return true;


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