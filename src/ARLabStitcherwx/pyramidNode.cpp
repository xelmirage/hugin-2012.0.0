#include "SuperOverlay.h"


bool pyramidNode::readKml(wxString kmlfile)
{
	
}
bool pyramidNode::writeKml()
{
	wxXmlDocument doc;
	wxXmlNode root = wxXmlNode(wxXML_ELEMENT_NODE, wxT("kml"));
	root.AddAttribute("xmlns", "http://earth.google.com/kml/2.2");

	wxXmlNode document = wxXmlNode(wxXML_ELEMENT_NODE, wxT("Document"));
	root.AddChild(&document);

	wxXmlNode region = wxXmlNode( wxXML_ELEMENT_NODE, wxT("Region"));
	document.AddChild(&region);

	wxXmlNode lod = wxXmlNode( wxXML_ELEMENT_NODE, wxT("Lod"));
	region.AddChild(&lod);


	wxXmlNode minlodpixels = wxXmlNode(wxXML_ELEMENT_NODE, wxT("minLodPixels"),boost::lexical_cast<string>(minLod));
	wxXmlNode maxlodpixels = wxXmlNode(wxXML_ELEMENT_NODE, wxT("minLodPixels"), boost::lexical_cast<string>(maxLod));
	lod.AddChild(&minlodpixels);
	lod.AddChild(&maxlodpixels);

	wxXmlNode latlonbox = wxXmlNode(wxXML_ELEMENT_NODE, wxT("LatLonAltBox"));
	region.AddChild(&latlonbox);

	wxXmlNode nodeNorth = wxXmlNode(wxXML_ELEMENT_NODE, wxT("north"), boost::lexical_cast<string>(north));
	wxXmlNode nodeSouth = wxXmlNode(wxXML_ELEMENT_NODE, wxT("south"), boost::lexical_cast<string>(south));
	wxXmlNode nodeEast = wxXmlNode(wxXML_ELEMENT_NODE, wxT("east"), boost::lexical_cast<string>(east));
	wxXmlNode nodeWest = wxXmlNode(wxXML_ELEMENT_NODE, wxT("west"), boost::lexical_cast<string>(west));

	latlonbox.AddChild(&nodeNorth);
	latlonbox.AddChild(&nodeSouth);
	latlonbox.AddChild(&nodeEast);
	latlonbox.AddChild(&nodeWest);

	wxXmlNode groundoverlay = wxXmlNode(wxXML_ELEMENT_NODE, wxT("GroundOverlay"));
	document.AddChild(&groundoverlay);

	wxXmlNode icon = wxXmlNode(wxXML_ELEMENT_NODE, wxT("Icon"));
	groundoverlay.AddChild(&icon);

	wxXmlNode href = wxXmlNode(wxXML_ELEMENT_NODE, wxT("href"),image.GetFullName());
	icon.AddChild(&href);


}