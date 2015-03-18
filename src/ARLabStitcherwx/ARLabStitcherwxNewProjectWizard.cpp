#include "ARLabStitcherwxNewProjectWizard.h"
wxMutex ARLabStitcherwxNewProjectWizard::s_FileFindMutex;
inline void setControlEnable(int id, bool state)
{
	wxWindow *win = wxWindow::FindWindowById(id);
	if (win) win->Enable(state);
}
ARLabStitcherwxNewProjectWizard::ARLabStitcherwxNewProjectWizard(wxWindow* parent)
	:
	NewProjectWizard(parent)
	
	, recover(false)
	, isMS(false)
{
	this->m_btnNext->Enable(false);
}
void ARLabStitcherwxNewProjectWizard::OpenSourceDir(wxCommandEvent& WXUNUSED(event))
{
	wxDirDialog dd(this, _("选择图像目录"), "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dd.ShowModal() == wxID_CANCEL)
		return;
	int ms = wxMessageBox("是否是多光谱数据？", "多光谱", wxYES_NO);
	if (ms==wxYES)
	{
		isMS	= true;
	}
	else
	{
		isMS = false;
	}
	std::vector<wxFileName> workset;
	m_textCtrlSourceDir->SetValue(dd.GetPath());
	std::string sdir = dd.GetPath();
	wxFileName beltlog(sdir + "\\belts.log");
	

	wxFileName stitch(sdir + "\\stitch.mosaicinfo");
	
	wxFileName stitch_cp(sdir + "\\stitch_cp.mosaicinfo");
	
	wxFileName stitch_cp_clean(sdir + "\\stitch_cp_clean.mosaicinfo");
	
	wxFileName stitch_cp_clean_line(sdir + "\\stitch_cp_clean_linefind.mosaicinfo");
	
	wxFileName stitch_cp_clean_line_op(sdir + "\\stitch_cp_clean_linefind_op.mosaicinfo");

	wxFileName gps_connect(this->outfileName + ".coord");
	
	wxFileName stitch_cp_clean_line_op_crop(sdir + "\\stitch_cp_clean_linefind_op_crop.mosaicinfo");


	workset.push_back(beltlog);
	workset.push_back(stitch);
	workset.push_back(stitch_cp);
	workset.push_back(stitch_cp_clean);
	workset.push_back(stitch_cp_clean_line);
	workset.push_back(stitch_cp_clean_line_op);
	workset.push_back(gps_connect);
	workset.push_back(stitch_cp_clean_line_op_crop);

	for (int i = 0; i < workset.size(); ++i)
	{
		if (workset[i].Exists())
		{
			continue;
		}
		else if (i > 0)
		{
			int answer = wxMessageBox("belt record found, recover from it?", "Recovery", wxYES_NO );
			if (answer == wxYES)
			{
				for (int j = i; j < workset.size(); ++j)
				{
					if (workset[j].Exists())
					{
						wxRemoveFile(workset[j].GetFullPath());
					}
				}
				recover = true;
				return;
			}

			else//用户选择了从头开始，于是清理所有残余
			{
				s_FileFindMutex.Lock();
				wxString f = wxFindFirstFile(sdir + "\\*.mosaicinfo");
				while (!f.empty())
				{
					wxRemoveFile(f);
					f = wxFindNextFile();
				}
				f = wxFindFirstFile(sdir +"\\*.log");
				while (!f.empty())
				{
					wxRemoveFile(f);
					f = wxFindNextFile();
				}
				f = wxFindFirstFile(sdir +"\\*.coord");
				while (!f.empty())
				{
					wxRemoveFile(f);
					f = wxFindNextFile();
				}

				s_FileFindMutex.Unlock();




				return;
			}
	
		}
		else//第一个记录缺失，强制从头开始
		{
			s_FileFindMutex.Lock();
			wxString f = wxFindFirstFile(sdir + wxT("*.mosaicinfo"));
			while (!f.empty())
			{
				wxRemoveFile(f);
				f = wxFindNextFile();
			}
			f = wxFindFirstFile(sdir + wxT("*.log"));
			while (!f.empty())
			{
				wxRemoveFile(f);
				f = wxFindNextFile();
			}
			f = wxFindFirstFile(sdir + wxT("*.coord"));
			while (!f.empty())
			{
				wxRemoveFile(f);
				f = wxFindNextFile();
			}

			s_FileFindMutex.Unlock();
			
			return;
		}
	}


}
void ARLabStitcherwxNewProjectWizard::OpenGPSFile(wxCommandEvent& WXUNUSED(event))
{
	//----------------GPS---------------------//
	::wxFileDialog fdGPS(this, _("选择GPS文件"), "", "", "txt files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (fdGPS.ShowModal() == wxID_CANCEL)
		return;
	this->m_textCtrlGPSFile->SetValue(fdGPS.GetPath());

}

void ARLabStitcherwxNewProjectWizard::OpenOutputFile(wxCommandEvent& WXUNUSED(event))
{
	::wxFileDialog outFile(this, _("选择输出文件"), "", "", "tif files (*.tif)|*.tif", ::wxFD_SAVE);

	if (outFile.ShowModal() == wxID_CANCEL)
		return;
	this->m_textCtrlOutputFile->SetValue(outFile.GetPath());
}


void ARLabStitcherwxNewProjectWizard::DisableNextFirst(wxWizardEvent& WXUNUSED(event))
{
	this->m_btnNext->Enable(false);
}
void ARLabStitcherwxNewProjectWizard::ValidateInputDir(wxCommandEvent& WXUNUSED(event))
{
	sdir = m_textCtrlSourceDir->GetValue();

	wxDir dir(sdir);

	wxPathList list;
	::wxDir::GetAllFiles(sdir, &list, wxT("*.jpg"), wxDIR_FILES);





	if (list.size() > 0)
	{
		this->m_btnNext->Enable(true);
		m_staticTextInputDirHint->SetForegroundColour(wxColor("BLACK"));
		m_staticTextInputDirHint->SetLabel("目录检查通过！");
	}
	else
	{
		m_staticTextInputDirHint->SetForegroundColour(wxColor("RED"));
		m_staticTextInputDirHint->SetLabel("目录名称存在错误！");
		this->m_btnNext->Enable(false);
	}
}
void ARLabStitcherwxNewProjectWizard::ValidateGPSFile(wxCommandEvent& WXUNUSED(event))
{
	gpsfileName = m_textCtrlGPSFile->GetValue();

	wxFile gfile(gpsfileName);
	if (gfile.Exists(gpsfileName))
	{
		this->m_btnNext->Enable(true);
		m_staticTextGPSHint->SetForegroundColour(wxColor("BLACK"));
		m_staticTextGPSHint->SetLabel("GPS文件检查通过！");
	}
	else
	{
		m_staticTextGPSHint->SetForegroundColour(wxColor("RED"));
		m_staticTextGPSHint->SetLabel("GPS文件名称存在错误！");
		this->m_btnNext->Enable(false);
	}
}
void ARLabStitcherwxNewProjectWizard::ValidateOutFile(wxCommandEvent& WXUNUSED(event))
{
	outfileName = m_textCtrlOutputFile->GetValue();

	wxFileName gfile(outfileName);

	if (gfile.IsOk())
	{
		this->m_btnNext->Enable(true);
		m_staticTextOutputFileHint->SetForegroundColour(wxColor("BLACK"));
		m_staticTextOutputFileHint->SetLabel("输出文件路径检查通过！");

	}
	else
	{
		m_staticTextOutputFileHint->SetForegroundColour(wxColor("RED"));
		m_staticTextOutputFileHint->SetLabel("输出文件名称存在错误！");
		this->m_btnNext->Enable(false);
	}
}

bool ARLabStitcherwxNewProjectWizard::isRecover()
{
	return false;
}
