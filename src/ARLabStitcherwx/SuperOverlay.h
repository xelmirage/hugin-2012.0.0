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
#define PI 3.141592653589793
using namespace std;


class pointll
{
public:
	int64_t latitude, longitude;

	double setLatitude(double l)
	{
		latitude = l*LLFACTOR;
		return latitude ;
	}
	double setLongitude(double l)
	{
		longitude = l*LLFACTOR;
		return longitude;
	}


	double getLatitude()
	{
		return (double)latitude / (double)LLFACTOR;
	}
	double getLongitude()
	{
		return (double)longitude / (double)LLFACTOR;
	}
};
class pyramidNode
{


public:
	pyramidNode();
	~pyramidNode();



	int maxLod, minLod;
	double north, south, west,east,rotation;
	
	vector<pointll> corners; //upperleft, upperright, lowerright, lowerleft;
	wxFileName image,kml;
	vector<pyramidNode> networkNodes;
	

	bool readKml(wxString kmlfile);
	bool writeKml();
	void getBox();
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
	int UTMNorthing, UTMEasting;



	double theta,rotate;
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
	 
	pointll north, west, south, east;
	vector<pointll> corners; //from 0 to 1 is upperleft, upperright, lowerleft, lowerright;
	wxSize blkSize;
	double xa1, xa2, xb, ya1, ya2, yb;


	bool readKml(wxString kmlFileName);
	void calc_reg(int width, int height);
	void FreeData(double **dat, double *d, int count);
	int LinearEquations(double *data, int count, double *Answer);
	int MultipleRegression(double *data, int rows, int cols, double *Answer, double *SquarePoor);
	void LLtoUTM(double Long, double Lat);
public:
	wxSize calc_split(wxSize imageSize, wxSize blockSize);
	bool writeKML(int pyNo, int i, int j, wxString blkName);
	void multiReg(int x, int y, double& lon, double& lat);
	
};


