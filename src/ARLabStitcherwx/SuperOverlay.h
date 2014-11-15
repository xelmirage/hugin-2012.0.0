#pragma once

#include "ARLabStitcherwx.h"
#include "base_wx/huginConfig.h"
#include "base_wx/wxPlatform.h"
#include "base_wx/platform.h"
#include <wx/wxprec.h>
#include<wx/dir.h>
#include<wx/process.h>
#include<wx/xml/xml.h>
#include <wx/gdicmn.h>
#include<boost/lexical_cast.hpp>
#include<algorithm>
#define LLFACTOR  1000000000000000
using namespace std;


class pointll
{
public:
	int64_t latitude, longitude;
	double getLatitude()
	{
		return latitude / LLFACTOR;
	}
	double getLongitude()
	{
		return longitude / LLFACTOR;
	}
};
class pyramidNode
{
private:

public:
	int maxLod, minLod;
	double north, south, west,east;
	wxFileName image,kml;
	vector<pyramidNode> networkNodes;
	

	bool readKml(wxString kmlfile);
	bool writeKml();

};


class SuperOverlay
{
public:

	SuperOverlay(wxString imagePath, wxString kmlPath, wxString outPath);
	~SuperOverlay();

	bool build();

private:
	wxFileName imagePath;
	wxFileName kmlPath;
	wxString outPath;
	int64_t llFactor;
	double theta;
	/*
	__________north_________
	|                /\
	|              /    \
	|            /        \    aL
	|          /            \
	| theta  /                \east
	|      /                  /
	|    /                  /
	|  /                  /
	|/                  /     
	|\ west           /   L
	|  \            /
	|    \        /
	|      \    /
	|        \/
	          south                */  
	int L, lod;
	double a;                                                       //longside/shortside  of source image;
	pointll upperleft, upperright, lowerleft, lowerright;
	pointll north, west, south, east;
	vector<pointll> corners;
	bool readKml(wxString kmlFileName);
	wxSize blkSize;
public:
	wxSize calc_split(wxSize imageSize, wxSize blockSize);
	bool writeKML(int pyNo, int i, int j, wxString blkName);
};

