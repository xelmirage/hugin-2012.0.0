#include "SuperOverlay.h"


SuperOverlay::SuperOverlay(wxString imagePath, wxString kmlPath, wxString outPath) :
imagePath(imagePath), kmlPath(kmlPath), outPath(outPath)
{

	corners.insert(corners.end(), lowerleft);
	corners.insert(corners.end(), lowerright);
	corners.insert(corners.end(), upperright);
	corners.insert(corners.end(), upperleft);

	llFactor = 1000000000000000;
	lod = 256;
	blkSize = wxSize(256, 256);
}


SuperOverlay::~SuperOverlay()
{
}


bool SuperOverlay::build()
{
	int i,j;
	wxImage baseImage;
	baseImage.AddHandler(new wxTIFFHandler);
	baseImage.LoadFile(imagePath.GetFullPath(), wxBITMAP_TYPE_TIFF);
	if (!baseImage.IsOk())
	{
		return false;
	}
	int longSide;
	longSide = max(baseImage.GetHeight(), baseImage.GetWidth());

	readKml(kmlPath.GetFullPath());

	for (i = 0; i < corners.size(); ++i)
	{
		if (i == 0)
		{
			north = corners[0];
			south = corners[0];
			east = corners[0];
			west = corners[0];
		}
		else
		{
			if (corners[i].latitude > north.latitude) north = corners[i];
			if (corners[i].latitude < south.latitude) south = corners[i];
			if (corners[i].longitude > east.longitude) east = corners[i];
			if (corners[i].longitude < west.longitude) west = corners[i];
		}

	}
	theta = atan((north.longitude - west.longitude) / (north.latitude - west.latitude));
	
	wxDir outdir(outPath);

	wxSize baseImgSize;
	baseImgSize.x = baseImage.GetWidth();
	baseImgSize.y = baseImage.GetHeight();
	
	wxSize blocks = calc_split(baseImgSize, blkSize);
	wxImage block;
	wxFileName outBlockName = wxFileName::DirName(outdir.GetName());
	wxString blockName;

	block.AddHandler(new wxTIFFHandler);

	for (i = 0; i < blocks.GetWidth(); ++i)
	{
		for (j =0; j < blocks.GetHeight(); ++j)
		{
			wxPoint p(-i*blkSize.GetX(), -j*blkSize.GetY());
			wxSize actrualSize = blkSize;
			if ((actrualSize.GetWidth() + p.x) > baseImgSize.GetWidth())
			{
				actrualSize.SetWidth(baseImgSize.GetWidth() - p.x);
			}
			if ((actrualSize.GetHeight() + p.y) > baseImgSize.GetHeight())
			{
				actrualSize.SetHeight(baseImgSize.GetHeight() - p.y);
			}
						
			
			block=baseImage.Size(actrualSize, p,50,50,50);


			blockName.Clear();
			blockName << imagePath.GetName() << "_Pyramid_0_block_" << j<< "_" << i ;
			outBlockName.SetName(blockName);
			outBlockName.SetExt("tif");
			block.SaveFile(outBlockName.GetFullPath());


		

		}
	}


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
									int64_t x, y;
									try
									{
										x = boost::lexical_cast<double>(lls[0])*LLFACTOR;
										y = boost::lexical_cast<double>(lls[1])*LLFACTOR;
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


wxSize SuperOverlay::calc_split(wxSize imageSize, wxSize blockSize)
{
	wxSize blocks;

	blocks.SetWidth(ceil(imageSize.GetWidth() / blockSize.GetWidth()));
	blocks.SetHeight(ceil(imageSize.GetHeight() / blockSize.GetHeight()));


	return blocks;
}


bool SuperOverlay::writeKML(int pyNo, int i, int j, wxString blkName)
{
	wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("kml"));
	root->AddAttribute("xmlns", "http://earth.google.com/kml/2.2");

	return true;
}
