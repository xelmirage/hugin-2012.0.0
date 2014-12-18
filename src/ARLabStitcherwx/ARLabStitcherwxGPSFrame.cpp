#include "ARLabStitcherwxGPSFrame.h"
#include "ARLabStitcherwx.h"

ARLabStitcherwxGPSFrame::ARLabStitcherwxGPSFrame( wxWindow* parent )
:
GPSFrame( parent )
{

}
void ARLabStitcherwxGPSFrame::process()
{

}

int ARLabStitcherwxGPSFrame::getReady()
{
	std::string line;
	ifstream data(_inputFile.c_str());
	//ofstream out(_outputFile.c_str());
	vector<double> YawVec;
	vector<string> SplitVec;

	double yawTemp;

	long minx = 0, miny = 0;
	int j = 0;

	filesystem::path full_path(filesystem::initial_path());

	unsigned long file_count = 0;
	unsigned long dir_count = 0;
	unsigned long err_count = 0;

	if (!filesystem::exists(full_path))
	{
		std::cout << "找不到配置文件目录,请检查该目录是否存在:";
		std::cout << full_path.string() << std::endl;
		return -1;
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
		trim(line);
		split(SplitVec, line, is_any_of(" "), token_compress_on);
		//yawTemp=lexical_cast<double>(SplitVec[8]);
		//cout<<yawTemp<<","<<endl;
		if (SplitVec.size() < 9)
			break;
		for (int i = 0; i<SplitVec.size(); ++i)
		{
			trim(SplitVec[i]);
		}

		int id;
		float x, y, yaw, roll;

		id = lexical_cast<int>(SplitVec[0]);

		x = lexical_cast<float>(SplitVec[3]);
		y = lexical_cast<float>(SplitVec[4]);
		yaw = lexical_cast<float>(SplitVec[8]);
		roll = lexical_cast<float>(SplitVec[7]);
		LLtoUTM(x, y);
		PointL *p = new PointL(id, UTMEasting, UTMNorthing, yaw, roll);

		pointsL.push_back(*p);
		YawVec.push_back(yaw);

		if (minx == 0 || minx>UTMEasting)
		{
			minx = UTMEasting;
		}

		if (miny == 0 || miny > UTMNorthing)
		{
			miny = UTMNorthing;
		}

		j++;

	}

	POINTSL::iterator i;
	PointL minPoint(0, minx, miny, 0, 0);
	for (i = pointsL.begin(); i != pointsL.end(); ++i)
	{
		(*i) = (*i) - minPoint;

		//cout<<(*i).x<<"   "<<(*i).y<<endl;
	}

	return 0;
}
