#include "ARLabStitcherwxMainFrame.h"

ARLabStitcherwxMainFrame::ARLabStitcherwxMainFrame( wxWindow* parent ,wxString Dir)
	:
MainFrame( parent )
{
	m_execPanel= new MyExecPanel(MainFrame:: m_notebookProgressOut);
	//m_execPanel->SetId(wxID_execPanel);
	
	//MainFrame::m_splitter5->SplitHorizontally( m_panel7, m_execPanel, 0 );
	ExeDir=Dir;
	MainFrame::m_notebookProgressOut->SetPageText(0,wxT("Working Progress"));
	//MainFrame::m_timerprocess->SetOwner(this);
	MainFrame::m_notebookProgressOut->AddPage(m_execPanel,wxT("Program Progress"));
	//::wxMessageBox(m_execPanel->GetParent()->GetName());

	MainFrame::m_notebookProgressOut->
		GetEventHandler()->
		Bind(wxEVT_END_PROCESS,&ARLabStitcherwxMainFrame::throw_to_parent,this);

	MainFrame::m_panel5->
		GetEventHandler()->
		Bind(wxEVT_END_PROCESS,&ARLabStitcherwxMainFrame::throw_to_parent,this);

	MainFrame::m_splitter4->
		GetEventHandler()->
		Bind(wxEVT_END_PROCESS,&ARLabStitcherwxMainFrame::throw_to_parent,this);

	MainFrame::m_panel6->
		GetEventHandler()->
		Bind(wxEVT_END_PROCESS,&ARLabStitcherwxMainFrame::throw_to_parent,this);
	
	

	phasename[0]="gpsfilter";
	phasename[1]="cpfind";
	phasename[2]="cpclean";
	phasename[3]="linefind";
	phasename[4]="checkpto";
	phasename[5]="optimise";
	phasename[6]="modify";
	phasename[7]="nona";
	phasename[8]="blend";

	sdir="f:\\kl\\part";
	gpsfileName="f:\\kl\\part\\02.txt";
	
}
void ARLabStitcherwxMainFrame::throw_to_parent(wxProcessEvent& e)
{
	
	if(((wxWindow*)e.GetEventObject())->GetParent())
	{
		//wxMessageBox(((wxWindow*)e.GetEventObject())->GetName()+"\n"+((wxWindow*)e.GetEventObject())->GetParent()->GetName());
		e.SetEventObject(((wxWindow*)e.GetEventObject())->GetParent());
		((wxWindow*)e.GetEventObject())->GetParent()->GetParent()->GetEventHandler()->ProcessEvent( e );
	}
}
void ARLabStitcherwxMainFrame::newProcess(wxCommandEvent& WXUNUSED(event))
{
	wxString cmd;
	//wxMessageBox("new");
	wxStringList fileNameList;
	wxStringList fileList;
	//::wxDirDialog
	wxDirDialog dd(this,_("Select directory"),"", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	//图像目录
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
void ARLabStitcherwxMainFrame::processcmd(wxCommandEvent& WXUNUSED(event))
{
	phase=-1;
	process();
	
	//MainFrame::m_timerprocess.Stop();

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

void ARLabStitcherwxMainFrame::push_message(wxString message)
{
	m_textCtrlProgress->SetValue(
		m_textCtrlProgress->GetValue()+message);
}
int ARLabStitcherwxMainFrame::execexternal(wxString command,wxString message)
{
	push_message("\n---------------\n["+run_time+"] start "+message+"\n---------------\n");
	if (m_execPanel->ExecWithRedirect(command) == -1) 
	{
		wxMessageBox(wxString::Format(_("Error running \n%s"), command.c_str()),
			_("Error running command"),  wxICON_ERROR | wxOK );
		return -1;
	}

	//push_message("\n---------------\n["+run_time+"] "+message+" finished\n---------------\n");

	return 0;
}
void ARLabStitcherwxMainFrame::end_process(::wxProcessEvent& e)
{
	
	push_message("\n---------------\n["+run_time+"] "+phasename[phase]+" finished\n---------------\n");
	process();
	//wxMessageBox("\n---------------\n["+run_time+"] GPSFILTER finished\n---------------\n");
}

void ARLabStitcherwxMainFrame::process(void)
{
	std::string cmd;
	wxFileName beltlog(sdir+"\\belts.log");
	wxFileName stitch(sdir+"\\stitch.pto");
	wxFileName stitch_cp(sdir+"\\stitch_cp.pto");
	wxFileName stitch_cp_clean(sdir+"//stich_cp_clean.pto");
	wxFileName stitch_cp_clean_line(sdir+"//stich_cp_clean_linefind.pto");

	++phase;
	switch (phase)
	{
	case 0://GPSFilting
		MainFrame::m_textCtrlProgress->Clear();
		this->m_execPanel->ClearText();
		time_count=0;
		MainFrame::m_timerprocess.Start(1000);
		::Sleep(1200);
		if(!beltlog.FileExists())
		{
			cmd=ExeDir+wxT("\\gpsfilter -o ")+sdir+wxT("\\belts.log -g ")+gpsfileName+wxT(" -s ")+sdir;

			if (execexternal(cmd,wxT("Filting GPS data"))!=0)
				return;
			//"gpsfilter -o "+sdir+"belts.log -g "+gpsfileName+" -s "+sdir,"processing GPSFilting");
			break;
		}
		else
		{
			push_message(wxT("\n---------------\n[")+run_time+wxT("] skip filting......\n---------------\n"));
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip filting......\n\n"));
			//QMessageBox::information(NULL, "Existing results","skip GPSFilting", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		}
		
	case 1://CPFind

		

		if(!stitch.FileExists())
		{
			cmd=ExeDir+wxT("\\pto_gen ")+sdir+wxT("\\*.jpg -o")+sdir+wxT("\\stitch.pto --gps -f 1");
			if(execexternal(cmd,wxT("generating progejc"))!=0)
			{
				return;
			}
			break;

		}
		else
		{
			push_message("\n---------------\n["+run_time+"] skip generating project......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip generating pto......\n\n"));

		}


	case 2:

		

		if(!stitch_cp.FileExists())
		{
			cmd=ExeDir+wxT("\\cpfindgps001 -o ")+sdir+wxT("\\stitch_cp.pto ")+sdir+wxT("\\stitch.pto --gps");
			if(execexternal(cmd,wxT("Finding Control Points"))!=0)
			{
				return;
			}
				break;
		}
		else
		{
			push_message("\n---------------\n["+run_time+"] skip finding control points......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip finding control points......\n\n"));

		}


	


	case 3:
		

		if(!stitch_cp_clean.FileExists())
		{
			cmd=ExeDir+wxT("//cpclean -o")+sdir+wxT("stitch_cp_clean.pto ")+sdir+wxT("//stich_cp.pto");
			if(execexternal(cmd,wxT("Cleaning Control Points"))!=0)
			{
				return;
			}
			break;
		}
		else
		{
			push_message("\n---------------\n["+run_time+"] skip cleaning control points......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip cleaning control points......\n\n"));
		}
		
	case 4:
		
		if(!stitch_cp_clean_line.FileExists())
		{

			cmd=ExeDir+"//linefind -o "+sdir+"stich_cp_clean_linefind.pto "+sdir+"stich_cp_clean.pto";
			if(execexternal(cmd,wxT("Finding vertical lines"))!=0)
			{
				return;
			}
			break;
		}
		else
		{
			push_message("\n---------------\n["+run_time+"] skip finding vertical lines......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip finding vertical lines......\n\n"));


		}
		

	case 5:

		cmd=ExeDir+"//checkpto "+sdir+"//stich_cp_clean_linefind.pto";
		if(execexternal(cmd,wxT("checking project"))!=0)
			{
				return;
			}
			break;
		break;
	default:
		::wxMessageBox(wxT("执行完成"));
		MainFrame::m_timerprocess.Stop();
	}

}
