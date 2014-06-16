#include"ARLabStitcherwx_app.h"
#include "base_wx/huginConfig.h"
bool ARLabStitcherwxApp:: OnInit()
{
	ARLabStitcherwxMainFrame* frame=new ARLabStitcherwxMainFrame((wxWindow*)NULL);
	frame->Show(true);
	SetTopWindow(frame);
	return true;


}