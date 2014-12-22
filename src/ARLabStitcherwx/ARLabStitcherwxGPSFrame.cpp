#include "ARLabStitcherwxGPSFrame.h"
#include "ARLabStitcherwx.h"

ARLabStitcherwxGPSFrame::ARLabStitcherwxGPSFrame( wxWindow* parent )
:
GPSFrame( parent )
{
	isReady = false;
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
	maxx = maxy = 0;
	int j = 0;

	

	unsigned long file_count = 0;
	unsigned long dir_count = 0;
	unsigned long err_count = 0;

	
	
	

	if (data.bad())
	{
		cerr << "ERROR: could not open file: '" << _inputFile << "'!" << endl;
		return -1;
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
		PointL p = PointL(id, UTMEasting, UTMNorthing, yaw, roll);

		pointsL.push_back(p);
		YawVec.push_back(yaw);

		if (minx == 0 || minx>UTMEasting)
		{
			minx = UTMEasting;
		}

		if (miny == 0 || miny > UTMNorthing)
		{
			miny = UTMNorthing;
		}
		if (maxx<UTMEasting)
		{
			maxx = UTMEasting;
		}
		if (maxy<UTMNorthing)
		{
			maxy = UTMNorthing;
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
	maxx -= minx;
	maxy -= miny;
	data.close();


	data.open(_GPSResult.c_str());
	if (data.bad())
	{
		cerr << "ERROR: could not open file: '" << _inputFile << "'!" << endl;
		return -1;
	}
	while (data.good())
	{
		std::getline(data, line);
		SplitVec.clear();
		trim(line);
		split(SplitVec, line, is_any_of(" "), token_compress_on);

		if (SplitVec.size() < 2)
			break;

		for (int i = 0; i < SplitVec.size(); ++i)
		{
			trim(SplitVec[i]);
		}
		if (SplitVec[0]=="selected")
		{
			int ID = lexical_cast<int>(SplitVec[1]);
			pointsL[ID].selected = true;
			if (SplitVec.size()>3)
			{
				vector<std::string> pairs;

				split(pairs, SplitVec[3], is_any_of(","), token_compress_on);
				for (int i = 0; i < pairs.size(); ++i)
				{
					Line l;
					l.a = pointsL[ID];
					l.b = pointsL[lexical_cast<int>(pairs[i])];
					lines.push_back(l);
				}

			}
			
		}


	}


	isReady = true;
	return 0;
}
void ARLabStitcherwxGPSFrame::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	dc.SetPen(*wxRED_PEN);
	
	wxSize windowSize = dc.GetSize();
	double ratio_x, ratio_y;
	double ratio_use;
	

	if (isReady)
	{
		ratio_x = (double)windowSize.x / (double)(maxx);
		ratio_y = (double)windowSize.y / (double)(maxy);

		if (ratio_x > ratio_y)
		{
			ratio_use = ratio_x;
		}
		else
		{
			ratio_use = ratio_y;
		}
		for (int i = 0; i < pointsL.size(); ++i)
		{
			dc.DrawCircle(pointsL[i].x*ratio_use, pointsL[i].y*ratio_use, 5);
		}

	}

}