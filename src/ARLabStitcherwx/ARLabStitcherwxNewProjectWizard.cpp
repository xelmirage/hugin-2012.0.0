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
	
{
	this->m_btnNext->Enable(false);
}
void ARLabStitcherwxNewProjectWizard::OpenSourceDir(wxCommandEvent& WXUNUSED(event))
{
	wxDirDialog dd(this, _("选择图像目录"), "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dd.ShowModal() == wxID_CANCEL)
		return;

	std::vector<wxFileName> workset;
	m_textCtrlSourceDir->SetValue(dd.GetPath());
	std::string sdir = dd.GetPath();
	wxFileName beltlog(sdir + "\\belts.log");
	workset.push_back(beltlog);

	wxFileName stitch(sdir + "\\stitch.pto");
	workset.push_back(stitch);
	wxFileName stitch_cp(sdir + "\\stitch_cp.pto");
	workset.push_back(stitch_cp);
	wxFileName stitch_cp_clean(sdir + "\\stitch_cp_clean.pto");
	workset.push_back(stitch_cp_clean);
	wxFileName stitch_cp_clean_line(sdir + "\\stitch_cp_clean_linefind.pto");
	workset.push_back(stitch_cp_clean_line);
	wxFileName stitch_cp_clean_line_op(sdir + "\\stitch_cp_clean_linefind_op.pto");
	workset.push_back(stitch_cp_clean_line_op);
	wxFileName gps_connect(this->outfileName + ".coord");
	workset.push_back(gps_connect);
	wxFileName stitch_cp_clean_line_op_crop(sdir + "\\stitch_cp_clean_linefind_op_crop.pto");
	workset.push_back(stitch_cp_clean_line_op_crop);

	for (int i = 0; i < workset.size(); ++i)
	{
		if (workset[i].Exists())
		{
			continue;
		}
		else if (i > 0)
		{
			int answer = wxMessageBox("belt record found, recover from it?", "Recovery", wxYES_NO | wxCANCEL);
			if (answer == wxYES)
			{
				for (int j = i; j < workset.size(); ++j)
				{
					if (workset[j].Exists())
					{
						wxRemoveFile(workset[j].GetFullPath());
					}
				}
				return;
			}

			else//用户选择了从头开始，于是清理所有残余
			{
				s_FileFindMutex.Lock();
				wxString f = wxFindFirstFile(sdir + "\\*.pto");
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
			wxString f = wxFindFirstFile(sdir + wxT("*.pto"));
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