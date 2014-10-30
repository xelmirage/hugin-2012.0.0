#include "ARLabStitcherwxNewProjectWizard.h"

inline void setControlEnable(int id, bool state)
{
	wxWindow *win = wxWindow::FindWindowById(id);
	if (win) win->Enable(state);
}
ARLabStitcherwxNewProjectWizard::ARLabStitcherwxNewProjectWizard( wxWindow* parent )
:
NewProjectWizard( parent )
{
	this->m_btnNext->Enable(false);
}
void ARLabStitcherwxNewProjectWizard::OpenSourceDir(wxCommandEvent& WXUNUSED(event))
{
	wxDirDialog dd(this, _("选择图像目录"), "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dd.ShowModal() == wxID_CANCEL)
		return;

	
		m_textCtrlSourceDir->SetValue( dd.GetPath());
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

	



	if (list.size()>0)
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
	gpath = m_textCtrlGPSFile->GetValue();
	
	wxFile gfile(gpath);
	if (gfile.Exists(gpath))
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