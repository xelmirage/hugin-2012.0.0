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
	pointll upperleft, upperright, lowerright, lowerleft;
	vector<pointll> corners;
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
	wxSize blkSize;
	double xa1, xa2, xb, ya1, ya2, yb;


	bool readKml(wxString kmlFileName);
	void calc_reg(int width, int height);
public:
	wxSize calc_split(wxSize imageSize, wxSize blockSize);
	bool writeKML(int pyNo, int i, int j, wxString blkName);
};


void FreeData(double **dat, double *d, int count)
{
	int i, j;
	free(d);
	for (i = 0; i < count; i++)
		free(dat[i]);
	free(dat);
}
int LinearEquations(double *data, int count, double *Answer)
{
	int j, m, n;
	double tmp, **dat, *d = data;
	dat = (double**)malloc(count * sizeof(double*));
	for (m = 0; m < count; m++, d += (count + 1))
	{
		dat[m] = (double*)malloc((count + 1) * sizeof(double));
		memcpy(dat[m], d, (count + 1) * sizeof(double));
	}
	d = (double*)malloc((count + 1) * sizeof(double));
	for (m = 0; m < count - 1; m++)
	{
		// 如果主对角线元素为0，行交换
		for (n = m + 1; n < count && dat[m][m] == 0.0; n++)
		{
			if (dat[n][m] != 0.0)
			{
				memcpy(d, dat[m], (count + 1) * sizeof(double));
				memcpy(dat[m], dat[n], (count + 1) * sizeof(double));
				memcpy(dat[n], d, (count + 1) * sizeof(double));
			}
		}
		// 行交换后，主对角线元素仍然为0，无解，返回-1
		if (dat[m][m] == 0.0)
		{
			FreeData(dat, d, count);
			return -1;
		}
		// 消元
		for (n = m + 1; n < count; n++)
		{
			tmp = dat[n][m] / dat[m][m];
			for (j = m; j <= count; j++)
				dat[n][j] -= tmp * dat[m][j];
		}
	}
	for (j = 0; j < count; j++)
		d[j] = 0.0;
	// 求得count - 1的元
	Answer[count - 1] = dat[count - 1][count] / dat[count - 1][count - 1];
	// 逐行代入求各元
	for (m = count - 2; m >= 0; m--)
	{
		for (j = count - 1; j > m; j--)
			d[m] += Answer[j] * dat[m][j];
		Answer[m] = (dat[m][count] - d[m]) / dat[m][m];
	}
	FreeData(dat, d, count);
	return 0;
}
int MultipleRegression(double *data, int rows, int cols, double *Answer, double *SquarePoor)
{
	int m, n, i, count = cols - 1;
	double *dat, *p, a, b;
	if (data == 0 || Answer == 0 || rows < 2 || cols < 2)
		return -1;
	dat = (double*)malloc(cols * (cols + 1) * sizeof(double));
	dat[0] = (double)rows;
	for (n = 0; n < count; n++)                     // n = 0 to cols - 2
	{
		a = b = 0.0;
		for (p = data + n, m = 0; m < rows; m++, p += cols)
		{
			a += *p;
			b += (*p * *p);
		}
		dat[n + 1] = a;                              // dat[0, n+1] = Sum(Xn)
		dat[(n + 1) * (cols + 1)] = a;               // dat[n+1, 0] = Sum(Xn)
		dat[(n + 1) * (cols + 1) + n + 1] = b;       // dat[n+1,n+1] = Sum(Xn * Xn)
		for (i = n + 1; i < count; i++)             // i = n+1 to cols - 2
		{
			for (a = 0.0, p = data, m = 0; m < rows; m++, p += cols)
				a += (p[n] * p[i]);
			dat[(n + 1) * (cols + 1) + i + 1] = a;   // dat[n+1, i+1] = Sum(Xn * Xi)
			dat[(i + 1) * (cols + 1) + n + 1] = a;   // dat[i+1, n+1] = Sum(Xn * Xi)
		}
	}
	for (b = 0.0, m = 0, p = data + n; m < rows; m++, p += cols)
		b += *p;
	dat[cols] = b;                                   // dat[0, cols] = Sum(Y)
	for (n = 0; n < count; n++)
	{
		for (a = 0.0, p = data, m = 0; m < rows; m++, p += cols)
			a += (p[n] * p[count]);
		dat[(n + 1) * (cols + 1) + cols] = a;        // dat[n+1, cols] = Sum(Xn * Y)
	}
	n = LinearEquations(dat, cols, Answer);          // 计算方程式
	// 方差分析
	if (n == 0 && SquarePoor)
	{
		b = b / rows;                                // b = Y的平均值
		SquarePoor[0] = SquarePoor[1] = 0.0;
		p = data;
		for (m = 0; m < rows; m++, p++)
		{
			for (i = 1, a = Answer[0]; i < cols; i++, p++)
				a += (*p * Answer[i]);               // a = Ym的估计值
			SquarePoor[0] += ((a - b) * (a - b));    // U(回归平方和)
			SquarePoor[1] += ((*p - a) * (*p - a));  // Q(剩余平方和)(*p = Ym)
		}
		SquarePoor[2] = SquarePoor[0] / count;       // 回归方差
		if (rows - cols > 0.0)
			SquarePoor[3] = SquarePoor[1] / (rows - cols); // 剩余方差
		else
			SquarePoor[3] = 0.0;
	}
	free(dat);
	return n;
}

