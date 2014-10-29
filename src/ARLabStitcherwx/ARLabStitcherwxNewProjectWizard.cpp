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
void ARLabStitcherwxNewProjectWizard::DisableNextFirst(wxWizardEvent& WXUNUSED(event))
{
	this->m_btnNext->Enable(false);
}
void ARLabStitcherwxNewProjectWizard::ValidateInputDir(wxCommandEvent& WXUNUSED(event))
{
	sdir = m_textCtrlSourceDir->GetValue();
	
	wxDir dir(sdir);
	if (dir.IsOpened())
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