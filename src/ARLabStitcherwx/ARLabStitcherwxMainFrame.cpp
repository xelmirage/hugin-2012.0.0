#include "ARLabStitcherwxMainFrame.h"
#include "ARLabStitcherwxGPSFrame.h"

ARLabStitcherwxMainFrame::ARLabStitcherwxMainFrame( wxWindow* parent ,wxString Dir)
	:
MainFrame( parent )
, isExecPanel_Running(false)
, allowToolStart(false)
, allowToolShowTrack(false)
, isExecPanel_Paused(false)
{
	time_count=0;
	m_GPSFrame = new ::ARLabStitcherwxGPSFrame(this);
	m_execPanel= new MyExecPanel(MainFrame:: m_notebookProgressOut);
	m_controlPointsFrame = new ARLabStitcherwxControlPointFrame(this);
	//m_execPanel->SetId(wxID_execPanel);
	
	//MainFrame::m_splitter5->SplitHorizontally( m_panel7, m_execPanel, 0 );
	ExeDir=Dir;
	MainFrame::m_notebookProgressOut->SetPageText(0,wxT("����״̬"));
	//MainFrame::m_timerprocess->SetOwner(this);
	MainFrame::m_notebookProgressOut->AddPage(m_execPanel,wxT("�Ӳ���״̬"));
	//::wxMessageBox(m_execPanel->GetParent()->GetName());
	MainFrame::m_notebook4->SetPageText(0,wxT("�����ļ�����"));
	MainFrame::m_notebook4->SetPageText(1, wxT("������Ϣ��ʾ"));
	
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
	spin=0;

	phasename[0]="����ʶ��";
	phasename[1]="���ɹ���";
	phasename[2]="ͼ��ƥ��";
	phasename[3]="�޳�����";
	phasename[4]="ͼ��У׼";
	phasename[5]="���̼��";
	phasename[6]="ͼ����";

	phasename[7]="����ü�";
	phasename[8]="GPS��λ�ز���";
	phasename[9]="ͼ���ں�";
	phasename[10] = "����";
	sdir="f:\\kl\\part";
	gpsfileName="f:\\kl\\part\\02.txt";
	


	m_toolStart->Enable(false);
	m_toolShowTrack->Enable(false);
	m_toolShowKML->Enable(false);

	isBatch = false;
	m_panel14->Bind(wxEVT_SIZE, &ARLabStitcherwxMainFrame::panelPreviewSizeChanged, this);
	preview_isReady = false;


	m_menuEdit->Enable(wxID_menuItemProcess,false);
	m_menuEdit->Enable(wxID_menuItemPreProcess, false);
	m_menuEdit->Enable(wxID_menuItemFindCP, false);
	m_menuEdit->Enable(wxID_menuItemOptimise, FALSE);



	wxToolBarBase *tb = GetToolBar();

	tb->FindById(wxID_ToolStart)->Enable(false);


	tb->EnableTool(wxID_ToolStart, false);
	tb->EnableTool(wxID_toolShowTrack, false);
	m_toolBarMain->EnableTool(wxID_menuItemMerge, false);
	m_toolBarMain->EnableTool(wxID_menuItemAutoCrop, false);
	m_toolBarMain->EnableTool(wxID_menuItemOptimise, false);
	m_toolBarMain->EnableTool(wxID_menuItemFindCP, false);
	


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
void ARLabStitcherwxMainFrame::newProjectTool(wxCommandEvent& WXUNUSED(event))
{
	::ARLabStitcherwxNewProjectWizard nw=new ::ARLabStitcherwxNewProjectWizard(this);
	
	
	if (nw.RunWizard(nw.m_pages[0])==TRUE)
	{
		sdir = nw.sdir;
		gpsfileName = nw.gpsfileName;
		outfileName = nw.outfileName;


		wxPathList list;
		::wxDir::GetAllFiles(sdir, &list, wxT("*.jpg"), wxDIR_FILES);

		MainFrame::m_listBoxPicList->Clear();
		MainFrame::m_listBoxPicList->InsertItems(list, 0);
		wxArrayString exif;
		wxExecute("exiftool " + list[0], exif);
		m_listBoxExif->InsertItems(exif, 0);

		beltlog.SetPath(sdir + "\\belts.log");
		stitch.SetPath(sdir + "\\stitch.mosaicinfo");
		stitch_cp.SetPath(sdir + "\\stitch_cp.mosaicinfo");
		stitch_cp_clean.SetPath(sdir + "\\stitch_cp_clean.mosaicinfo");
		stitch_cp_clean_line.SetPath(sdir + "\\stitch_cp_clean_linefind.mosaicinfo");
		stitch_cp_clean_line_op.SetPath(sdir + "\\stitch_cp_clean_linefind_op.mosaicinfo");
		gps_connect.SetPath(this->outfileName + ".coord");
		stitch_cp_clean_line_op_crop.SetPath(sdir + "\\stitch_cp_clean_linefind_op_crop.mosaicinfo");
		
		workset.push_back(beltlog);
		workset.push_back(stitch);
		workset.push_back(stitch_cp);
		workset.push_back(stitch_cp_clean);
		workset.push_back(stitch_cp_clean_line);
		workset.push_back(stitch_cp_clean_line_op);
		workset.push_back(gps_connect);
		workset.push_back(stitch_cp_clean_line_op_crop);
		
		
		
		
		
		
		ofile.SetPath(this->outfileName);




		m_toolShowTrack->Enable(true);
		m_toolStart->Enable(true);


		m_menuEdit->Enable(wxID_menuItemPreProcess, TRUE);
		m_menuEdit->Enable(wxID_menuItemProcess, TRUE);

		m_toolBarMain->EnableTool(wxID_ToolStart, true);
		return;
	}

	




	wxToolBarBase *tb = GetToolBar();


	tb->EnableTool(wxID_ToolStart, false);
	tb->EnableTool(wxID_toolShowTrack, false);

}
void ARLabStitcherwxMainFrame::newProcess_discard(wxCommandEvent& WXUNUSED(event))
{
	wxString cmd;
	//wxMessageBox("new");
	wxStringList fileNameList;
	wxStringList fileList;



	//::wxDirDialog
	wxDirDialog dd(this,_("ѡ��ͼ��Ŀ¼"),"", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	
	//ͼ��Ŀ¼
	if(dd.ShowModal()== wxID_CANCEL)
		return;

	sdir=dd.GetPath();
	//wxMessageBox(sdir);

	//----------------GPS---------------------//
	::wxFileDialog fdGPS(this,_("ѡ��GPS�ļ�"),"","","txt files (*.txt)|*.txt",wxFD_OPEN |wxFD_FILE_MUST_EXIST );
	if(fdGPS.ShowModal()== wxID_CANCEL)
		return;
	gpsfileName=fdGPS.GetPath();


	//---------------OUT------//

	::wxFileDialog outFile(this,_("ѡ������ļ�"),"","","tif files (*.tif)|*.tif",::wxFD_SAVE);

	if(outFile.ShowModal()==wxID_CANCEL)
		return;
	this->outfileName=outFile.GetPath();
	

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
void ARLabStitcherwxMainFrame::ListBoxClicked(wxCommandEvent& e)
{

	currentPreviewPic = e.GetString();
	preview_isReady = true;
	UpdateImagePreview();



	wxArrayString exif;
	wxExecute("exiftool " + e.GetString(), exif);
	m_listBoxExif->InsertItems(exif, 0);

	
}
void ARLabStitcherwxMainFrame::processcmd(wxCommandEvent& WXUNUSED(event))
{
	//MainFrame::m_timerprocess.Start(1000);
	::Sleep(1200);
	phase=-1;

	process();
	
	//MainFrame::m_timerprocess.Stop();

}
void ARLabStitcherwxMainFrame::count_time(::wxTimerEvent& e)
{
	
	//time_now=t.GetTicks();
	//time_count=static_cast<long>(time_now-time_start);
	
	wxDateTime now = wxDateTime::Now();
	wxTimeSpan runtime = now - t;
	
	
	run_time.clear();
	
	run_time+=wxString::Format(wxT("%d"),runtime.GetDays())+wxT(" �� ");
	run_time+=wxString::Format(wxT("%d"),runtime.GetHours()%24)+wxT(" Сʱ ");
	run_time+=wxString::Format(wxT("%d"),runtime.GetMinutes()%60)+wxT(" �� ");
	run_time += (runtime.GetSeconds()%60).ToString() + wxT(" �� ");

	this->change_status();

	
}
void ARLabStitcherwxMainFrame::change_status()
{
	string status;
	status.clear();
	
	switch(spin)
	{
	case 0:
		status+="- ";
		spin=1;
		break;
	case 1:
		status+="\\ ";
		spin=2;
		break;
	case 2:
		status+="| ";
		spin=3;
		break;
	case 3:
		status+="/ ";
		spin=0;
		break;

	}
	

	if(phase>10) phase=10;
	status+=phasename[phase]+"   "+boost::lexical_cast<string>(phase)+"/10    "+run_time;
	m_statusBar->SetStatusText(status);
}
void ARLabStitcherwxMainFrame::push_message(wxString message)
{
	
	//m_textCtrlProgress->SetValue(
	//	m_textCtrlProgress->GetValue()+message);
	m_textCtrlProgress->AppendText(message);
}
int ARLabStitcherwxMainFrame::execexternal(wxString command,wxString message)
{
	if (isExecPanel_Running)
	{
		wxMessageBox("�����У������ظ�����");
		return 0;
	}
	isExecPanel_Running = true;
	push_message("\n---------------\n["+run_time+"] ��ʼ "+message+"\n---------------\n");
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
	wxString cmd;
	EnableFunction(phase);
	push_message("\n---------------\n["+run_time+"] "+phasename[phase]+" ���\n---------------\n");
	isExecPanel_Running = false;
	if (isBatch)
	{
		process();
	}
	else
	{
		
		wxString res = sdir + wxT("\\belts.log");
		switch (phase)
		{
		case 0://����ʶ��
			
			

			phase = 1;

			cmd = ExeDir + wxT("\\pto_gen ") + sdir + wxT("\\*.jpg -o") + sdir + wxT("\\stitch.mosaicinfo --gps -f 1");
			if (execexternal(cmd, wxT("���ɹ���")) != 0)
			{
				return;
			}
			
			break;
		case 1://generate project
			m_GPSFrame->setGPSFileName(gpsfileName);
			m_GPSFrame->setResultName(res);



			m_GPSFrame->getReady();
			m_GPSFrame->Show(true);
			m_menuEdit->Enable(wxID_menuItemFindCP, TRUE);
			MainFrame::m_timerprocess.Stop();
			
			break;
			
		case 2://cpclean finish,the clean cp
			phase = 3;
			cmd = ExeDir + wxT("\\cpclean -o") + sdir + wxT("\\stitch_cp_clean.mosaicinfo ") + sdir + wxT("\\stitch_cp.mosaicinfo");
			if (execexternal(cmd, wxT("��������")) != 0)
			{
				return;
			}
			break;
		case 3:
			m_controlPointsFrame->setPTO(sdir + wxT("\\stitch_cp.mosaicinfo "));
			if (!m_controlPointsFrame->isReady())
			{
				if (m_controlPointsFrame->getReady() != 0)
				{
					wxMessageBox("cpframe not ready!");
					return;
				}
			}
			m_controlPointsFrame->Show();
			MainFrame::m_timerprocess.Stop();
			EnableFunction(phase_cpfind);
			break;
		case 4:
			phase = 6;
			cmd = ExeDir + "\\autooptimiser -a -s -l -o " + sdir + "\\stitch_cp_clean_linefind_op.mosaicinfo " + sdir + "\\stitch_cp_clean_linefind.mosaicinfo";
			if (execexternal(cmd, wxT("ͼ����")) != 0)
			{
				return;
			}
			break;
		case 6:
			wxMessageBox("������ɣ�");
			EnableFunction(phase_optimise);
			break;
		case 7:
			phase = 8;
			cmd = ExeDir + "\\nona -f " + sdir + "\\stitch_cp_clean_linefind_op_crop.mosaicinfo -o " + this->outfileName;
			if (execexternal(cmd, wxT("GPS�ز���")) != 0)
			{
				return;
			}
			break;
		case 8:
			wxMessageBox("crop finish!");
			break;
		case 9:
			wxMessageBox("");
			break;
		default:
			break;
		}
	}
	
	
}

void ARLabStitcherwxMainFrame::process(void)
{
	if (!allowToolStart)
	{
		wxMessageBox("����δ���������½�����");
		return;
	}
	isBatch = true;
	std::string cmd;
	
	++phase;
	switch (phase)
	{
	case 0://GPSFilting
		::Sleep(1200);
		MainFrame::m_gauge3->SetValue(10);
		MainFrame::m_textCtrlProgress->Clear();
		this->m_execPanel->ClearText();
		time_count=0;
		t = wxDateTime::Now();
		
		this->m_timerprocess.Start(1000);
		if(!beltlog.FileExists())
		{
			cmd=ExeDir+wxT("\\gpsfilter -o ")+sdir+wxT("\\belts.log -g ")+gpsfileName+wxT(" -s ")+sdir;

			if (execexternal(cmd,wxT("����ʶ��"))!=0)
				return;
			//"gpsfilter -o "+sdir+"belts.log -g "+gpsfileName+" -s "+sdir,"processing GPSFilting");
			break;
		}
		else
		{
			++phase;
			
			push_message(wxT("\n---------------\n[")+run_time+wxT("] ��������ʶ��......\n---------------\n"));
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip filting......\n\n"));
			//QMessageBox::information(NULL, "Existing results","skip GPSFilting", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		}
		
		
	case 1:

		MainFrame::m_gauge3->SetValue(20);


		if(!stitch.FileExists())
		{
			cmd=ExeDir+wxT("\\pto_gen ")+sdir+wxT("\\*.jpg -o")+sdir+wxT("\\stitch.mosaicinfo --gps -f 1");
			if(execexternal(cmd,wxT("���ɹ���"))!=0)
			{
				return;
			}
			break;

		}
		else
		{
			++phase;
			
			push_message("\n---------------\n["+run_time+"] �������ɹ���......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip generating pto......\n\n"));

		}
		
	case 2://CPFind
		MainFrame::m_gauge3->SetValue(30);

		

		if(!stitch_cp.FileExists())
		{
			cmd=ExeDir+wxT("\\cpfindgps001 -o ")+sdir+wxT("\\stitch_cp.mosaicinfo ")+sdir+wxT("\\stitch.mosaicinfo --gps");
			if(execexternal(cmd,wxT("ƥ��ͼ��"))!=0)
			{
				return;
			}
				break;
		}
		else
		{
			++phase;
			
			push_message("\n---------------\n["+run_time+"] ����ͼ��ƥ��......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip finding control points......\n\n"));

		}


	


	case 3:
		MainFrame::m_gauge3->SetValue(40);


		if(!stitch_cp_clean.FileExists())
		{
			cmd=ExeDir+wxT("\\cpclean -o")+sdir+wxT("\\stitch_cp_clean.mosaicinfo ")+sdir+wxT("\\stitch_cp.mosaicinfo");
			if(execexternal(cmd,wxT("��������"))!=0)
			{
				return;
			}
			break;
		}
		else
		{
			++phase;
			
			push_message("\n---------------\n["+run_time+"] ������������......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip cleaning control points......\n\n"));
		}
		
	case 4:
		MainFrame::m_gauge3->SetValue(50);

		if(!stitch_cp_clean_line.FileExists())
		{

			cmd=ExeDir+"\\linefind -o "+sdir+"\\stitch_cp_clean_linefind.mosaicinfo "+sdir+"\\stitch_cp_clean.mosaicinfo";
			if(execexternal(cmd,wxT("ͼ��У׼"))!=0)
			{
				return;
			}
			break;
		}
		else
		{
			++phase;
			
			push_message("\n---------------\n["+run_time+"] ����ͼ��У׼......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip finding vertical lines......\n\n"));


		}


	case 5:
		MainFrame::m_gauge3->SetValue(60);

		cmd=ExeDir+"\\checkpto "+sdir+"\\stitch_cp_clean_linefind.mosaicinfo";
		if(execexternal(cmd,wxT("���̼��"))!=0)
		{
			return;
		}
		
	case 6:
		MainFrame::m_gauge3->SetValue(70);

		if(!stitch_cp_clean_line_op.FileExists())
		{
			cmd=ExeDir+"\\autooptimiser -a -s -l -o "+sdir+"\\stitch_cp_clean_linefind_op.mosaicinfo "+sdir+"\\stitch_cp_clean_linefind.mosaicinfo";
			if(execexternal(cmd,wxT("ͼ����"))!=0)
			{
				return;
			}
			break;
		}
		else
		{
			++phase;
			push_message("\n---------------\n["+run_time+"] �������̼��......\n---------------\n");
		}
	
	case 7:
		MainFrame::m_gauge3->SetValue(80);

		if (!stitch_cp_clean_line_op_crop.FileExists())
		{
			cmd = ExeDir + "\\pano_modify --canvas=30% --crop=auto " + sdir + "\\stitch_cp_clean_linefind_op.mosaicinfo -o " + sdir + "\\stitch_cp_clean_linefind_op_crop.mosaicinfo";
			if (execexternal(cmd, wxT("�ü�")) != 0)
			{
				return;
			}
			break;
		}
		else
		{
			++phase;
			push_message("\n---------------\n[" + run_time + "] �����ü�......\n---------------\n");
		}
		this->change_status();
	
	case 8:
		MainFrame::m_gauge3->SetValue(90);

		if (!gps_connect.FileExists())
		{
			cmd = ExeDir + "\\nona -f " + sdir + "\\stitch_cp_clean_linefind_op_crop.mosaicinfo -o " + this->outfileName;
			if (execexternal(cmd, wxT("GPS�ز���")) != 0)
			{
				return;
			}

			break;
		}
		else
		{
			++phase;
			push_message("\n---------------\n[" + run_time + "] GPS�ز���......\n---------------\n");
		}
		

	case 9:
		MainFrame::m_gauge3->SetValue(95);

		while((!stitch_cp_clean_line_op_crop.FileExists())||(!gps_connect.FileExists()))
		{
		}
		cmd=ExeDir+"\\split_blend "+sdir+"\\stitch_cp_clean_linefind_op_crop.mosaicinfo -o "+this->outfileName;
		if(execexternal(cmd,wxT("�ں�"))!=0)
		{
			return;
		}
		this->change_status();
		break;


	default:
		MainFrame::m_gauge3->SetValue(100);

		while(!ofile.FileExists())
		{

		}
		//::wxMessageBox(wxT("ִ�����"));
		this->change_status();
		push_message(wxT("\n---------------\n["+run_time+"] �������;-)\n---------------\n"));
		m_timerprocess.Stop();
		MainFrame::m_timerprocess.Stop();
		m_toolShowKML->Enable(true);
		isBatch = false;
	}
	
}
void ARLabStitcherwxMainFrame::showKML(wxCommandEvent& WXUNUSED(event))
{
	wxString kmlPath;
	wxFileName fname(outfileName);
	fname.SetExt("kml");
	wxArrayString list;
	wxExecute(fname.GetFullPath(),list);
}
void ARLabStitcherwxMainFrame::showTrack(wxCommandEvent& WXUNUSED(event))
{
	wxString strAppPath;
	wxStandardPathsBase& stdp = wxStandardPaths::Get();
	wxFileName exeFile(stdp.GetExecutablePath());
	strAppPath = exeFile.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
	strAppPath += "python\\python.exe track.py -i "+this->gpsfileName;
	wxArrayString output;
	wxExecute(strAppPath, output);
	

}


void ARLabStitcherwxMainFrame::generateSuperOverlay(wxCommandEvent& WXUNUSED(event))
{
	SuperOverlay ovl =SuperOverlay("f:/ge/20141111debug.tif", "f:/ge/20141111debug.kml", "f:/ge/superoverlay");
	ovl.build();
	
}
void ARLabStitcherwxMainFrame::preProcess(wxCommandEvent& WXUNUSED(event))
{
	isBatch = false;
	int answer;

	


	if (m_GPSFrame->isReady())
	{
		answer=wxMessageBox(wxT("�Ѿ�����ƥ���������¼��㣿"), wxT("����ƥ���"), wxYES_NO,this);
		if (answer == wxNO)
		{
			m_GPSFrame->Show();
			phase = phase_preprocess;
			wxProcessEvent ee;
			ee.SetEventObject(m_execPanel);
			end_process(ee);
			return;
		}
	}
	
	if (beltlog.FileExists())
	{
		answer = wxMessageBox(wxT("�ҵ��Ѵ��ڵ��ļ���ֱ��ʹ�ã�"), wxT("����ƥ���"), wxYES_NO, this);
		if (answer == wxYES)
		{
			m_GPSFrame->setGPSFileName(beltlog.GetFullPath());
			phase = phase_pto_gen;
			wxProcessEvent ee;
			ee.SetEventObject(m_execPanel);
			end_process(ee);

			return;
		}

	}



	wxString cmd;
	
	
	cmd = ExeDir + wxT("\\gpsfilter -o ") + sdir + wxT("\\belts.log -g ") + gpsfileName + wxT(" -s ") + sdir;
	phase = 0;

	MainFrame::m_textCtrlProgress->Clear();
	this->m_execPanel->ClearText();
	time_count = 0;
	t = wxDateTime::Now();
	this->m_timerprocess.Start(1000);
	if (execexternal(cmd, wxT("����ʶ��")) != 0)
		return;

	/*wxString res = sdir + wxT("\\belts.log");
	this->m_timerprocess.Start(1000);
	m_GPSFrame->setGPSFileName(gpsfileName);
	m_GPSFrame->setResultName(res);*/

	
	/*if (execexternal(cmd, wxT("����ʶ��")) != 0)
		return;*/
	
}
void ARLabStitcherwxMainFrame::menuProcess(wxCommandEvent& WXUNUSED(event))
{
	
	



}

void ARLabStitcherwxMainFrame::findCP(wxCommandEvent& WXUNUSED(event))
{
	wxString cmd;
	isBatch = false;
	int answer;
	if (stitch_cp.FileExists())
	{
		answer = wxMessageBox(wxT("�ҵ��Ѵ��ڵĽ���ļ���ֱ��ʹ�ã�"), wxT("����ƥ���"), wxYES_NO, this);
		if (answer == wxYES)
		{
			phase = phase_cpfind;
			//end_process();
			return;
		}
	}
	cmd = ExeDir + wxT("\\cpfindgps001 -o ") + sdir + wxT("\\stitch_cp.mosaicinfo ") + sdir + wxT("\\stitch.mosaicinfo --gps");
	phase = 2;

	MainFrame::m_textCtrlProgress->Clear();
	this->m_execPanel->ClearText();
	time_count = 0;
	t = wxDateTime::Now();


	
	this->m_timerprocess.Start(1000);
	if (execexternal(cmd, wxT("����ƥ���")) != 0)
		return;


	
}

void ARLabStitcherwxMainFrame::UpdateImagePreview()
{
	if (!preview_isReady)
	{
		return;
	}
	wxImage preimg;
	float ratioh, ratiow, ratio;

	wxSize pSize = MainFrame::m_panel14->GetSize();
	pSize -= pSize - wxSize(50, 50);
	//preimg.Clear();
	preimg.LoadFile(currentPreviewPic, wxBITMAP_TYPE_JPEG);
	ratioh = preimg.GetHeight() / pSize.GetHeight();
	ratiow = preimg.GetWidth() / pSize.GetWidth();

	if (ratiow > ratioh)
	{
		ratio = ratiow;

	}
	else
	{
		ratio = ratioh;

	}

	MainFrame::m_bitmappreview->SetSize(preimg.GetWidth() / ratio, preimg.GetHeight() / ratio);
	preimg = preimg.Scale(preimg.GetWidth() / ratio, preimg.GetHeight() / ratio);
	MainFrame::m_bitmappreview->ClearBackground();
	MainFrame::m_bitmappreview->ResetConstraints();
	MainFrame::m_bitmappreview->SetBitmap(preimg);



	
}


void ARLabStitcherwxMainFrame::panelPreviewSizeChanged(wxSizeEvent& e)
{
	
	UpdateImagePreview();
}


void ARLabStitcherwxMainFrame::menuOptimise(wxCommandEvent& ee)
{
	phase = 4;
		wxString cmd = ExeDir + "\\linefind -o " + sdir + "\\stitch_cp_clean_linefind.mosaicinfo " + sdir + "\\stitch_cp_clean.mosaicinfo";
	if (execexternal(cmd, wxT("ͼ��У׼")) != 0)
	{
		return;
	}


}


void ARLabStitcherwxMainFrame::allDisableForWork()
{
	m_menuEdit->Enable(wxID_menuItemProcess, false);
	m_menuEdit->Enable(wxID_menuItemPreProcess, false);
	m_menuEdit->Enable(wxID_menuItemFindCP, false);
	m_menuEdit->Enable(wxID_menuItemOptimise, FALSE);


	m_toolStart->Enable(FALSE);
	m_toolShowTrack->Enable(FALSE);
	m_toolSuperOverLay->Enable(FALSE);
}


void ARLabStitcherwxMainFrame::allEnableForWork()
{
	m_menuEdit->Enable(wxID_menuItemProcess, false);
	m_menuEdit->Enable(wxID_menuItemPreProcess, false);
	m_menuEdit->Enable(wxID_menuItemFindCP, false);
	m_menuEdit->Enable(wxID_menuItemOptimise, FALSE);


	m_toolStart->Enable(FALSE);
	m_toolShowTrack->Enable(FALSE);
	m_toolSuperOverLay->Enable(FALSE);
}


void ARLabStitcherwxMainFrame::menuCrop(wxCommandEvent& ee)
{
	isBatch = false;
	phase = 7;
	wxString cmd = ExeDir + "\\pano_modify --canvas=30% --crop=auto " + sdir + "\\stitch_cp_clean_linefind_op.mosaicinfo -o " + sdir + "\\stitch_cp_clean_linefind_op_crop.mosaicinfo";
	if (execexternal(cmd, wxT("�ü�")) != 0)
	{
		return;
	}
	


}


void ARLabStitcherwxMainFrame::blend(wxCommandEvent& ee)
{
	wxString cmd = ExeDir + "\\split_blend " + sdir + "\\stitch_cp_clean_linefind_op_crop.mosaicinfo -o " + this->outfileName;
	if (execexternal(cmd, wxT("�ں�")) != 0)
	{
		return;
	}
}


void ARLabStitcherwxMainFrame::EnableFunction(int phase)
{
	m_toolBarMain->EnableTool(wxID_menuItemMerge, false);
	m_toolBarMain->EnableTool(wxID_menuItemAutoCrop, false);
	m_toolBarMain->EnableTool(wxID_menuItemOptimise, false);
	m_toolBarMain->EnableTool(wxID_menuItemFindCP, false);
	switch (phase)
	{
	case phase_merge:
	case phase_nona_gps:
	case phase_crop://crop finishi
		m_toolBarMain->EnableTool(wxID_menuItemMerge, TRUE);
	case phase_optimise:
		m_toolBarMain->EnableTool(wxID_menuItemAutoCrop, TRUE);
	case phase_checkpto:
	case phase_linefind:
	case phase_cpclean:
	case phase_cpfind:
		m_toolBarMain->EnableTool(wxID_menuItemOptimise,TRUE);
	case phase_pto_gen:
	case phase_preprocess:
		m_toolBarMain->EnableTool(wxID_menuItemFindCP, TRUE);
	default:
		break;
	}

	switch (phase)
	{
		
	case phase_preprocess:
		if (stitch.Exists())
		{
			wxRemoveFile(stitch.GetFullPath());
		}
	case phase_pto_gen:
		if (stitch.Exists())
		{
			wxRemoveFile(stitch_cp.GetFullPath());
		}
	case phase_cpfind:
		if (stitch.Exists())
		{
			wxRemoveFile(stitch_cp_clean.GetFullPath());
		}
	case phase_cpclean:
		if (stitch.Exists())
		{
			wxRemoveFile(stitch_cp_clean_line.GetFullPath());
		}
	case phase_linefind:
		if (stitch.Exists())
		{
			wxRemoveFile(stitch_cp_clean_line_op.GetFullPath());
		}
	case phase_checkpto:
		
	case phase_optimise:
		if (stitch.Exists())
		{
			wxRemoveFile(stitch_cp_clean_line_op_crop.GetFullPath());
		}
	case phase_crop://crop finishi
		if (gps_connect.Exists())
		{
			wxRemoveFile(gps_connect.GetFullPath());
		}
	case phase_nona_gps:
		
	case phase_merge:
		
	default:
		break;
	}
}


void ARLabStitcherwxMainFrame::pauseProcess(wxCommandEvent& ee)
{
	if (isExecPanel_Running)
	{
		if (m_toolPause->IsToggled())
		{
			m_execPanel->ContinueProcess();
		}
		else
		{
			m_execPanel->PauseProcess();
		}
		
	}
	return;
	
}


void ARLabStitcherwxMainFrame::stopProcess(wxCommandEvent& ee)
{
	if (isExecPanel_Running)
	{
		m_execPanel->KillProcess();
		isExecPanel_Running = false;
	}
	return;
}
