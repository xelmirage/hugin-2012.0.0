#include "ARLabStitcherwxMainFrame.h"

ARLabStitcherwxMainFrame::ARLabStitcherwxMainFrame( wxWindow* parent )
	:
MainFrame( parent )
{
	m_execPanel= new MyExecPanel(MainFrame::m_splitter4);

	MainFrame::m_splitter4->SplitHorizontally( m_panel15, m_execPanel, 0 );
	
}
void ARLabStitcherwxMainFrame::newProcess(wxCommandEvent& WXUNUSED(event))
{
	string cmd;
	//wxMessageBox("new");
	::wxStringList fileNameList;
	::wxFileDialog fd(this,_("Select directory"),"","","JPEG files (*.xyz)|*.JPG",wxFD_CHANGE_DIR);

	//Í¼ÏñÄ¿Â¼
	if(fd.ShowModal()== wxID_CANCEL)
		return;

	sdir=fd.GetDirectory();
	wxMessageBox(sdir);

	//----------------GPS---------------------//
	::wxFileDialog fdGPS(this,_("Select GPS File"),"","","txt files (*.txt)|*.txt",wxFD_OPEN |wxFD_FILE_MUST_EXIST );
	if(fdGPS.ShowModal()== wxID_CANCEL)
		return;
	gpsfileName=fdGPS.GetPath();




	cmd="gpsfilter -o "+sdir+"belts.log -g "+gpsfileName+" -s "+sdir;
	if (m_execPanel->ExecWithRedirect(cmd) == -1) 
		wxMessageBox(wxString::Format(_("Error running \n%s"), cmd.c_str()),
                         _("Error running command"),  wxICON_ERROR | wxOK );

}