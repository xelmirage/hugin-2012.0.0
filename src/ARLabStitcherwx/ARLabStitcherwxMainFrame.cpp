#include "ARLabStitcherwxMainFrame.h"


#include <config.h>
#include <hugin_version.h>

#include "base_wx/huginConfig.h"
#include<hugin/GLPreviewFrame.h>

using namespace PT;
using namespace std;
using namespace hugin_utils;
ARLabStitcherwxMainFrame::ARLabStitcherwxMainFrame( wxWindow* parent ,wxString Dir)
	:
MainFrame( parent )
{
	time_count=0;
	
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
	spin=0;

	phasename[0]="航迹识别";
	phasename[1]="生成工程";
	phasename[2]="图像匹配";
	phasename[3]="剔除误差点";
	phasename[4]="图像校准";
	phasename[5]="工程检查";
	phasename[6]="图像定向";

	phasename[7]="GPS点位重采样";
	phasename[8]="结果裁剪";
	phasename[9]="图像融合";
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
}
void ARLabStitcherwxMainFrame::processcmd(wxCommandEvent& WXUNUSED(event))
{
	MainFrame::m_timerprocess.Start(1000);
	::Sleep(1200);
	phase=-1;

	process();
	
	//MainFrame::m_timerprocess.Stop();

}
void ARLabStitcherwxMainFrame::count_time(::wxTimerEvent& e)
{
	
	//time_now=t.GetTicks();
	//time_count=static_cast<long>(time_now-time_start);
	++time_count;
	int day,hour,minute,second;
	day=time_count/24/3600;
	hour=(time_count/3600)%24;
	minute=(time_count/60)%60;
	second=time_count%60;

	run_time.clear();
	
	run_time+=wxString::Format(wxT("%d"),day)+wxT(" 天 ");
	run_time+=wxString::Format(wxT("%d"),hour)+wxT(" 小时 ");
	run_time+=wxString::Format(wxT("%d"),minute)+wxT(" 分 ");
	run_time+=wxString::Format(wxT("%d"),second)+wxT(" 秒 ");

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
	process();
	//wxMessageBox("\n---------------\n["+run_time+"] GPSFILTER finished\n---------------\n");
}

void ARLabStitcherwxMainFrame::process(void)
{
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

	PT::Panorama  p;
	PT::Panorama & pano(p);

	GLPreviewFrame* gl_preview_frame = new GLPreviewFrame(this, pano);

	switch (phase)
	{
	case 0://GPSFilting
		::Sleep(1200);
		MainFrame::m_textCtrlProgress->Clear();
		this->m_execPanel->ClearText();
		time_count=0;
		time_start=t.GetTicks();
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
			
			push_message("\n---------------\n["+run_time+"] skip generating project......\n---------------\n");
			//textEdit->setText(textEdit->toPlainText()+tr("\n skip generating pto......\n\n"));

		}


	case 2:

		

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
		
		cmd=ExeDir+"\\checkpto "+sdir+"\\stitch_cp_clean_linefind.pto";
		if(execexternal(cmd,wxT("工程检查"))!=0)
		{
			return;
		}
		
	case 6:
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
		if(!gps_connect.FileExists())
		{
			cmd=ExeDir+"\\nona -f "+sdir+"\\stitch_cp_clean_linefind_op.pto -o "+this->outfileName;
			if(execexternal(cmd,wxT("GPS重采样"))!=0)
			{
				return;
			}

			break;
		}
		else
		{
			++phase;
			push_message("\n---------------\n["+run_time+"] GPS重采样......\n---------------\n");
		}
	
	case 8:
	
		if(!stitch_cp_clean_line_op_crop.FileExists())
		{
			cmd=ExeDir+"\\pano_modify --crop=auto "+sdir+"\\stitch_cp_clean_linefind_op.pto -o "+sdir+"\\stitch_cp_clean_linefind_op_crop.pto";
			if(execexternal(cmd,wxT("裁剪"))!=0)
			{
				return;
			}
			break;
		}
		else
		{
			++phase;
			push_message("\n---------------\n["+run_time+"] 跳过裁剪......\n---------------\n");
		}
		this->change_status();


	case 9:
		



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
		while(!ofile.FileExists())
		{

		}
		//::wxMessageBox(wxT("执行完成"));
		this->change_status();
		push_message(wxT("\n---------------\n["+run_time+"] 处理完成;-)\n---------------\n"));
		
		MainFrame::m_timerprocess.Stop();
	}

}
