#include "SuperOverlay.h"

pyramidNode::pyramidNode()
{
	corners.insert(corners.end(), pointll());
	corners.insert(corners.end(), pointll());
	corners.insert(corners.end(), pointll());
	corners.insert(corners.end(), pointll());
}
pyramidNode::~pyramidNode()
{

}
bool pyramidNode::readKml(wxString kmlfile)
{
	return true;
}
bool pyramidNode::writeKml()
{
	getBox();
	
	
	wxXmlNode* root =new wxXmlNode(NULL,wxXML_ELEMENT_NODE, wxT("kml"));
	root->AddAttribute("xmlns", "http://www.opengis.net/kml/2.2");
	root->AddAttribute("xmlns:gx", "http://www.google.com/kml/ext/2.2");
	root->AddAttribute("xmlns:kml", "http://www.opengis.net/kml/2.2");
	root->AddAttribute("xmlns:atom", "http://www.w3.org/2005/Atom");

	wxXmlNode* document =new wxXmlNode(NULL,wxXML_ELEMENT_NODE, wxT("Document"),wxEmptyString);
	root->AddChild(document);

	wxXmlNode* region =new wxXmlNode( wxXML_ELEMENT_NODE, wxT("Region"));
	document->AddChild(region);

	wxXmlNode* lod =new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Lod"));
	region->AddChild(lod);


	wxXmlNode* minlodpixels =new wxXmlNode(wxXML_ELEMENT_NODE, wxT("minLodPixels"));
	wxXmlNode* minlodpixels_content =new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(minLod));
	minlodpixels->AddChild(minlodpixels_content);

	wxXmlNode* maxlodpixels =new wxXmlNode( wxXML_ELEMENT_NODE, wxT("maxLodPixels"));
	wxXmlNode* maxlodpixels_content =new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(maxLod));
	maxlodpixels->AddChild(maxlodpixels_content);

	lod->AddChild(minlodpixels);
	lod->AddChild(maxlodpixels);

	wxXmlNode* latlonaltbox = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("LatLonAltBox"), wxEmptyString);
	region->AddChild(latlonaltbox);

	wxXmlNode* altNorth =new wxXmlNode( wxXML_ELEMENT_NODE, wxT("north"));
	wxXmlNode* altNorth_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(north));
	altNorth->AddChild(altNorth_content);

	wxXmlNode* altSouth =new wxXmlNode(wxXML_ELEMENT_NODE, wxT("south"));
	wxXmlNode* altSouth_content =new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(south));
	altSouth->AddChild(altSouth_content);


	wxXmlNode* altEast =new wxXmlNode(wxXML_ELEMENT_NODE, wxT("east"));
	wxXmlNode* altEast_content =new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(east));
	altEast->AddChild(altEast_content);


	wxXmlNode* altWest =new wxXmlNode(wxXML_ELEMENT_NODE, wxT("west"));
	wxXmlNode* altWest_content =new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(west));
	altWest->AddChild(altWest_content);
	

	latlonaltbox->AddChild(altNorth);
	latlonaltbox->AddChild(altSouth);
	latlonaltbox->AddChild(altEast);
	latlonaltbox->AddChild(altWest);

	wxXmlNode* groundoverlay = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("GroundOverlay"), wxEmptyString);
	document->AddChild(groundoverlay);

	wxXmlNode* icon = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Icon"), wxEmptyString);
	groundoverlay->AddChild(icon);

	wxXmlNode* href =new wxXmlNode(wxXML_ELEMENT_NODE, wxT("href"));
	wxXmlNode* href_content =new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, image.GetFullName());
	href->AddChild(href_content);
	
	icon->AddChild(href);

	/*wxXmlNode* latlonbox = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("LatLonBox"), wxEmptyString);
	groundoverlay->AddChild(latlonbox);




	wxXmlNode* boxNorth = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("north"));
	wxXmlNode* boxNorth_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(north));
	boxNorth->AddChild(boxNorth_content);

	wxXmlNode* boxSouth = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("south"));
	wxXmlNode* boxSouth_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(south));
	boxSouth->AddChild(boxSouth_content);


	wxXmlNode* boxEast = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("east"));
	wxXmlNode* boxEast_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(east));
	boxEast->AddChild(boxEast_content);


	wxXmlNode* boxWest = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("west"));
	wxXmlNode* boxWest_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(west));
	boxWest->AddChild(boxWest_content);





	wxXmlNode* boxRotation = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("rotation"));
	wxXmlNode* boxRotation_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(rotation));
	boxRotation->AddChild(boxRotation_content);

	latlonbox->AddChild(boxNorth);
	latlonbox->AddChild(boxSouth);
	latlonbox->AddChild(boxEast);
	latlonbox->AddChild(boxWest);
	latlonbox->AddChild(boxRotation);*/

	wxString coordinateString;
	coordinateString.clear();
	for (int i = 0; i < corners.size(); ++i)
	{
		coordinateString.append(boost::lexical_cast<string>(corners[i].getLongitude()));
		coordinateString.append(",");
		coordinateString.append(boost::lexical_cast<string>(corners[i].getLatitude()));
		coordinateString.append(",0 ");
	}

	wxXmlNode* latlonquad = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("gx:LatLonQuad"), wxEmptyString);
	groundoverlay->AddChild(latlonquad);
	wxXmlNode* coordinates = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("coordinates"));
	latlonquad->AddChild(coordinates);

	wxXmlNode* coordinates_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, coordinateString);
	coordinates->AddChild(coordinates_content);



	if (networkNodes.size()>0)
	{
		for (int ni = 0; ni < networkNodes.size(); ++ni)
		{
			wxXmlNode* networklink = new wxXmlNode(NULL,wxXML_ELEMENT_NODE, wxT("NetworkLink"), wxEmptyString);
			document->AddChild(networklink);

			wxXmlNode* region = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Region"));
			networklink->AddChild(region);

			wxXmlNode* lod = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Lod"));
			region->AddChild(lod);


			wxXmlNode* minlodpixels = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("minLodPixels"));
			wxXmlNode* minlodpixels_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(networkNodes[ni].minLod));
			minlodpixels->AddChild(minlodpixels_content);

			wxXmlNode* maxlodpixels = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("maxLodPixels"));
			wxXmlNode* maxlodpixels_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(networkNodes[ni].maxLod));
			maxlodpixels->AddChild(maxlodpixels_content);

			lod->AddChild(minlodpixels);
			lod->AddChild(maxlodpixels);

			wxXmlNode* latlonaltbox = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("LatLonAltBox"), wxEmptyString);
			region->AddChild(latlonaltbox);

			wxXmlNode* altNorth = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("north"));
			wxXmlNode* altNorth_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(networkNodes[ni].north));
			altNorth->AddChild(altNorth_content);

			wxXmlNode* altSouth = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("south"));
			wxXmlNode* altSouth_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(networkNodes[ni].south));
			altSouth->AddChild(altSouth_content);


			wxXmlNode* altEast = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("east"));
			wxXmlNode* altEast_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(networkNodes[ni].east));
			altEast->AddChild(altEast_content);


			wxXmlNode* altWest = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("west"));
			wxXmlNode* altWest_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, boost::lexical_cast<string>(networkNodes[ni].west));
			altWest->AddChild(altWest_content);


			latlonaltbox->AddChild(altNorth);
			latlonaltbox->AddChild(altSouth);
			latlonaltbox->AddChild(altEast);
			latlonaltbox->AddChild(altWest);

			wxXmlNode* link = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Link"));
			networklink->AddChild(link);

			wxXmlNode* href = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("href"));
			wxXmlNode* href_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, networkNodes[ni].kml.GetFullName());
			href->AddChild(href_content);
			link->AddChild(href);

			wxXmlNode* viewRefreshMode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("viewRefreshMode"));
			wxXmlNode* viewRefreshMode_content = new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, "onRegion");
			viewRefreshMode->AddChild(viewRefreshMode_content);
			link->AddChild(viewRefreshMode);



		}
	}










	wxXmlDocument doc;
	doc.SetRoot(root);
	doc.Save(kml.GetFullPath());




	return true;
}

void pyramidNode::getBox()
{
	for (int i = 0; i < corners.size(); ++i)
	{
		if (i == 0)
		{
			north = corners[0].getLatitude();
			south = corners[0].getLatitude();
			east = corners[0].getLongitude();
			west = corners[0].getLongitude();
		}
		else
		{
			if (corners[i].getLatitude() > north) north = corners[i].getLatitude();
			if (corners[i].getLatitude() < south) south = corners[i].getLatitude();
			if (corners[i].getLongitude() > east) east = corners[i].getLongitude();
			if (corners[i].getLongitude() < west) west = corners[i].getLongitude();
		}

	}
	
	/*double centerX=0, centerY=0;
	for (int i = 0; i < corners.size(); ++i)
	{
		centerX += corners[i].getLongitude();
		centerY += corners[i].getLatitude();

	}
	centerX = centerX / 4;
	centerY = centerY / 4;

	double dy = 0.5*sqrt(
		(corners[0].getLongitude() - corners[1].getLongitude())
		*(corners[0].getLongitude() - corners[1].getLongitude())

		+ (corners[0].getLatitude() - corners[1].getLatitude()) 
		*(corners[0].getLatitude() - corners[1].getLatitude()));


	double dx = 0.5*sqrt(
		(corners[0].getLongitude() - corners[3].getLongitude()) 
		*(corners[0].getLongitude() - corners[3].getLongitude())

		+ (corners[0].getLatitude() - corners[3].getLatitude())
		*(corners[0].getLatitude() - corners[3].getLatitude()));

	north = centerY + dy;
	south = centerY - dy;
	east = centerX + dx;
	west = centerX - dx;*/
}
