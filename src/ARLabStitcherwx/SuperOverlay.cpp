#include "SuperOverlay.h"


SuperOverlay::SuperOverlay(wxString imagePath, wxString kmlPath, wxString outPath) :
imagePath(imagePath), kmlPath(kmlPath), outPath(outPath)
{

	corners.insert(corners.end(), lowerleft);
	corners.insert(corners.end(), lowerright);
	corners.insert(corners.end(), upperright);
	corners.insert(corners.end(), upperleft);
}


SuperOverlay::~SuperOverlay()
{
}


bool SuperOverlay::build()
{
	/*int i;
	wxImage baseImage;
	baseImage.AddHandler(new wxTIFFHandler);
	baseImage.LoadFile(imagePath, wxBITMAP_TYPE_TIFF);
	if (!baseImage.IsOk())
	{
		return false;
	}
	int longSide;
	longSide = max(baseImage.GetHeight(), baseImage.GetWidth());*/

	readKml(kmlPath);


	return true;
}

bool SuperOverlay::readKml(wxString kmlFileName)
{
	wxString stringCoordinates;
	wxXmlDocument doc;
	if (!doc.Load(kmlFileName))
		return false;

	if (doc.GetRoot()->GetName() != "kml")
		return false;

	wxXmlNode *child = doc.GetRoot()->GetChildren();
	if (child == NULL||child->GetName()!="Folder")
	{
		return false;
	}
	child = child->GetChildren();
	if (child == NULL || child->GetName() != "name")
	{
		return false;
	}
	else
	{
		while (child) 
		{
			if (child->GetName() == "GroundOverlay")
			{
				wxXmlNode *ovlchild = child->GetChildren();
				if (child == NULL )
				{
					return false;
				}
				while (ovlchild)
				{
					if (ovlchild->GetName() == "gx:LatLonQuad")
					{
						if (ovlchild->GetChildren()->GetName() == "coordinates")
						{
							stringCoordinates = ovlchild->GetChildren()->GetNodeContent();
							wxArrayString arrayCoordinates;
							stringCoordinates.Trim();
							arrayCoordinates=wxSplit(stringCoordinates, ' ');
							for (int i=0; i < arrayCoordinates.size(); ++i)
							{
								if (i >= corners.size())
								{
									break;
								}
								if (arrayCoordinates[i].size() > 6)
								{
									arrayCoordinates[i].Trim();
									wxArrayString lls = wxSplit(arrayCoordinates[i], ',');
									double x, y;
									try
									{
										x = boost::lexical_cast<double>(lls[0]);
										y = boost::lexical_cast<double>(lls[1]);
										corners[i].latitude = y;
										corners[i].longitude = x;
									}
									catch (boost::bad_lexical_cast& e)
									{
										wxMessageBox(e.what());
										return false;
									}
								}
								//wxArrayString arrayll = wxSplit(arrayCoordinates[i], ',');

							}
	
						}
					}
					ovlchild = ovlchild->GetNext();
				}
			}
			child=child->GetNext();
		}
	}


	return true;
}
