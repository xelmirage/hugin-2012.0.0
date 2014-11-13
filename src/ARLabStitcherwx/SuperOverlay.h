#pragma once

#include "ARLabStitcherwx.h"
#include "base_wx/huginConfig.h"
#include "base_wx/wxPlatform.h"
#include "base_wx/platform.h"
#include <wx/wxprec.h>
#include<wx/dir.h>
#include<wx/process.h>
#include<wx/xml/xml.h>
#include<boost/lexical_cast.hpp>

#include<algorithm>
using namespace std;


struct pointll
{
	double latitude, longitude;
};



class SuperOverlay
{
public:

	SuperOverlay(wxString imagePath, wxString kmlPath, wxString outPath);
	~SuperOverlay();

	bool build();

private:
	wxString imagePath;
	wxString kmlPath;
	wxString outPath;

	
	pointll upperleft, upperright, lowerleft, lowerright;
	vector<pointll> corners;
	bool readKml(wxString kmlFileName);
};

