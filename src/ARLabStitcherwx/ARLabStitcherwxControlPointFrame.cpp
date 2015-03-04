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

		imgRightNrs.push_back(tempCP.image2Nr);
	}
	sort(imgRightNrs.begin(), imgRightNrs.end());
	std::vector<unsigned int>::iterator pos;
	pos =std::unique(imgRightNrs.begin(), imgRightNrs.end());
	imgRightNrs.erase(pos, imgRightNrs.end());

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
	UpdatePreview();


}


void ARLabStitcherwxControlPointFrame::choiceRightChanged(wxCommandEvent & ee)
{
	unsigned int currentNr;
	isRightImgReady = true;
	imgRightPath = m_choiceRight->GetString(m_choiceRight->GetSelection());
	imgRight.LoadFile(imgRightPath, wxBITMAP_TYPE_JPEG);

	for (int i = 0; i < cpForLeftImg.size();++i)
	{
		currentNr = cpForLeftImg[i].second.image2Nr;

		if (currentNr == imgRightNrs[m_choiceRight->GetSelection()])
		{
			CPToDraw.push_back(cpForLeftImg[i]);
			scaledCP.push_back(cpForLeftImg[i]);
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
	UpdatePreview();


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
	wxClientDC dc(m_bitmapLeft);
	paintCP(&dc);


	wxSize pSize = m_panelLeft->GetSize();
	this->SetTitle(wxString::Format("%d,%d", pSize.GetWidth(), pSize.GetHeight())+m_bitmapLeft->GetParent()->GetName());
	
}


void ARLabStitcherwxControlPointFrame::OnPaint(wxPaintEvent& ee)
{
	ControlPointFrame::OnPaint(ee);
	wxPaintDC dc(m_bitmapLeft);
	paintCP(&dc);
}


void ARLabStitcherwxControlPointFrame::paintCP(wxDC* dc)
{
	
	dc->SetPen(*wxGREEN_PEN);
	wxPoint p(0, 0);
	wxCoord c(50);
	//dc->DrawCircle(p, c);
	if (isLeftImgReady&&isRightImgReady)
	{
		double factorLeft = m_bitmapLeft->GetSize().GetHeight() / imgLeft.GetHeight();
		double factorRight = m_bitmapRight->GetSize().GetHeight() / imgRight.GetHeight();
		
		for (int i=0; i < scaledCP.size();++i)
		{
			p.x = scaledCP[i].second.x1 = CPToDraw[i].second.x1*factorLeft;
			p.y=scaledCP[i].second.y1 = CPToDraw[i].second.y1*factorLeft;
			dc->DrawCircle(p, c);



			p.x = scaledCP[i].second.x2 = CPToDraw[i].second.x2*factorRight;
			p.y = scaledCP[i].second.y2 = CPToDraw[i].second.y2*factorRight;
			//dc->DrawCircle(p, c);
			



		}






	}





}
