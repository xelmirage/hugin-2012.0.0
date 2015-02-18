#include "ARLabStitcherwxControlPointFrame.h"
#include "ARLabStitcherwxMainFrame.h"

ARLabStitcherwxControlPointFrame::ARLabStitcherwxControlPointFrame( wxWindow* parent )
:
ControlPointFrame( parent )
{

}

void ARLabStitcherwxControlPointFrame::setPTO(wxString s)
{
	ptoFile = s;
}



int ARLabStitcherwxControlPointFrame::getReady()
{
	ifstream prjfile(ptoFile.char_str());
	if (!prjfile.good()) {
		wxMessageBox( "could not open script : ");
		return 1;
	}
	pano.setFilePrefix(hugin_utils::getPathPrefix(ptoFile.ToStdString()));
	DocumentData::ReadWriteError err = pano.readData(prjfile);
	if (err != DocumentData::SUCCESSFUL) {
		wxMessageBox("error while parsing panos tool script: " + ptoFile.ToStdString() +"\n"
			+ "DocumentData::ReadWriteError code: " + lexical_cast<string>(err)+ "\n");
		return 1;
	}
	if (pano.getNrOfImages()==0)
	{
		wxMessageBox ("Panorama should consist of at least one image" );
		return 1;
	}

	if (pano.getNrOfCtrlPoints() == 0)
	{
		wxMessageBox("Panorama should consist of at least one image");
		return 1;
	}
	for (int i = 0; i < pano.getNrOfImages(); ++i)
	{
		ControlPointFrame::m_choiceLeft->Insert(
			pano.getImage(i).getFilename(),m_choiceLeft->GetCount());

		ControlPointFrame::m_choiceRight->Insert(
			pano.getImage(i).getFilename(), m_choiceRight->GetCount());
	}



	return 0;
}


int ARLabStitcherwxControlPointFrame::UpdateDisplay()
{





	return 0;
}
