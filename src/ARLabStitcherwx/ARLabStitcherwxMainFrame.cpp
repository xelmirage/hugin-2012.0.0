#include "ARLabStitcherwxMainFrame.h"
#include "ARLabStitcherwxGPSFrame.h"

ARLabStitcherwxMainFrame::ARLabStitcherwxMainFrame( wxWindow* parent ,wxString Dir)
	:
MainFrame( parent )
{
	time_count=0;
	m_GPSFrame = new ::ARLabStitcherwxGPSFrame(this);
	m_execPanel= new MyExecPanel(MainFrame:: m_notebookProgressOut);
	m_controlPointsFrame = new ARLabStitcherwxControlPointFrame(this);
	//m_execPanel->SetId(wxID_execPanel);
	
	//MainFrame::m_splitter5->SplitHorizontally( m_panel7, m_execPanel, 0 );
	ExeDir=Dir;
	MainFrame::m_notebookProgressOut->SetPageText(0,wxT("处理状态"));
	//MainFrame::m_timerprocess->SetOwner(this);
	MainFrame::m_notebookProgressOut->AddPage(m_execPanel,wxT("子步骤状态"));
	//::wxMessageBox(m_execPanel->GetParent()->GetName());
	MainFrame::m_notebook4->SetPageText(0,wxT("工程文件管理"));
	MainFrame::m_notebook4->SetPageText(1, wxT("工程信息显示"));
	
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

	phasename[0]="航迹识别";
	phasename[1]="生成工程";
	phasename[2]="图像匹配";
	phasename[3]="剔除误差点";
	phasename[4]="图像校准";
	phasename[5]="工程检查";
	phasename[6]="图像定向";

	phasename[7]="结果裁剪";
	phasename[8]="GPS点位重采样";
	phasename[9]="图像融合";
	phasename[10] = "结束";
	sdir="f:\\kl\\part";
	gpsfileName="f:\\kl\\part\\02.txt";
	


	m_toolStart->Enable(false);
	m_toolShowTrack->Enable(false);
	m_toolShowKML->Enable(false);

	isBatch = false;
	
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
	nw.RunWizard(nw.m_pages[0]);
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


	m_toolShowTrack->Enable(true);
	m_toolStart->Enable(true);

}
void ARLabStitcherwxMainFrame::newProcess(wxCommandEvent& WXUNUSED(event))
{
	wxString cmd;
	//wxMessageBox("new");
	wxStringList fileNameList;
	wxStringList fileList;



	//::wxDirDialog
	wxDirDialog dd(this,_("选择图像目录"),"", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	
	//图像目录
	if(dd.ShowModal()== wxID_CANCEL)
		return;

	sdir=dd.GetPath();
	//wxMessageBox(sdir);

	//----------------GPS---------------------//
	::wxFileDialog fdGPS(this,_("选择GPS文件"),"","","txt files (*.txt)|*.txt",wxFD_OPEN |wxFD_FILE_MUST_EXIST );
	if(fdGPS.ShowModal()== wxID_CANCEL)
		return;
	gpsfileName=fdGPS.GetPath();


	//---------------OUT------//

	::wxFileDialog outFile(this,_("选择输出文件"),"","","tif files (*.tif)|*.tif",::wxFD_SAVE);

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
	
	run_time+=wxString::Format(wxT("%d"),runtime.GetDays())+wxT(" 天 ");
	run_time+=wxString::Format(wxT("%d"),runtime.GetHours()%24)+wxT(" 小时 ");
	run_time+=wxString::Format(wxT("%d"),runtime.GetMinutes()%60)+wxT(" 分 ");
	run_time += (runtime.GetSeconds()%60).ToString() + wxT(" 秒 ");

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

	push_message("\n---------------\n["+run_time+"] 开始 "+message+"\n---------------\n");
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
	
	push_message("\n---------------\n["+run_time+"] "+phasename[phase]+" 完成\n---------------\n");
	if (isBatch)
	{
		process();
	}
	else
	{
		m_timerprocess.Stop();
		MainFrame::m_timerprocess.Stop();
		wxString res = sdir + wxT("\\belts.log");
		switch (phase)
		{
		case 0://航迹识别
			
			m_GPSFrame->setGPSFileName(gpsfileName);
			m_GPSFrame->setResultName(res);
			
			m_GPSFrame->getReady();
			m_GPSFrame->Show(true);
			break;
		default:
			break;
		}
	}
	
	
}

void ARLabStitcherwxMainFrame::process(void)
{
	isBatch = true;
	std::string cmd;
	wxFileName beltlog(sdir+"\\belts.log");
	wxFileName stitch(sdir+"\\stitch.pto");
	wxFileName stitch_cp(sdir+"\\stitch_cp.pto");
	wxFileName stitch_cp_clean(sdir+"\\stitch_cp_clean.pto");
	wxFileName stitch_cp_clean_line(sdir+"\\stitch_cp_clean_linefind.pto");
	wxFileName stitch_cp_clean_line_op(sdir+"\\stitch_cp_clean_linefind_op.pto");
	wxFileName gps_connect(this->outfileName+".coord");
	wxFileName stitch_cp_clean_line_op_crop(sdir+"\\stitch_cp_clean_linefind_op_crop.pto");
	wxFileName ofile(this->outfileName);
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

			if (execexternal(cmd,wxT("航迹识别"))!=0)
				return;
			//"gpsfilter -o "+sdir+"belts.log -g "+gpsfileName+" -s "+sdir,"processing GPSFilting");
			break;
		}
		else
		{
			++phase;
			
			push_message(wxT("\n---------------\n[")+run_time+wxT("] 跳过航迹识别......\n---------------\n"));
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip filting......\n\n"));
			//QMessageBox::information(NULL, "Existing results","skip GPSFilting", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		}
		
		
	case 1://CPFind

		MainFrame::m_gauge3->SetValue(20);


		if(!stitch.FileExists())
		{
			cmd=ExeDir+wxT("\\pto_gen ")+sdir+wxT("\\*.jpg -o")+sdir+wxT("\\stitch.pto --gps -f 1");
			if(execexternal(cmd,wxT("生成工程"))!=0)
			{
				return;
			}
			break;

		}
		else
		{
			++phase;
			
			push_message("\n---------------\n["+run_time+"] 跳过生成工程......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip generating pto......\n\n"));

		}
		
	case 2:
		MainFrame::m_gauge3->SetValue(30);

		

		if(!stitch_cp.FileExists())
		{
			cmd=ExeDir+wxT("\\cpfindgps001 -o ")+sdir+wxT("\\stitch_cp.pto ")+sdir+wxT("\\stitch.pto --gps");
			if(execexternal(cmd,wxT("匹配图像"))!=0)
			{
				return;
			}
				break;
		}
		else
		{
			++phase;
			
			push_message("\n---------------\n["+run_time+"] 跳过图像匹配......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip finding control points......\n\n"));

		}


	


	case 3:
		MainFrame::m_gauge3->SetValue(40);


		if(!stitch_cp_clean.FileExists())
		{
			cmd=ExeDir+wxT("\\cpclean -o")+sdir+wxT("\\stitch_cp_clean.pto ")+sdir+wxT("\\stitch_cp.pto");
			if(execexternal(cmd,wxT("清理误差点"))!=0)
			{
				return;
			}
			break;
		}
		else
		{
			++phase;
			
			push_message("\n---------------\n["+run_time+"] 跳过清理误差点......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip cleaning control points......\n\n"));
		}
		
	case 4:
		MainFrame::m_gauge3->SetValue(50);

		if(!stitch_cp_clean_line.FileExists())
		{

			cmd=ExeDir+"\\linefind -o "+sdir+"\\stitch_cp_clean_linefind.pto "+sdir+"\\stitch_cp_clean.pto";
			if(execexternal(cmd,wxT("图像校准"))!=0)
			{
				return;
			}
			break;
		}
		else
		{
			++phase;
			
			push_message("\n---------------\n["+run_time+"] 跳过图像校准......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip finding vertical lines......\n\n"));


		}


	case 5:
		MainFrame::m_gauge3->SetValue(60);

		cmd=ExeDir+"\\checkpto "+sdir+"\\stitch_cp_clean_linefind.pto";
		if(execexternal(cmd,wxT("工程检查"))!=0)
		{
			return;
		}
		
	case 6:
		MainFrame::m_gauge3->SetValue(70);

		if(!stitch_cp_clean_line_op.FileExists())
		{
			cmd=ExeDir+"\\autooptimiser -a -s -l -o "+sdir+"\\stitch_cp_clean_linefind_op.pto "+sdir+"\\stitch_cp_clean_linefind.pto";
			if(execexternal(cmd,wxT("图像定向"))!=0)
			{
				return;
			}
			break;
		}
		else
		{
			++phase;
			push_message("\n---------------\n["+run_time+"] 跳过工程检查......\n---------------\n");
		}
	
	case 7:
		MainFrame::m_gauge3->SetValue(80);

		if (!stitch_cp_clean_line_op_crop.FileExists())
		{
			cmd = ExeDir + "\\pano_modify --canvas=30% --crop=auto " + sdir + "\\stitch_cp_clean_linefind_op.pto -o " + sdir + "\\stitch_cp_clean_linefind_op_crop.pto";
			if (execexternal(cmd, wxT("裁剪")) != 0)
			{
				return;
			}
			break;
		}
		else
		{
			++phase;
			push_message("\n---------------\n[" + run_time + "] 跳过裁剪......\n---------------\n");
		}
		this->change_status();
	
	case 8:
		MainFrame::m_gauge3->SetValue(90);

		if (!gps_connect.FileExists())
		{
			cmd = ExeDir + "\\nona -f " + sdir + "\\stitch_cp_clean_linefind_op_crop.pto -o " + this->outfileName;
			if (execexternal(cmd, wxT("GPS重采样")) != 0)
			{
				return;
			}

			break;
		}
		else
		{
			++phase;
			push_message("\n---------------\n[" + run_time + "] GPS重采样......\n---------------\n");
		}
		

	case 9:
		MainFrame::m_gauge3->SetValue(95);

		while((!stitch_cp_clean_line_op_crop.FileExists())||(!gps_connect.FileExists()))
		{
		}
		cmd=ExeDir+"\\split_blend "+sdir+"\\stitch_cp_clean_linefind_op_crop.pto -o "+this->outfileName;
		if(execexternal(cmd,wxT("融合"))!=0)
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
		//::wxMessageBox(wxT("执行完成"));
		this->change_status();
		push_message(wxT("\n---------------\n["+run_time+"] 处理完成;-)\n---------------\n"));
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
int ARLabStitcherwxMainFrame::stitch(wxString inputFileName, wxString outFileName)
{

	return 0;
}

void ARLabStitcherwxMainFrame::generateSuperOverlay(wxCommandEvent& WXUNUSED(event))
{
	SuperOverlay ovl =SuperOverlay("f:/ge/20141111debug.tif", "f:/ge/20141111debug.kml", "f:/ge/superoverlay");
	ovl.build();
	
}
void ARLabStitcherwxMainFrame::preProcess(wxCommandEvent& WXUNUSED(event))
{
	wxString cmd;
	
	
	cmd = ExeDir + wxT("\\gpsfilter -o ") + sdir + wxT("\\belts.log -g ") + gpsfileName + wxT(" -s ") + sdir;
	phase = 0;

	MainFrame::m_textCtrlProgress->Clear();
	this->m_execPanel->ClearText();
	time_count = 0;
	t = wxDateTime::Now();


	wxString res = sdir + wxT("\\belts.log");
	this->m_timerprocess.Start(1000);
	m_GPSFrame->setGPSFileName(gpsfileName);
	m_GPSFrame->setResultName(res);

	m_GPSFrame->getReady();
	m_GPSFrame->Show(true);

	/*if (execexternal(cmd, wxT("航迹识别")) != 0)
		return;*/
	
}
void ARLabStitcherwxMainFrame::menuProcess(wxCommandEvent& WXUNUSED(event))
{
	
	



}

void ARLabStitcherwxMainFrame::findCP(wxCommandEvent& WXUNUSED(event))
{
	m_controlPointsFrame->setPTO("C:\part\stitch_cp.pto");
	if (m_controlPointsFrame->getReady() != 0)
	{
		wxMessageBox("cpframe not ready!");
	};
	m_controlPointsFrame->Show();
}