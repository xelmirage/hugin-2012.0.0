// GPSFilter.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include<vector>
#include <getopt.h>
#include <wx/wxprec.h>
#include <hugin_config.h>
#include "base_wx/huginConfig.h"
#include <hugin_version.h>
#include <hugin_utils/stl_utils.h>
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
int UTMNorthing;
int UTMEasting;
wxString huginExeDir;
class runexif : public ZThread::Runnable
{
private:
	string cmd;
	string name;
public:
	runexif(string cmd,string name):cmd(cmd),name(name){}
	void run()
	{
		cout<<name<<endl;
		std::system(cmd.c_str());
		
	}
};


struct dist
{
	int id;
	long distance;
	float d_yaw;
	dist(int id, long distance,float d_yaw)
		: id(id), distance(distance),d_yaw(d_yaw)
	{ }

	bool operator <(const dist& rhs) const // ��������ʱ����д�ĺ���
	{
		return distance < rhs.distance;
	}

	bool operator >(const dist& rhs) const // ��������ʱ����д�ĺ���
	{
		return distance>  rhs.distance;
	}



};
struct PointL
{
	PointL()
		: x(0), y(0)
	{
		selected=false; 
	}
	PointL(int id,long x, long y,double yaw,double roll)
		:id(id), x(x), y(y),yaw(yaw),roll(roll)
	{
		selected=false;
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
		yawm=yaw+rhs.yaw;
		if (yawm>360) yawm-=360;

		return PointL (id,x+rhs.x, y+rhs.y,yawm,roll);
	}

	PointL operator-(PointL rhs) const
	{
		double yawm;
		yawm=yaw-rhs.yaw;
		if (yawm<0) yawm+=360;
		return PointL (id,x-rhs.x, y-rhs.y,yawm,roll);
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

	double slope(PointL p)
	{
		double dx,dy;
		dx=p.x-x;
		dy=p.y-y;
		if(dx!=0)
			return dy/dx;
		else 
			return NULL;
	}

	double heading(PointL p)
	{
		double slp;
		double dx,dy;
		dx=p.x-x;
		dy=p.y-y;
		if(dx!=0)
		{
			slp=dy/dx;
			if(dx>0)
			{
				return 0.5*PI-atan(slp);
			}
			else 
			{
				return 1.5*PI-atan(slp);
			}
		}
		else
		{
			if(dy>=0)
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
typedef vector<PointL> POINTSL; 
typedef vector<POINTSL> POINTSLV; 
typedef vector<dist> DISTS;
typedef vector<DISTS> DISTSV;
POINTSL pointsL;//����
POINTSLV vector_pointsL;//��������
DISTSV rows; //�洢�����ŵ�����
DISTS row;
vector<string> images;
void LLtoUTM(double Long, double Lat) {
	//double PI=3.1415926535897932;
	UTMNorthing = 0;
	UTMEasting = 0;
	double a = 6378137; // ���򳤰���
	double b = 6356752.3142451793; // ����̰���
	double f = (a - b) / a; // ����
	double FN = 0;
	// e��ʾWGS84��һƫ����,de��ʾe��ƽ��,
	double de = (2 * f - f * f);
	double k0 = 1;
	double ee = (de) / (1 - de);
	double N, T, C, A, M;

	// ȷ��longtitudeλ��-180.00----179.9֮��
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

	UTMEasting = -(double) (k0
		* N
		* (A + (1 - T + C) * A * A * A / 6 + (5 - 18 * T + T * T
		+ 72 * C - 58 * ee)
		* A * A * A * A * A / 120) + 500000.0);
	UTMNorthing = (double) (k0 * (M + N
		* tan(LatRad)
		* (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A
		/ 24 + (61 - 58 * T + T * T + 600 * C - 330 * ee)
		* A * A * A * A * A * A / 720)));
	// �ϰ���γ�����Ϊ10000000.0m
	UTMNorthing = UTMNorthing + FN;
}
int findmost()
{
	int len=pointsL.size();
	int* data=new int[len];
	int* sum=new int[len];
	int i=0, j=0, datalen=0, most=0;
	bool findflag;
	for (i=0;i<len;i++)
	{
		findflag=false;
		for (j=0;j<datalen;j++)
		{
			if (data[j]==pointsL[i].yawint)
			{
				sum[j]+=1;
				findflag=true;
			}
		}
		if (!findflag)
		{
			data[j]=pointsL[i].yawint;
			datalen++;
			sum[j]=0;
		}
	}
	for (i=0;i<datalen;i++)
	{

		if (sum[i]>sum[most])
			most=i;
	}



	return data[most];
}
void find_nearest(POINTSL::iterator point_position,POINTSLV::iterator belt_position)
{
	DISTS dists;
	POINTSL::iterator point_iterato;
	POINTSLV::iterator belt_iterator=belt_position+1;

	if(belt_iterator==vector_pointsL.end())
	{
		cout<<endl;
		return;
	}




	dists.clear();
	POINTSL::iterator i1;
	for(i1=(*belt_iterator).begin();i1!=(*belt_iterator).end();++i1)
	{

		long d=point_position->distance(*i1);
		dist tempDist(i1->id,d,0);
		dists.push_back(tempDist);

	}
	sort(dists.begin(),dists.end());

	for(int ii=0;ii<1;ii++)
	{
		cout<<dists[ii].id<<",";
		//					belt_out<<dists[ii].id<<",";

		row.push_back(dists[ii]);
		//tag+=","+lexical_cast<std::string>(dists[ii].id);
		//belt_out<<","<<dists[ii].id;
	}



	for(i1=(*belt_iterator).begin();i1!=(*belt_iterator).end();++i1)
	{
		if(dists[0].id==i1->id)
		{
			find_nearest(i1,belt_iterator);
		}
	}












	//			belt_out<<endl;



}
bool is_next_valid(int id,int step)
{
	POINTSL::iterator point_iterator;
	POINTSLV::iterator belt_iterator;
	for(belt_iterator=vector_pointsL.begin();belt_iterator!=vector_pointsL.end();++belt_iterator)
	{
		for(point_iterator=(*belt_iterator).begin();point_iterator!=(*belt_iterator).end();++point_iterator)
		{
			if((*point_iterator).id==id)
			{
				if((point_iterator+step)<(*belt_iterator).end())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
}
void build_belt(string outfile)
{
	int _cores=hugin_utils::getCPUCount();
	boost::thread_group threads;
	POINTSLV::iterator belt_iterator;
	int row_step=1;    //ѡȡ��Ƭ�ļ��
	DISTS dists;
	dists.clear();
	string tag;
	tag.clear();
	bool valid;
	string _outputFile=outfile;
	ofstream belt_out(_outputFile.c_str());
	POINTSL::iterator i;
	ZThread::PoolExecutor aExecutor(_cores);
	for(i=pointsL.begin();i!=pointsL.end()-1;++i)
	{
		if(!i->selected)
		{
			if((i+1)->selected)
			{
				POINTSL pl;
				vector_pointsL.push_back(pl);
			}
			else
			{}
		}
		else
		{
			(vector_pointsL.end()-1)->push_back((*i));


		}


	}
	POINTSLV::iterator j;
	for(j=vector_pointsL.begin();j!=vector_pointsL.end();)
	{
		//cout<<j->size()<<endl;
		//�޳���Ƭ��С��10�ĺ���
		if(j->size()<10)
		{


			vector_pointsL.erase(j);
			//continue;
			//j=vector_pointsL.begin();

		}
		else
		{
			++j;
		}


	}
	rows.clear();


	for(j=vector_pointsL.begin();j!=vector_pointsL.begin()+1;++j)
	{


		for(i=(*j).begin();i<(*j).end()-1;i+=row_step)
		{
			dists.clear();
			
			int img_id;
			img_id=lexical_cast<int>(i->id);
			


			//belt_out<<images[img_id-1]<<endl;

			cout<<i->id<<",";



			//find_nearest(i,j);
			//belt_out<<images[img_id-1]<<endl;
			
			row.clear();
			row.push_back(dist(i->id,0,0));
				for(belt_iterator=j+1;belt_iterator!=vector_pointsL.end();++belt_iterator)
				{
					dists.clear();
					POINTSL::iterator i1;
					for(i1=(*belt_iterator).begin();i1!=(*belt_iterator).end();++i1)
					{

						long d=i->distance(*i1);
						dist tempDist(i1->id,d,0);
						dists.push_back(tempDist);

					}
					sort(dists.begin(),dists.end());
					for(int ii=0;ii<1;ii++)
					{
						/*if (belt_iterator==(j+1)) 
						{tag+=","+lexical_cast<std::string>(dists[ii].id);}*/
						//belt_out<<","<<dists[ii].id;
						//cout<<","<<dists[ii].id;
						row.push_back(dists[ii]);
					}

				}
				





				rows.push_back(row);

				//cout<<endl;
				//cout<<images[img_id-1]<<endl;

				DISTS::iterator row_iterator;
				for(row_iterator=row.begin();row_iterator!=row.end();++row_iterator)
				{
					cout<<row_iterator->id<<",";
					//belt_out<<row_iterator->id<<",";
					tag.clear();
					tag="selected ";
					tag+=lexical_cast<std::string>(row_iterator->id);
					tag+=" pairs_with ";
					valid=is_next_valid(row_iterator->id,row_step);
					if(valid)
					{
						tag+=lexical_cast<std::string>(row_iterator->id+row_step);
					}
					if(row_iterator!=row.end()-1)
					{
						if (valid)
						{
							tag+=","+lexical_cast<std::string>((row_iterator+1)->id);
						}
						else
						{
							tag+=lexical_cast<std::string>((row_iterator+1)->id);

						}
					}
					//cout<<endl<<tag<<endl;
					/*	Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(images[(row_iterator->id-1)]);
					assert (image.get() != 0);
					image->readMetadata();
					Exiv2::ExifData &exifData = image->exifData();
					exifData["Exif.Photo.UserComment"]
					= "charset=Ascii "+tag;
					image->writeMetadata();*/



					string cmd="exiftool -F -m -overwrite_original -UserComment=\""+tag+"\" "
						+images[(row_iterator->id-1)];
					//threads.add_thread(new boost::thread(boost::bind(&run,cmd)));
					//std::system(cmd.c_str());

					aExecutor.execute(new runexif(cmd,images[(row_iterator->id-1)]));

				}
				cout<<endl;
				//belt_out<<images[i->id-1]<<" ";                          //��Ҫ����images[]�������ʱҪ��id-1����Ϊ��id��ʱ��û��0


				for(row_iterator=row.begin();row_iterator!=row.end();++row_iterator)
				{
					cout<<images[(*row_iterator).id-1]<<endl;
					belt_out<<images[(*row_iterator).id-1]<<" ";

				}
				cout<<endl;


				aExecutor.wait();

				//belt_out<<endl;
				



				//threads.join_all();
			
		
		}	


	}



	cout<<endl<<endl<<"Filtered successfully    ;-)"<<endl;


}

static void usage(const char* name)
{
    cout << name << ": Filtting images with GPS data" << endl
         << name << " version " << DISPLAY_VERSION << endl
         << endl
         << "Usage:  " << name << " [options]" << endl
         << endl
         << "  Options:" << endl
         << "     -o, --output=file.pto  Output Hugin PTO file." << endl
       
         << "     -s, --sourcedir=dir    directory contains images to be handled" << endl
         << "                            (default: 1, no stacks)" << endl
         << "     -g, --gpsfile=file.txt Specify GPS data file" << endl
         << "     -h, --help             Shows this help" << endl
         << endl;
}

int main(int argc,char* argv[])
{
	int _cores=hugin_utils::getCPUCount();
	int c;
	int optionIndex = 0;
	std::string				_inputFile="",line="",_outputFile="",_inputDIR="";
	ZThread::PoolExecutor aExecutor(_cores);
	huginExeDir = getExePath(argv[0]);
	
	const char* optstring = "o:s:g:h";
	static struct option longOptions[] =
    {
		
        {"output", required_argument, NULL, 'o' },
        {"sourcedir", required_argument, NULL, 's' },
        {"gpsfile", required_argument, NULL, 'g' },
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
			case 'g':
				_inputFile=optarg;
				cout<<"inputfile "<<_inputFile<<endl;
                
                break;
            case 'o':
                _outputFile=optarg;
				cout<<"outputfile "<<_outputFile<<endl;
                break;
           
            case 'p':
                {
                  
                };
                break;
            
           
            case 's':
                _inputDIR=optarg;
				cout<<"inputDIR "<<_inputDIR<<endl;
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






	
	ifstream data(_inputFile.c_str());
	//ofstream out(_outputFile.c_str());
	vector<double> YawVec;
	vector<string> SplitVec;

	double yawTemp;

	long minx=0,miny=0;
	int j=0;

	filesystem::path full_path(filesystem::initial_path());

	unsigned long file_count = 0;  
	unsigned long dir_count = 0;  
	unsigned long err_count = 0;

	if ( !filesystem::exists( full_path ) )  
	{   
		std::cout << "�Ҳ��������ļ�Ŀ¼,�����Ŀ¼�Ƿ����:"  ;
		std::cout << full_path.string() << std::endl; 
		return -1;
	} 
	images.clear();
	if ( filesystem::is_directory( full_path ) )
	{
		filesystem::directory_iterator end_iter;
		for(filesystem::directory_iterator pos(_inputDIR.c_str());pos!=end_iter;++pos)
		{
			if(pos->path().extension()==".JPG"||pos->path().extension()==".jpg")
				images.push_back(pos->path().string());
			//cout<<*pos<<"   "<<pos->path().extension()<<   endl;
		}

	}

	if (data.bad())
	{
		cerr << "ERROR: could not open file: '" << _inputFile << "'!" << endl;
		return false;
	}

	while (data.good()) {

		std::getline(data, line);
		//cout<<line<<endl;
		SplitVec.clear();
		split(SplitVec, line, is_any_of(" "), token_compress_on);
		yawTemp=lexical_cast<double>(SplitVec[9]);
		//cout<<yawTemp<<","<<endl;
		YawVec.push_back(yawTemp);

		int id;
		double x,y,yaw,roll;
		id=lexical_cast<int>(SplitVec[1]);
		x=lexical_cast<double>(SplitVec[4]);
		y=lexical_cast<double>(SplitVec[5]);
		yaw=lexical_cast<double>(SplitVec[9]);
		roll=lexical_cast<double>(SplitVec[8]);
		LLtoUTM(x,y);
		PointL *p=new PointL(id,UTMEasting,UTMNorthing,yaw,roll);

		pointsL.push_back(*p);


		if(minx==0||minx>UTMEasting)
		{
			minx=UTMEasting;
		}

		if(miny==0||miny>UTMNorthing)
		{
			miny=UTMNorthing;
		}


		//Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(images[j]);
		//assert (image.get() != 0);
		//image->readMetadata();
		//Exiv2::ExifData &exifData = image->exifData();
		//exifData["Exif.Photo.UserComment"]
		//= "charset=Ascii thrown";
		
		string cmd=huginExeDir.ToStdString()+"\\exiftool -F -m -overwrite_original -GPSLongitude=\""+ lexical_cast<string>(SplitVec[4])
			+"\"  -GPSLatitude=\""+ lexical_cast<string>(SplitVec[5])
			+"\" -UserComment=\"thrown\" "
			+images[j];
		
		aExecutor.execute(new runexif(cmd,images[j]));
		//threads.add_thread(new boost::thread(boost::bind(&run,cmd)));
		//std::system(cmd.c_str());

		cout<<images[j]<<endl;
		//image->writeMetadata();
		j++;

	}
	aExecutor.wait();
	//threads.join_all();
	POINTSL::iterator i;
	PointL minPoint(0,minx,miny,0,0);
	for(i=pointsL.begin();i!=pointsL.end();++i)
	{
		(*i)=(*i)-minPoint;

		//cout<<(*i).x<<"   "<<(*i).y<<endl;
	}
	POINTSLV groups;
	double deg,heading,deg1,deg2;
//	out.clear();
	//out<<"(*i).x"<<"    "<<"(*i).y"<<"    "<<"(*i).heading(*(i+1))"<<"    "<<"deg"<<"    "<<"(*i).yaw"<<"    "<<"deg-(*i).yaw"<<endl;;
	//out<<"(*i).x"<<"    "<<"(*i).y"<<"    "<<"(*i).slope(*(i+1))"<<"    "<<"atan((*i).slope(*(i+1)))"<<"    "<<"deg"<<"    "<<"(*i).yaw"<<"    "<<"deg-(*i).yaw"<<endl;;
	i=pointsL.begin()+1;
	for(i=pointsL.begin()+1;i!=pointsL.end()-1;++i)
	{
		heading=(*(i-1)).heading(*(i));

		deg1=360-heading/PI*180;
		heading=(*(i)).heading(*(i+1));
		deg2=360-heading/PI*180;
		if((i->yaw/5-floor(i->yaw/5))>=0.5)
			i->yawint=ceil (i->yaw/5);
		else
			i->yawint=floor(i->yaw/5);
		i->dyaw=abs(deg1-deg2);
		//out<<(*i).x<<"    "<<(*i).y<<"    "<<deg1<<"    "<<deg2<<"    "<<abs(deg1-deg2)<<endl;
		//cout<<(*i).x<<"    "<<(*i).y<<"    "<<deg1<<"    "<<deg2<<"    "<<abs(deg1-deg2)<<endl;
		//cout<<(*i).id<<"     "<<deg1<<"    "<<deg2<<"    "<<abs(deg1-deg2)<<" yawint "<<i->yawint<<endl;
		//out<<(*i).id<<"     "<<deg1<<"    "<<deg2<<"    "<<abs(deg1-deg2)<<" yawint "<<i->yawint<<endl;
		//cout<<(*i).x<<"    "<<(*i).y<<"    "<<heading<<"    "<<deg<<"    "<<(*i).yaw<<"    "<<deg-(*i).yaw<<endl;
		//out<<(*i).x<<"    "<<(*i).y<<"    "<<heading<<"    "<<deg<<"    "<<(*i).yaw<<"    "<<deg-(*i).yaw<<endl;
		//system("pause");
	}

	int most1=findmost(),most2;
	if (most1>36)
		most2=most1-36;
	else
		most2=36+most1;


	for (i=pointsL.begin();i<pointsL.end();++i)
	{
		if (( (*i).yawint==most1||(*i).yawint==(most2))&&(*i).dyaw<3)
		{
			(*i).selected=true;


		}
		//out<<(*i).id<<"   yawint  "<<(*i).yawint<<"  roll  "<<(*i).roll<<" dyaw:"<<(*i).dyaw<<"  selected "<<i->selected<<endl;
		//cout<<(*i).id<<"   yawint  "<<(*i).yawint<<"  roll  "<<(*i).roll<<" dyaw:"<<(*i).dyaw<<"  selected "<<i->selected<<endl;
		//cout<<(*i).id<<"   yawint  "<<(*i).yawint<<"  roll  "<<(*i).roll<<" dyaw:"<<(*i).dyaw<<"  selected "<<i->selected<<endl;
	}

	//cout<<most1<<"     "<<most2<<endl;





	build_belt(_outputFile);


	//::system("pause");








	return 0;
}

