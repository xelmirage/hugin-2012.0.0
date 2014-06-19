#include "ARLabStitcherwxMainFrame.h"

ARLabStitcherwxMainFrame::ARLabStitcherwxMainFrame( wxWindow* parent ,wxString Dir)
	:
MainFrame( parent )
{
	m_execPanel= new MyExecPanel(MainFrame::m_splitter5);

	MainFrame::m_splitter5->SplitHorizontally( m_panel7, m_execPanel, 0 );
	ExeDir=Dir;
	//MainFrame::m_timerprocess->SetOwner(this);
}
void ARLabStitcherwxMainFrame::newProcess(wxCommandEvent& WXUNUSED(event))
{
	wxString cmd;
	//wxMessageBox("new");
	wxStringList fileNameList;
	wxStringList fileList;
	//::wxDirDialog
	wxDirDialog dd(this,_("Select directory"),"", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	//Í¼ÏñÄ¿Â¼
	if(dd.ShowModal()== wxID_CANCEL)
		return;

	sdir=dd.GetPath();
	wxMessageBox(sdir);

	//----------------GPS---------------------//
	::wxFileDialog fdGPS(this,_("Select GPS File"),"","","txt files (*.txt)|*.txt",wxFD_OPEN |wxFD_FILE_MUST_EXIST );
	if(fdGPS.ShowModal()== wxID_CANCEL)
		return;
	gpsfileName=fdGPS.GetPath();



	

	//cmd=ExeDir+wxT("\\gpsfilter -o ")+sdir+wxT("\\belts.log -g ")+gpsfileName+wxT(" -s ")+sdir;
	//cmd=wxT("exiftool -F -m -overwrite_original ")+sdir+wxT("\\*.jpg");
	//if (m_execPanel->ExecWithRedirect(cmd) == -1) 
	//	wxMessageBox(wxString::Format(_("Error running \n%s"), cmd.c_str()),
 //                        _("Error running command"),  wxICON_ERROR | wxOK );

	wxPathList list;
	::wxDir::GetAllFiles(sdir,&list,wxT("*.jpg"),wxDIR_FILES );

	MainFrame::m_listBoxPicList->Clear();
	MainFrame::m_listBoxPicList->InsertItems(list,0);


}
void ARLabStitcherwxMainFrame::ListBoxPicListClick(wxCommandEvent& e)
{
	wxImage preimg;
	float ratioh,ratiow,ratio;
	
	//preimg.Clear();
	preimg.LoadFile(e.GetString(),wxBITMAP_TYPE_JPEG);
	ratioh=preimg.GetHeight()/MainFrame::m_panel14->GetSize().GetHeight();
	ratiow=preimg.GetWidth()/MainFrame::m_panel14->GetSize().GetWidth();

	if(ratiow>ratioh)
	{
		ratio=ratiow;
		
	}
	else
	{
		ratio=ratioh;
		
	}

	MainFrame::m_bitmappreview->SetSize(preimg.GetWidth()/ratio,preimg.GetHeight()/ratio);
	preimg=preimg.Scale(preimg.GetWidth()/ratio,preimg.GetHeight()/ratio);
	MainFrame::m_bitmappreview->ClearBackground();
	MainFrame::m_bitmappreview->ResetConstraints();
	MainFrame::m_bitmappreview->SetBitmap(preimg);
}
void ARLabStitcherwxMainFrame::process(wxCommandEvent& WXUNUSED(event))
{
	std::string cmd;
	wxFileName beltlog(sdir+"belts.log");
//	MainFrame::m_timerprocess.Start(1000);
	if(!beltlog.FileExists())
	{
		cmd=ExeDir+wxT("\\gpsfilter -o ")+sdir+wxT("\\belts.log -g ")+gpsfileName+wxT(" -s ")+sdir;

		if (m_execPanel->ExecWithRedirect(cmd) == -1) 
		{
			wxMessageBox(wxString::Format(_("Error running \n%s"), cmd.c_str()),
				_("Error running command"),  wxICON_ERROR | wxOK );
			return;
		}
		//"gpsfilter -o "+sdir+"belts.log -g "+gpsfileName+" -s "+sdir,"processing GPSFilting");
	}
	else
	{
		//push_message("\n---------------\n["+run_time+"] skip filting......\n---------------\n");
		//textEdit->setText(textEdit->toPlainText()+tr("\n skip filting......\n\n"));
		//QMessageBox::information(NULL, "Existing results","skip GPSFilting", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	}


}
void ARLabStitcherwxMainFrame::count_time(::wxTimerEvent& e)
{
	time_count++;
	int day,hour,minute,second;
	day=time_count/24/3600;
	hour=(time_count/3600)%24;
	minute=(time_count/60)%60;
	second=time_count%60;

	run_time.clear();
	
	run_time+=wxString::Format(wxT("%d"),day)+wxT(" d ");
	run_time+=wxString::Format(wxT("%d"),hour)+wxT(" h ");
	run_time+=wxString::Format(wxT("%d"),minute)+wxT(" m ");
	run_time+=wxString::Format(wxT("%d"),second)+wxT(" s ");

	

	
}