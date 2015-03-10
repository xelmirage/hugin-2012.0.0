#ifndef __ARLabStitcherwxGPSFrame__
#define __ARLabStitcherwxGPSFrame__

/**
@file
Subclass of GPSFrame, which is generated by wxFormBuilder.
*/

#include "ARLabStitcherwx.h"
#include "base_wx/huginConfig.h"
#include "base_wx/wxPlatform.h"
#include "base_wx/platform.h"
#include <wx/wxprec.h>
#include<wx/dir.h>
#include<wx/process.h>
#include<wx/dcbuffer.h>
#include<boost/lexical_cast.hpp>
#include<boost/algorithm/string.hpp>
//// end generated include
#include <iostream>
#include<vector>
using namespace std;
using namespace boost;
struct dist
{
	int id;
	long distance;
	float d_yaw;
	dist(int id, long distance, float d_yaw)
		: id(id), distance(distance), d_yaw(d_yaw)
	{ }

	bool operator <(const dist& rhs) const // 升序排序时必须写的函数
	{
		return distance < rhs.distance;
	}

	bool operator >(const dist& rhs) const // 降序排序时必须写的函数
	{
		return distance > rhs.distance;
	}



};
struct PointL
{
	PointL()
		: x(0), y(0)
	{
		selected = false;
	}
	PointL(int id, long x, long y, double yaw, double roll)
		:id(id), x(x), y(y), yaw(yaw), roll(roll)
	{
		selected = false;
	}


	bool operator==(PointL rhs) const
	{
		return x == rhs.x &&  y == rhs.y;
	}

	bool operator!=(PointL rhs) const
	{
		return x != rhs.x || y != rhs.y;
	}

	PointL operator+(PointL rhs) const
	{
		double yawm;
		yawm = yaw + rhs.yaw;
		if (yawm > 360) yawm -= 360;

		return PointL(id, x + rhs.x, y + rhs.y, yawm, roll);
	}

	PointL operator-(PointL rhs) const
	{
		double yawm;
		yawm = yaw - rhs.yaw;
		if (yawm < 0) yawm += 360;
		return PointL(id, x - rhs.x, y - rhs.y, yawm, roll);
	}

	PointL & operator*=(long val)
	{
		x = x*val;
		y = y*val;
		return *this;
	}

	PointL operator*(long val)
	{
		PointL result;
		result.x = x * val;
		result.y = y * val;
		return result;
	}
	long Sdistance(PointL p)
	{
		return x*p.x + y*p.y;
	}

	long distance(PointL p)
	{
		long x2, y2;
		x2 = x - p.x;
		y2 = y - p.y;
		//cout<<x<<","<<y<<","<<p.x<<","<<p.y<<","<<x2<<","<<y2<<endl;

		return sqrtl(x2*x2 + y2*y2);
	}

	double slope(PointL p)
	{
		double dx, dy;
		dx = p.x - x;
		dy = p.y - y;
		if (dx != 0)
			return dy / dx;
		else
			return NULL;
	}

	double heading(PointL p)
	{
		double slp;
		double dx, dy;
		dx = p.x - x;
		dy = p.y - y;
		if (dx != 0)
		{
			slp = dy / dx;
			if (dx > 0)
			{
				return 0.5*PI - atan(slp);
			}
			else
			{
				return 1.5*PI - atan(slp);
			}
		}
		else
		{
			if (dy >= 0)
				return 0;
			else
				return 1;
		}


	}
public:
	int id;
	long x, y;
	float yaw, dyaw, roll;

	int yawint;
	bool selected;
};
struct  Line
{
	wxPoint a, b;
};
typedef vector<PointL> POINTSL;
typedef vector<POINTSL> POINTSLV;
typedef vector<dist> DISTS;
typedef vector<DISTS> DISTSV;
typedef vector<Line> LINES;
/** Implementing GPSFrame */
class ARLabStitcherwxGPSFrame : public GPSFrame
{
public:
	/** Constructor */
	ARLabStitcherwxGPSFrame(wxWindow* parent) :
		GPSFrame(parent)
	{
		Ready = false;
		SetBackgroundStyle(wxBG_STYLE_CUSTOM);
		m_doubleBuffer = NULL;
	};

	//// end generated class members
	void OnPaint(wxPaintEvent& event);
	void process();
	int getReady();
	DECLARE_EVENT_TABLE()
private:
	std::string _inputFile,_GPSResult;
	
	int UTMNorthing;
	int UTMEasting;
	int maxx, maxy;
	bool Ready;
	void LLtoUTM(double Long, double Lat) {
		//double PI=3.1415926535897932;
		UTMNorthing = 0;
		UTMEasting = 0;
		double a = 6378137; // 椭球长半轴
		double b = 6356752.3142451793; // 椭球短半轴
		double f = (a - b) / a; // 扁率
		double FN = 0;
		// e表示WGS84第一偏心率,de表示e的平方,
		double de = (2 * f - f * f);
		double k0 = 1;
		double ee = (de) / (1 - de);
		double N, T, C, A, M;

		// 确保longtitude位于-180.00----179.9之间
		double LongTemp = (Long + 180) - (int)((Long + 180) / 360) * 360
			- 180;
		double LatRad = Lat * PI / 180;
		double LongRad = LongTemp * PI / 180;
		double LongOriginRad = 129 * PI / 180;

		N = a / sqrt(1 - de * sin(LatRad) * sin(LatRad));
		T = tan(LatRad) * tan(LatRad);
		C = ee *cos(LatRad) * cos(LatRad);
		A = cos(LatRad) * (LongRad - LongOriginRad);
		M = a
			* ((1 - de / 4 - 3 * de * de / 64 - 5 * de * de * de / 256)
			* LatRad
			- (3 * de / 8 + 3 * de * de / 32 + 45 * de * de
			* de / 1024) * sin(2 * LatRad)
			+ (15 * de * de / 256 + 45 * de * de * de / 1024)
			* sin(4 * LatRad) - (35 * de * de * de / 3072)
			* sin(6 * LatRad));

		UTMEasting = -(double)(k0
			* N
			* (A + (1 - T + C) * A * A * A / 6 + (5 - 18 * T + T * T
			+ 72 * C - 58 * ee)
			* A * A * A * A * A / 120) + 500000.0);
		UTMNorthing = (double)(k0 * (M + N
			* tan(LatRad)
			* (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A
			/ 24 + (61 - 58 * T + T * T + 600 * C - 330 * ee)
			* A * A * A * A * A * A / 720)));
		// 南半球纬度起点为10000000.0m
		UTMNorthing = UTMNorthing + FN;
	}
	POINTSL pointsL;//航带
	LINES lines;
	wxBitmap* m_doubleBuffer;
public:
	void setGPSFileName(wxString name);
	void setResultName(wxString resName);
	void OnResize(wxSizeEvent& e);
	void paint();
	void OnErase(wxEraseEvent& e);
	bool isReady();
};


#endif // __ARLabStitcherwxGPSFrame__
