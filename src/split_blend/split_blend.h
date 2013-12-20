#include <hugin_config.h>
#include <hugin_version.h>
#include "panoinc_WX.h"
#include "panoinc.h"


#include <fstream>
#include <sstream>
#ifdef WIN32
 #include <getopt.h>
#else
 #include <unistd.h>
#endif

#include <hugin_basic.h>
#include <hugin_utils/stl_utils.h>
#include "PT/Panorama.h"
#include "PT/utils.h"

#include<hugin1/base_wx/huginConfig.cpp>

#include "base_wx/platform.h"
#include "base_wx/wxPlatform.h"
#include "SplitBlendPanel.h"
//#include<boost/lexical_cast.hpp>
// somewhere SetDesc gets defined.. this breaks wx/cmdline.h on OSX
#ifdef SetDesc
#undef SetDesc
#endif

#include <wx/cmdline.h>
using namespace std;
using namespace hugin_utils;
using namespace HuginBase;
using namespace AppBase;


class SplitBlendFrame: public wxFrame
{
public:
    SplitBlendFrame(wxWindow * parent, const wxString& title, const wxPoint& pos, const wxSize& size);

    bool SplitBlend(wxString scriptFile, wxString outname,
                       HuginBase::PanoramaMakefilelibExport::PTPrograms progs,
                       bool doDeleteOnExit);
	
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    /** sets, if existing output file should be automatic overwritten */
    void SetOverwrite(bool doOverwrite);
	bool m_isStitching;
	
private:
	SplitBlendPanel* m_stitchPanel;
    
    wxString m_scriptFile;
    bool m_deleteOnExit;

    void OnProcessTerminate(wxProcessEvent & event);
    void OnCancel(wxCommandEvent & event);
	
   // SplitBlendPanel * m_stitchPanel;

    DECLARE_EVENT_TABLE()
};

enum
{
    ID_Quit = 1,
    ID_About   
};

BEGIN_EVENT_TABLE(SplitBlendFrame, wxFrame)
    EVT_MENU(ID_Quit,  SplitBlendFrame::OnQuit)
    EVT_MENU(ID_About, SplitBlendFrame::OnAbout)
    EVT_BUTTON(wxID_CANCEL, SplitBlendFrame::OnCancel)
    EVT_END_PROCESS(-1, SplitBlendFrame::OnProcessTerminate)
END_EVENT_TABLE()
class split_blend_App : public wxApp
{
	public:

    /** ctor.
     */
    split_blend_App();

    /** dtor.
     */
    virtual ~split_blend_App();

    /** pseudo constructor. with the ability to fail gracefully.
     */
    virtual bool OnInit();

    /** just for testing purposes */
    virtual int OnExit();
    
#ifdef __WXMAC__
    /** the wx calls this method when the app gets "Open file" AppleEvent */
    void MacOpenFile(const wxString &fileName);
#endif

private:
	vector<SplitBlendFrame*> frames;
    wxLocale m_locale;
#ifdef __WXMAC__
    wxString m_macFileNameToOpenOnStart;
#endif

};


