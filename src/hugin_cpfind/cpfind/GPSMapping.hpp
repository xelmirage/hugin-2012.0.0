// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include<vector>


#include <hugin_config.h>
#include <hugin_version.h>

//#include <panodata/Panorama.h>
//#include <panodata/PTScriptParsing.h>
#include<hugin_math\hugin_math.h>

#include<boost/algorithm/string.hpp>
//#include<boost/lexical_cast.hpp>

// TODO: 在此处引用程序需要的其他头文件
// HuginGPS.cpp : 定义控制台应用程序的入口点。
//
//using namespace HuginBase;
using namespace std;
using namespace boost;

//double PI=3.1415926535897932;
struct PointD
{
	PointD()
		: x(0), y(0)
	{ }
	PointD(double x, double y)
		: x(x), y(y)
	{ }


	bool operator==(PointD rhs) const
	{
		return x == rhs.x &&  y == rhs.y;
	}

	bool operator!=(PointD rhs) const
	{
		return x != rhs.x || y != rhs.y;
	}

	PointD operator+(PointD rhs) const
	{
		return PointD (x+rhs.x, y+rhs.y);
	}

	PointD operator-(PointD rhs) const
	{
		return PointD (x-rhs.x, y-rhs.y);
	}

	PointD & operator*=(double val)
	{
		x = x*val;
		y = y*val;
		return *this;
	}

	PointD operator*(double val)
	{
		PointD result;
		result.x = x * val;
		result.y = y * val;
		return result;
	}
	double Sdistance(PointD p)
	{
		return x*p.x+y*p.y;
	}







	double x,y;

};
struct PointL
{
	PointL()
		: x(0), y(0)
	{ }
	PointL(long x, long y,double yaw)
		: x(x), y(y),yaw(yaw)
	{ }


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
		yawm=yaw+rhs.yaw;
		if (yawm>360) yawm-=360;

		return PointL (x+rhs.x, y+rhs.y,yawm);
	}

	PointL operator-(PointL rhs) const
	{
		double yawm;
		yawm=yaw-rhs.yaw;
		if (yawm<0) yawm+=360;
		return PointL (x-rhs.x, y-rhs.y,yawm);
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
		return x*p.x+y*p.y;
	}

	long distance(PointL p)
	{
		long x2,y2;
		x2=x-p.x;
		y2=y-p.y;
		//cout<<x<<","<<y<<","<<p.x<<","<<p.y<<","<<x2<<","<<y2<<endl;

		return sqrtl(x2*x2+y2*y2);
	}






public:
	long x,y;
	double yaw;

};
struct dist
{
	int id;
	long distance;
	float d_yaw;
	dist(int id, long distance,float d_yaw)
		: id(id), distance(distance),d_yaw(d_yaw)
	{ }

	bool operator <(const dist& rhs) const // 升序排序时必须写的函数
	{
		return distance < rhs.distance;
	}

	bool operator >(const dist& rhs) const // 降序排序时必须写的函数
	{
		return distance>  rhs.distance;
	}



};
typedef vector<PointD> POINTSD; 
typedef vector<PointL> POINTSL; 
typedef vector<dist> DISTS; 

void LLtoUTM(double Long, double Lat,int* UTMNorthing,int* UTMEasting ) {
	//double PI=3.1415926535897932;
	*UTMNorthing = 0;
	*UTMEasting = 0;
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
	double LongTemp = (Long + 180) - (int) ((Long + 180) / 360) * 360
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

	*UTMEasting = -(double) (k0
		* N
		* (A + (1 - T + C) * A * A * A / 6 + (5 - 18 * T + T * T
		+ 72 * C - 58 * ee)
		* A * A * A * A * A / 120) + 500000.0);
	*UTMNorthing = (double) (k0 * (M + N
		* tan(LatRad)
		* (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A
		/ 24 + (61 - 58 * T + T * T + 600 * C - 330 * ee)
		* A * A * A * A * A * A / 720)));
	// 南半球纬度起点为10000000.0m
	*UTMNorthing = *UTMNorthing + FN;
}



//int GPS_tmain(int argc, _TCHAR* argv[])
//{
//	std::string				_inputFile,line;
//	_inputFile="03.txt";
//	//POINTSD pointsD;
//	POINTSL pointsL;
//	ifstream data(_inputFile.c_str());
//	vector<string> SplitVec;
//	long minx=0,miny=0;
//
//
//
//
//
//	if (data.bad())
//	{
//		cerr << "ERROR: could not open file: '" << _inputFile << "'!" << endl;
//		return false;
//	}
//	pointsL.clear();
//
//	while (data.good()) {
//		std::getline(data, line);
//		//cout<<line<<endl;
//		SplitVec.clear();
//
//		split(SplitVec, line, is_any_of(" "), token_compress_on);
//		vector<string>::iterator i;
//
//
//
//		int UTMEasting,UTMNorthing;
//
//		double x,y;
//		x=lexical_cast<double>(SplitVec[4]);
//		y=lexical_cast<double>(SplitVec[5]);
//
//		LLtoUTM(x,y,&UTMEasting,&UTMNorthing);
//
//		pointsL.push_back(PointL(UTMEasting,UTMNorthing));;
//		if(minx==0||minx>UTMEasting)
//		{
//			minx=UTMEasting;
//		}
//
//		if(miny==0||miny>UTMNorthing)
//		{
//			miny=UTMNorthing;
//		}
//
//		//cout<<x<<"  "<<y<<"  "<<UTMEasting<<"  "<<UTMNorthing<<endl;
//
//	}
//
//	POINTSL::iterator i;
//	PointL minPoint(minx,miny);
//	for(i=pointsL.begin();i!=pointsL.end();++i)
//	{
//		(*i)=(*i)-minPoint;
//		//cout<<(*i).x<<"   "<<(*i).y<<endl;
//	}
//	int n=0;
//
//
//	for(i=pointsL.begin();i!=pointsL.end();++i)
//	{
//
//		cout<<"*************"<<n<<endl;
//		int id=0;
//		DISTS dists;
//		POINTSL::iterator j;
//		for(j=i;j!=pointsL.end();++j)
//		{
//			long d=(*i).distance(*j);
//			dist tempDist(id,d);
//			if(tempDist.distance >0)
//			{
//				dists.push_back(tempDist);
//			}
//			id++;
//
//		}
//
//		sort(dists.begin(),dists.end());
//		DISTS::iterator di;
//		for(di=dists.begin();di!=dists.begin()+10;++di)
//		{
//			cout<<(*di).id<<"    "<<(*di).distance<<endl;
//		}
//
//		n++;
//
//
//
//	}
//
//
//
//
//
//
//
//
//
//
//
//
//
//	return 0;
//}

