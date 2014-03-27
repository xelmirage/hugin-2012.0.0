
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include<vector>
#include <getopt.h>

#include <hugin_config.h>
#include <hugin_version.h>
#include <hugin_basic.h>
#include <hugin_utils/stl_utils.h>
#include "PT/Panorama.h"
#include "PT/utils.h"
//#include <panodata/Panorama.h>
//#include <panodata/PTScriptParsing.h>
#include<hugin_math\hugin_math.h>
//#include<hugin_base\makefilelib\Manager.h>
#include<boost/algorithm/string.hpp>
#include<boost/lexical_cast.hpp>
#include<boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <cassert>
#include "zthread/Runnable.h"
#include "zthread/PoolExecutor.h"
//#include "Utils.h"
using namespace std;
using namespace boost;
using namespace ZThread; 
using namespace AppBase;
using namespace hugin_utils;
using namespace HuginBase;



void Display(double *dat, double *Answer, double *SquarePoor, int rows, int cols)
{
    double v, *p;
    int i, j;
    printf("回归方程式:    Y = %.10lf", Answer[0]);
    for (i = 1; i < cols; i ++)
        printf(" + %.10lf*X%d", Answer[i], i);
    printf(" ");
    printf("回归显著性检验: ");
    printf("回归平方和：%12.4lf  回归方差：%12.4lf \n", SquarePoor[0], SquarePoor[2]);
    printf("剩余平方和：%12.4lf  剩余方差：%12.4lf \n", SquarePoor[1], SquarePoor[3]);
    printf("离差平方和：%12.4lf  标准误差：%12.4lf \n", SquarePoor[0] + SquarePoor[1], sqrt(SquarePoor[3]));
    printf("F   检  验：%12.4lf  相关系数：%12.4lf \n", SquarePoor[2] /SquarePoor[3],
           sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1])));
    printf("剩余分析: \n");
    printf("      观察值      估计值      剩余值    剩余平方 \n");
    for (i = 0, p = dat; i < rows; i ++, p ++)
    {
        v = Answer[0];
        for (j = 1; j < cols; j ++, p ++)
            v += *p * Answer[j];
        //printf("%12.2lf%12.2lf%12.2lf%12.2lf \n", *p, v, *p - v, (*p - v) * (*p - v));
    }
    std::system("pause");
}
void FreeData(double **dat, double *d, int count)
{
    int i, j;
    free(d);
    for (i = 0; i < count; i ++)
        free(dat[i]);
    free(dat);
}
// 解线性方程。data[count*(count+1)]矩阵数组；count：方程元数；
// Answer[count]：求解数组 。返回：0求解成功，-1无解或者无穷解
int LinearEquations(double *data, int count, double *Answer)
{
    int j, m, n;
    double tmp, **dat, *d = data;
    dat = (double**)malloc(count * sizeof(double*));
    for (m = 0; m < count; m ++, d += (count + 1))
    {
        dat[m] = (double*)malloc((count + 1) * sizeof(double));
        memcpy(dat[m], d, (count + 1) * sizeof(double));
    }
    d = (double*)malloc((count + 1) * sizeof(double));
    for (m = 0; m < count - 1; m ++)
    {
        // 如果主对角线元素为0，行交换
        for (n = m + 1; n < count && dat[m][m] == 0.0; n ++)
        {
            if ( dat[n][m] != 0.0)
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
        for (n = m + 1; n < count; n ++)
        {
            tmp = dat[n][m] / dat[m][m];
            for (j = m; j <= count; j ++)
                dat[n][j] -= tmp * dat[m][j];
        }
    }
    for (j = 0; j < count; j ++)
        d[j] = 0.0;
    // 求得count - 1的元
    Answer[count - 1] = dat[count - 1][count] / dat[count - 1][count - 1];
    // 逐行代入求各元
    for (m = count - 2; m >= 0; m --)
    {
        for (j = count - 1; j > m; j --)
            d[m] += Answer[j] * dat[m][j];
        Answer[m] = (dat[m][count] - d[m]) / dat[m][m];
    }
    FreeData(dat, d, count);
    return 0;
}

// 求多元回归方程：Y = B0 + B1X1 + B2X2 + ...BnXn
// data[rows*cols]二维数组；X1i,X2i,...Xni,Yi (i=0 to rows-1)
// rows：数据行数；cols数据列数；Answer[cols]：返回回归系数数组(B0,B1...Bn)
// SquarePoor[4]：返回方差分析指标: 回归平方和，剩余平方和，回归平方差，剩余平方差
// 返回值：0求解成功，-1错误
int MultipleRegression(double *data, int rows, int cols, double *Answer, double *SquarePoor)
{
    int m, n, i, count = cols - 1;
    double *dat, *p, a, b;
    if (data == 0 || Answer == 0 || rows < 2 || cols < 2)
        return -1;
    dat = (double*)malloc(cols * (cols + 1) * sizeof(double));
    dat[0] = (double)rows;
    for (n = 0; n < count; n ++)                     // n = 0 to cols - 2
    {
        a = b = 0.0;
        for (p = data + n, m = 0; m < rows; m ++, p += cols)
        {
            a += *p;
            b += (*p * *p);
        }
        dat[n + 1] = a;                              // dat[0, n+1] = Sum(Xn)
        dat[(n + 1) * (cols + 1)] = a;               // dat[n+1, 0] = Sum(Xn)
        dat[(n + 1) * (cols + 1) + n + 1] = b;       // dat[n+1,n+1] = Sum(Xn * Xn)
        for (i = n + 1; i < count; i ++)             // i = n+1 to cols - 2
        {
            for (a = 0.0, p = data, m = 0; m < rows; m ++, p += cols)
                a += (p[n] * p[i]);
            dat[(n + 1) * (cols + 1) + i + 1] = a;   // dat[n+1, i+1] = Sum(Xn * Xi)
            dat[(i + 1) * (cols + 1) + n + 1] = a;   // dat[i+1, n+1] = Sum(Xn * Xi)
        }
    }
    for (b = 0.0, m = 0, p = data + n; m < rows; m ++, p += cols)
        b += *p;
    dat[cols] = b;                                   // dat[0, cols] = Sum(Y)
    for (n = 0; n < count; n ++)
    {
        for (a = 0.0, p = data, m = 0; m < rows; m ++, p += cols)
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
        for (m = 0; m < rows; m ++, p ++)
        {
            for (i = 1, a = Answer[0]; i < cols; i ++, p ++)
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
static void usage(const char* name)
{
	cout << name << ": Connect centers with GPS data" << endl
		<< name << " version " << DISPLAY_VERSION << endl
		<< endl
		<< "Usage:  " << name << " [options]" << endl
		<< endl
		<< "  Options:" << endl
		<< "     -c, --centerfile=file  input centers." << endl

		<< "     -p, --ptofile=file.pto    input pto file" << endl
		<< "     -o, --outputfile=file.txt Specify output data file" << endl
		<< "     -h, --help             Shows this help" << endl
		<< endl;
}
int main(int argc,char* argv[])
{
	std::string				_centerFile="",line="",_outputFile="",_ptoFile="";

	int c;
	int optionIndex = 0;
	const char* optstring = "c:p:o:h";
	static struct option longOptions[] =
	{

		{"centerfile", required_argument, NULL, 'c' },
		{"ptofile", required_argument, NULL, 'p' },
		{"outputfile", required_argument, NULL, 'o' },
		//{"gpsoutput", required_argument, NULL, 'p' },
		//{"stacklength", required_argument, NULL, 's' },
		//{"linkstacks", no_argument, NULL, 'l' },

		{"help", no_argument, NULL, 'h' },

		0
	};

	while ((c = getopt_long (argc, argv, optstring, longOptions,&optionIndex)) != -1)
	{
		switch (c)
		{
		case 'c':
			_centerFile=optarg;
			cout<<"_centerFile="<<_centerFile<<endl;

			break;
		case 'p':
			_ptoFile=optarg;
			cout<<"_ptoFile="<<_ptoFile<<endl;
			break;


		case 'o':
			_outputFile=optarg;
			cout<<"_outputFile="<<_outputFile<<endl;
			break;


		case ':':
			cerr <<"Option " << longOptions[optionIndex].name << " requires a number" << endl;
			return 1;
			break;
		case '?':
			break;
		case 'h':
			usage(argv[0]);
			return 0;
		default:
			abort ();
		}
	}
	HuginBase::Panorama pano;
	ifstream prjfile(_ptoFile.c_str());
	if (!prjfile.good()) {
		cerr << "could not open ptoFile : " <<_ptoFile << endl;
		return 1;
	}
	pano.setFilePrefix(hugin_utils::getPathPrefix(_ptoFile));
	DocumentData::ReadWriteError err = pano.readData(prjfile);
	if (err != DocumentData::SUCCESSFUL) {
		cerr << "error while parsing panos tool script: " << _ptoFile << endl;
		cerr << "DocumentData::ReadWriteError code: " << err << endl;
		return 1;
	}
	PanoramaOptions opt = pano.getOptions();
	for(int i=0;i<pano.getNrOfImages();++i)
	{
		//cout<<pano.getImage(i).getFilename()<<endl;
	}

	ifstream data(_centerFile.c_str());
	if (data.bad())
	{
		cerr << "ERROR: could not open file: '" <<_centerFile << "'!" << endl;
		return false;
	}

	ofstream out(_outputFile.c_str());
	if (out.bad())
	{
		cerr << "ERROR: could not open file: '" <<_outputFile << "'!" << endl;
		return false;
	}
	vector<string> SplitVec;
	vector<vector<double>> mat;
	mat.clear();
	int imgNr;
	string fname;
	while (data.good()) {
		std::getline(data, line);
		SplitVec.clear();
		split(SplitVec,line,is_any_of(","),token_compress_on);
		if (SplitVec.size()<=2) continue;
		imgNr=boost::lexical_cast<int>(SplitVec[0]);
		if(imgNr>=pano.getNrOfImages()) break;
		fname=pano.getImage(imgNr).getFilename();
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(fname);
		assert (image.get() != 0);
		image->readMetadata();
		Exiv2::ExifData &exifData = image->exifData();
		
		cout<<imgNr<<"   "<<fname<<endl;
		cout.precision(6); 
		float lat=exifData["Exif.GPSInfo.GPSLatitude"].toFloat(0)
			+exifData["Exif.GPSInfo.GPSLatitude"].toFloat(1)/60
			+exifData["Exif.GPSInfo.GPSLatitude"].toFloat(2)/3600;
		float lon=exifData["Exif.GPSInfo.GPSLongitude"].toFloat(0)
			+exifData["Exif.GPSInfo.GPSLongitude"].toFloat(1)/60
			+exifData["Exif.GPSInfo.GPSLongitude"].toFloat(2)/3600;
		cout<<"Latitude:"<<setprecision(10)<<lat<<"  "<<exifData["Exif.GPSInfo.GPSLatitude"].toString()<<"   "<<exifData["Exif.GPSInfo.GPSLatitude"]<<endl;
		cout<<"Longitude:"<<setprecision(10)<<lon<<"  "<<exifData["Exif.GPSInfo.GPSLongitude"].toString()<<"   "<<exifData["Exif.GPSInfo.GPSLongitude"]<<endl;
		out<<imgNr<<" "<<SplitVec[1]<<" "<<SplitVec[2]<<" "<<setprecision(10)<<lon<<" "<<setprecision(10)<<lat<<endl;
		vector<double> row;
		row.clear();
		row.push_back (boost::lexical_cast<double>(SplitVec[1]));
		row.push_back (boost::lexical_cast<double>(SplitVec[2]));
		row.push_back (lon);
		row.push_back (lat);
		mat.push_back(row);


	}
	out.close();
	
	
	double *_data;
	_data = (double*)malloc(3*mat.size()*sizeof(double));
	double *answer=(double*)malloc(3*(sizeof(double)));
	for(int i=0;i<mat.size();++i)
	{
		*(_data+i*3)=mat[i][0];
		*(_data+i*3+1)=mat[i][1];
		*(_data+i*3+2)=mat[i][2];

	}
	
	

	double SquarePoor[4];
	if (MultipleRegression(_data, mat.size(), 3, answer, SquarePoor) == 0)
		Display(_data, answer, SquarePoor, mat.size(), 3);


	for(int i=0;i<mat.size();++i)
	{
		*(_data+i*3)=mat[i][0];
		*(_data+i*3+1)=mat[i][1];
		*(_data+i*3+2)=mat[i][3];

	}
	
	


	if (MultipleRegression(_data, mat.size(), 3, answer, SquarePoor) == 0)
		Display(_data, answer, SquarePoor, mat.size(), 3);




	free(_data);
	free(answer);

	

	return 0;
}