#include "ARLabStitcherwxControlPointFrame.h"
#include "ARLabStitcherwxMainFrame.h"

ARLabStitcherwxControlPointFrame::ARLabStitcherwxControlPointFrame( wxWindow* parent )
:
ControlPointFrame( parent )
, previewLeftIsReady(false)
, isLeftImgReady(false)
, isRightImgReady(false)
{
	//this->Bind(wxEVT_SIZE, &ARLabStitcherwxControlPointFrame::UpdatePreviewEvent, this);
	
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

		//ControlPointFrame::m_choiceRight->Insert(
		//	pano.getImage(i).getFilename(), m_choiceRight->GetCount());
	}

	ControlPointFrame::m_choiceRight->Enable(false);


	return 0;
}


int ARLabStitcherwxControlPointFrame::UpdateDisplay()
{





	return 0;
}


void ARLabStitcherwxControlPointFrame::choiceLeftChanged(wxCommandEvent& ee)
{
	using namespace HuginBase;
	unsigned int imageLeftNr=2;
	imageLeftNr = m_choiceLeft->GetSelection();
	cpForLeftImg.clear();
	cpForLeftImg = pano.getCtrlPointsVectorForImage(imageLeftNr);
	
	
	for (int i = 0; i < cpForLeftImg.size(); ++i)
	{

		ControlPoint tempCP = cpForLeftImg[i].second;

		rightImgNrs.push_back(tempCP.image2Nr);
	}
	sort(rightImgNrs.begin(), rightImgNrs.end());
	std::vector<unsigned int>::iterator pos;
	pos =std::unique(rightImgNrs.begin(), rightImgNrs.end());
	rightImgNrs.erase(pos, rightImgNrs.end());

	for (int i = 0; i < rightImgNrs.size(); ++i)
	{
		m_choiceRight->Insert(
			pano.getImage(rightImgNrs[i]).getFilename(), m_choiceRight->GetCount()
			);
	}
	m_choiceRight->Enable(true);
	isLeftImgReady = true;
	leftImgPath = m_choiceLeft->GetString(imageLeftNr);
	UpdatePreview();

}


void ARLabStitcherwxControlPointFrame::choiceRightChanged(wxCommandEvent & ee)
{
	isRightImgReady = true;
	rightImgPath = m_choiceRight->GetString(m_choiceRight->GetSelection());

}


void ARLabStitcherwxControlPointFrame::OnResize(wxSizeEvent& e)
{
	e.Skip(true);
	this->Refresh();
	UpdatePreview();
}


void ARLabStitcherwxControlPointFrame::setImage(wxStaticBitmap* m_staticBitmap, wxString imagePath)
{

	wxImage preimg;
	float ratioh, ratiow, ratio;


	//preimg.Clear();
	preimg.LoadFile(imagePath, wxBITMAP_TYPE_JPEG);
	ratioh = preimg.GetHeight() / m_staticBitmap->GetParent()->GetSize().GetHeight();
	ratiow = preimg.GetWidth() / m_staticBitmap->GetParent()->GetSize().GetWidth();

	if (ratiow > ratioh)
	{
		ratio = ratiow;

	}
	else
	{
		ratio = ratioh;

	}

	m_staticBitmap->SetSize(preimg.GetWidth() / ratio, preimg.GetHeight() / ratio);
	preimg = preimg.Scale(preimg.GetWidth() / ratio, preimg.GetHeight() / ratio);
	m_staticBitmap->ClearBackground();
	m_staticBitmap->ResetConstraints();
	m_staticBitmap->SetBitmap(preimg);
}


void ARLabStitcherwxControlPointFrame::UpdatePreview()
{
	if (isLeftImgReady)
	{
		setImage(m_bitmapLeft, leftImgPath);
	}

	if (isRightImgReady)
	{
		setImage(m_bitmapRight, rightImgPath);
	}





}
