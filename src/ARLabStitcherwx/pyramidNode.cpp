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

	wxString minlodString;
	minlodString << minLod;
	wxString maxlodString;
	maxlodString << maxLod;
	wxXmlNode minlodpixels = wxXmlNode(wxXML_ELEMENT_NODE, wxT("minLodPixels"),minlodString);
	wxXmlNode maxlodpixels = wxXmlNode(wxXML_ELEMENT_NODE, wxT("minLodPixels"), minlodString);


	wxXmlNode latlonbox = wxXmlNode(wxXML_ELEMENT_NODE, wxT("LatLonAltBox"), minlodString);

}