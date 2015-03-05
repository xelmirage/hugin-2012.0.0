#include "ARLabStitcherwxControlPointFrame.h"
#include "ARLabStitcherwxMainFrame.h"

ARLabStitcherwxControlPointFrame::ARLabStitcherwxControlPointFrame( wxWindow* parent )
:
ControlPointFrame( parent )
, previewLeftIsReady(false)
, isLeftImgReady(false)
, isRightImgReady(false)
, CPToDraw(0)
, imgLeftNr(0)
, imgRightNr(0)
, scaledCP(0)
, isReady(FALSE)
, bitmapSide(0)
{
	bitmapSide = 15;

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

	isReady = TRUE;
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
	
	imgRightNrs.clear();
	for (int i = 0; i < cpForLeftImg.size(); ++i)
	{

		ControlPoint tempCP = cpForLeftImg[i].second;

		imgRightNrs.push_back(tempCP.image2Nr);
	}
	sort(imgRightNrs.begin(), imgRightNrs.end());
	std::vector<unsigned int>::iterator pos;
	pos =std::unique(imgRightNrs.begin(), imgRightNrs.end());
	imgRightNrs.erase(pos, imgRightNrs.end());
	m_choiceRight->Clear();
	for (int i = 0; i < imgRightNrs.size(); ++i)
	{
		
		m_choiceRight->Insert(
			pano.getImage(imgRightNrs[i]).getFilename(), m_choiceRight->GetCount()
			);
	}
	m_choiceRight->Enable(true);
	isLeftImgReady = true;
	imgLeftPath = m_choiceLeft->GetString(imageLeftNr);
	imgLeft.LoadFile(imgLeftPath, wxBITMAP_TYPE_JPEG);
	double yaw = const_map_get(pano.getImageVariables(imgLeftNr), "y").getValue();
	double pitch = const_map_get(pano.getImageVariables(imgLeftNr), "p").getValue();
	double roll = const_map_get(pano.getImageVariables(imgLeftNr), "r").getValue();
	ImageRotation rot = GetRot(yaw,pitch,roll);
	if (rot!=ARLabStitcherwxControlPointFrame::ROT0)
	{
		switch (rot)
		{
		
		case ARLabStitcherwxControlPointFrame::ROT90:
			imgLeft.Rotate90(TRUE);
			break;
		case ARLabStitcherwxControlPointFrame::ROT180:
			imgLeft.Rotate180();
			break;
		case ARLabStitcherwxControlPointFrame::ROT270:
			imgLeft.Rotate90(FALSE);
			break;
		default:
			break;
		}
	}


	isRightImgReady = false;
	m_bitmapRight->ClearBackground();
	UpdatePreview();


}


void ARLabStitcherwxControlPointFrame::choiceRightChanged(wxCommandEvent & ee)
{
	unsigned int currentNr;
	isRightImgReady = true;
	imgRightPath = m_choiceRight->GetString(m_choiceRight->GetSelection());
	imgRight.LoadFile(imgRightPath, wxBITMAP_TYPE_JPEG);
	CPToDraw.clear();
	scaledCP.clear();
	for (int i = 0; i < cpForLeftImg.size();++i)
	{
		currentNr = cpForLeftImg[i].second.image2Nr;

		if (currentNr == imgRightNrs[m_choiceRight->GetSelection()])
		{
			
			CPToDraw.push_back(cpForLeftImg[i]);
			scaledCP.push_back(cpForLeftImg[i]);
		}
	}
	double yaw = const_map_get(pano.getImageVariables(imgRightNr), "y").getValue();
	double pitch = const_map_get(pano.getImageVariables(imgRightNr), "p").getValue();
	double roll = const_map_get(pano.getImageVariables(imgRightNr), "r").getValue();
	ImageRotation rot = GetRot(yaw, pitch, roll);
	if (rot != ARLabStitcherwxControlPointFrame::ROT0)
	{
		switch (rot)
		{

		case ARLabStitcherwxControlPointFrame::ROT90:
			imgRight.Rotate90(TRUE);
			break;
		case ARLabStitcherwxControlPointFrame::ROT180:
			imgRight.Rotate180();
			break;
		case ARLabStitcherwxControlPointFrame::ROT270:
			imgRight.Rotate90(FALSE);
			break;
		default:
			break;
		}
	}




	UpdatePreview();
	

}


void ARLabStitcherwxControlPointFrame::OnResize(wxSizeEvent& e)
{
	//e.Skip(true);
	wxSize newSize = e.GetSize();
	newSize.SetWidth(newSize.GetWidth() / 2);
	m_panelLeft->SetSize(newSize);

	wxPoint panelRightPosition = m_panelLeft->GetPosition();
	panelRightPosition.x += newSize.GetWidth();

	m_panelRight->SetPosition(panelRightPosition);
	m_panelRight->SetSize(newSize);
	wxString title;
	wxSize fsize = this->GetSize();
	title.Format("frame w %d, h %d,    panel w %d, h %d", fsize.GetWidth(), fsize.GetHeight(), newSize.GetWidth(), newSize.GetHeight());
	this->SetTitle(title);


	//this->Refresh();
	//UpdatePreview();


}


void ARLabStitcherwxControlPointFrame::setImage(wxStaticBitmap* m_staticBitmap, wxString imagePath, wxImage preimg)
{

	wxImage pImage;
	float ratioh, ratiow, ratio;


	//preimg.Clear();

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
	pImage = preimg.Scale(preimg.GetWidth() / ratio, preimg.GetHeight() / ratio);
	m_staticBitmap->ClearBackground();
	m_staticBitmap->ResetConstraints();
	m_staticBitmap->SetBitmap(pImage);


	
}


void ARLabStitcherwxControlPointFrame::UpdatePreview()
{
	if (isLeftImgReady)
	{
		setImage(m_bitmapLeft, imgLeftPath, imgLeft);
	
	}

	if (isRightImgReady)
	{
		setImage(m_bitmapRight, imgRightPath, imgRight);
	}
	wxClientDC dcLeft(m_bitmapLeft);
	wxClientDC dcRight(m_bitmapRight);
	paintCP(&dcLeft, &dcRight);


	/*wxSize pSize = m_panelLeft->GetSize();
	this->SetTitle(wxString::Format("%d,%d", pSize.GetWidth(), pSize.GetHeight())+m_bitmapLeft->GetParent()->GetName());*/
	
}


void ARLabStitcherwxControlPointFrame::OnPaint(wxPaintEvent& ee)
{
	
	ControlPointFrame::OnPaint(ee);
	wxPaintDC dcLeft(m_bitmapLeft);
	wxPaintDC dcRight(m_bitmapRight);
	paintCP(&dcLeft, &dcRight);
}


void ARLabStitcherwxControlPointFrame::paintCP(wxDC* dcLeft, wxDC* dcRight)
{
	dcLeft->SetPen(*wxGREEN_PEN);
	wxPoint p(0, 0);
	wxCoord c(5);
	//dcLeft->DrawCircle(p, c);
	if (isLeftImgReady&&isRightImgReady)
	{
		double factorLeft = (double)m_bitmapLeft->GetSize().GetHeight() / (double)imgLeft.GetHeight();
		double factorRight = (double)m_bitmapRight->GetSize().GetHeight() / (double)imgRight.GetHeight();
		
		for (int i=0; i < scaledCP.size();++i)
		{
			p.x = scaledCP[i].second.x1 = CPToDraw[i].second.x1*factorLeft;
			p.y=scaledCP[i].second.y1 = CPToDraw[i].second.y1*factorLeft;
			dcLeft->DrawCircle(p, c);
			
			p.x = scaledCP[i].second.x2 = CPToDraw[i].second.x2*factorRight;
			p.y = scaledCP[i].second.y2 = CPToDraw[i].second.y2*factorRight;
			dcRight->DrawCircle(p, c);
			//dc->DrawCircle(p, c);
			
		}
		



	}





}


void ARLabStitcherwxControlPointFrame::OnMoveEnd(wxMoveEvent& ee)
{
	UpdatePreview();

}


ARLabStitcherwxControlPointFrame::ImageRotation ARLabStitcherwxControlPointFrame::GetRot(double yaw, double pitch, double roll)
{
	ARLabStitcherwxControlPointFrame::ImageRotation rot = ARLabStitcherwxControlPointFrame::ROT0;
	// normalize roll angle
	while (roll > 360) roll -= 360;
	while (roll < 0) roll += 360;

	while (pitch > 180) pitch -= 360;
	while (pitch < -180) pitch += 360;
	bool headOver = (pitch > 90 || pitch < -90);

	if (FALSE) {
		if (roll >= 315 || roll < 45) {
			rot = headOver ? ARLabStitcherwxControlPointFrame::ROT180 : ARLabStitcherwxControlPointFrame::ROT0;
		}
		else if (roll >= 45 && roll < 135) {
			rot = headOver ? ARLabStitcherwxControlPointFrame::ROT270 : ARLabStitcherwxControlPointFrame::ROT90;
		}
		else if (roll >= 135 && roll < 225) {
			rot = headOver ? ARLabStitcherwxControlPointFrame::ROT0 : ARLabStitcherwxControlPointFrame::ROT180;
		}
		else {
			rot = headOver ? ARLabStitcherwxControlPointFrame::ROT90 : ARLabStitcherwxControlPointFrame::ROT270;
		}
	}
	return rot;

}
