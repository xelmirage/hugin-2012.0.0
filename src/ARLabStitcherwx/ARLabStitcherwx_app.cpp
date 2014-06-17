#include"ARLabStitcherwx_app.h"
#include "base_wx/huginConfig.h"
bool ARLabStitcherwxApp:: OnInit()
{
	wxString ExeDir = getExePath(argv[0]);
	ARLabStitcherwxMainFrame* frame=new ARLabStitcherwxMainFrame((wxWindow*)NULL,ExeDir);
	frame->Show(true);
	SetTopWindow(frame);
	wxImage::AddHandler(new ::wxJPEGHandler);
	return true;


}