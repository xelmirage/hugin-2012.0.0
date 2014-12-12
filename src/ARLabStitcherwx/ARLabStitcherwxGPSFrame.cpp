#include "ARLabStitcherwxGPSFrame.h"

ARLabStitcherwxGPSFrame::ARLabStitcherwxGPSFrame(wxWindow* parent, MyExecPanel * execpanel, wxString exeDir)
:
GPSFrame(parent), m_execPanel(execpanel), huginExeDir(exeDir)
{

}
void ARLabStitcherwxGPSFrame::process()
{
	int _cores = hugin_utils::getCPUCount();
	int c;
	int optionIndex = 0;
	std::string	line = "";
	ZThread::PoolExecutor aExecutor(_cores);


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
		m_execPanel->m_textctrl->AppendText("\n找不到配置文件目录,请检查该目录是否存在:" + full_path.string()+"\n");
		
		return;
	}

	images.clear();
	if (filesystem::is_directory(full_path))
	{
		filesystem::directory_iterator end_iter;
		for (filesystem::directory_iterator pos(_inputDIR.c_str()); pos != end_iter; ++pos)
		{
			if (pos->path().extension() == ".JPG" || pos->path().extension() == ".jpg" || pos->path().extension() == ".tif")
				images.push_back(pos->path().string());
			//cout<<*pos<<"   "<<pos->path().extension()<<   endl;
		}

	}

	if (data.bad())
	{
		cerr << "ERROR: could not open file: '" << _inputFile << "'!" << endl;
		return ;
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


		//Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(images[j]);
		//assert (image.get() != 0);
		//image->readMetadata();
		//Exiv2::ExifData &exifData = image->exifData();
		//exifData["Exif.Photo.UserComment"]
		//= "charset=Ascii thrown";

		string cmd = huginExeDir.ToStdString() + "\\exiftool -F -m -overwrite_original -GPSLongitude=\"" + lexical_cast<string>(SplitVec[3])
			+ "\"  -GPSLatitude=\"" + lexical_cast<string>(SplitVec[4])
			+ "\" -UserComment=\"thrown\" "
			+ images[j];

		aExecutor.execute(new runexif(cmd, images[j]));
		//threads.add_thread(new boost::thread(boost::bind(&run,cmd)));
		//std::system(cmd.c_str());

		cout << images[j] << endl;
		//image->writeMetadata();
		j++;

	}

	aExecutor.wait();
	//threads.join_all();
	POINTSL::iterator i;
	PointL minPoint(0, minx, miny, 0, 0);
	for (i = pointsL.begin(); i != pointsL.end(); ++i)
	{
		(*i) = (*i) - minPoint;

		//cout<<(*i).x<<"   "<<(*i).y<<endl;
	}
	POINTSLV groups;
	double deg, heading, deg1, deg2;
	//	out.clear();
	//out<<"(*i).x"<<"    "<<"(*i).y"<<"    "<<"(*i).heading(*(i+1))"<<"    "<<"deg"<<"    "<<"(*i).yaw"<<"    "<<"deg-(*i).yaw"<<endl;;
	//out<<"(*i).x"<<"    "<<"(*i).y"<<"    "<<"(*i).slope(*(i+1))"<<"    "<<"atan((*i).slope(*(i+1)))"<<"    "<<"deg"<<"    "<<"(*i).yaw"<<"    "<<"deg-(*i).yaw"<<endl;;
	i = pointsL.begin() + 1;

	int inf = 5;

	for (i = pointsL.begin() + 1; i != pointsL.end() - 1; ++i)
	{
		heading = (*(i - 1)).heading(*(i));

		deg1 = 360 - heading / PI * 180;
		heading = (*(i)).heading(*(i + 1));
		deg2 = 360 - heading / PI * 180;

		if ((i->yaw / inf - floor(i->yaw / inf)) >= 0.5)
			i->yawint = ceil(i->yaw / inf);
		else
			i->yawint = floor(i->yaw / inf);
		i->dyaw = abs(deg1 - deg2);
		//out<<(*i).x<<"    "<<(*i).y<<"    "<<deg1<<"    "<<deg2<<"    "<<abs(deg1-deg2)<<endl;
		//cout<<(*i).x<<"    "<<(*i).y<<"    "<<deg1<<"    "<<deg2<<"    "<<abs(deg1-deg2)<<endl;
		//cout<<(*i).id<<"     "<<deg1<<"    "<<deg2<<"    "<<abs(deg1-deg2)<<" yawint "<<i->yawint<<endl;
		//out<<(*i).id<<"     "<<deg1<<"    "<<deg2<<"    "<<abs(deg1-deg2)<<" yawint "<<i->yawint<<endl;
		//cout<<(*i).x<<"    "<<(*i).y<<"    "<<heading<<"    "<<deg<<"    "<<(*i).yaw<<"    "<<deg-(*i).yaw<<endl;
		//out<<(*i).x<<"    "<<(*i).y<<"    "<<heading<<"    "<<deg<<"    "<<(*i).yaw<<"    "<<deg-(*i).yaw<<endl;
		//system("pause");
	}

	int most1 = findmost(), most2;
	if (most1 > (180 / inf))
		most2 = most1 - (180 / inf);
	else
		most2 = (180 / inf) + most1;


	for (i = pointsL.begin(); i < pointsL.end(); ++i)
	{
		if (((*i).yawint == most1 || (*i).yawint == (most2)) && (*i).dyaw < 5)
		{
			(*i).selected = true;


		}
		//out<<(*i).id<<"   yawint  "<<(*i).yawint<<"  roll  "<<(*i).roll<<" dyaw:"<<(*i).dyaw<<"  selected "<<i->selected<<endl;
		//cout<<(*i).id<<"   yawint  "<<(*i).yawint<<"  roll  "<<(*i).roll<<" dyaw:"<<(*i).dyaw<<"  selected "<<i->selected<<endl;
		//cout<<(*i).id<<"   yawint  "<<(*i).yawint<<"  roll  "<<(*i).roll<<" dyaw:"<<(*i).dyaw<<"  selected "<<i->selected<<endl;
	}

	//cout<<most1<<"     "<<most2<<endl;





	build_belt(_outputFile);

	

}
int ARLabStitcherwxGPSFrame::findmost()
{
	int len = pointsL.size();
	int* data = new int[len];
	int* sum = new int[len];
	int i = 0, j = 0, datalen = 0, most = 0;
	bool findflag;
	for (i = 0; i < len; i++)
	{
		findflag = false;
		for (j = 0; j < datalen; j++)
		{
			if (data[j] == pointsL[i].yawint)
			{
				sum[j] += 1;
				findflag = true;
			}
		}
		if (!findflag)
		{
			data[j] = pointsL[i].yawint;
			datalen++;
			sum[j] = 0;
		}
	}
	for (i = 0; i<datalen; i++)
	{

		if (sum[i]>sum[most])
			most = i;
	}



	return data[most];
}
void ARLabStitcherwxGPSFrame::find_nearest(POINTSL::iterator point_position, POINTSLV::iterator belt_position)
{
	DISTS dists;
	POINTSL::iterator point_iterato;
	POINTSLV::iterator belt_iterator = belt_position + 1;

	if (belt_iterator == vector_pointsL.end())
	{
		cout << endl;
		return;
	}




	dists.clear();
	POINTSL::iterator i1;
	for (i1 = (*belt_iterator).begin(); i1 != (*belt_iterator).end(); ++i1)
	{

		long d = point_position->distance(*i1);
		dist tempDist(i1->id, d, 0);
		dists.push_back(tempDist);

	}
	sort(dists.begin(), dists.end());

	for (int ii = 0; ii<1; ii++)
	{
		cout << dists[ii].id << ",";
		//					belt_out<<dists[ii].id<<",";

		row.push_back(dists[ii]);
		//tag+=","+lexical_cast<std::string>(dists[ii].id);
		//belt_out<<","<<dists[ii].id;
	}



	for (i1 = (*belt_iterator).begin(); i1 != (*belt_iterator).end(); ++i1)
	{
		if (dists[0].id == i1->id)
		{
			find_nearest(i1, belt_iterator);
		}
	}












	//			belt_out<<endl;



}
bool ARLabStitcherwxGPSFrame::is_next_valid(int id, int step)
{
	POINTSL::iterator point_iterator;
	POINTSLV::iterator belt_iterator;
	for (belt_iterator = vector_pointsL.begin(); belt_iterator != vector_pointsL.end(); ++belt_iterator)
	{
		for (point_iterator = (*belt_iterator).begin(); point_iterator != (*belt_iterator).end(); ++point_iterator)
		{
			if ((*point_iterator).id == id)
			{
				if ((point_iterator + step)<(*belt_iterator).end())
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
void ARLabStitcherwxGPSFrame::build_belt(string outfile)
{
	int _cores = hugin_utils::getCPUCount();
	boost::thread_group threads;
	POINTSLV::iterator belt_iterator;
	int row_step = 1;    //选取照片的间隔
	DISTS dists;
	dists.clear();
	string tag;
	tag.clear();
	bool valid;
	string _outputFile = outfile;
	ofstream belt_out(_outputFile.c_str());
	POINTSL::iterator i;
	ZThread::PoolExecutor aExecutor(_cores);
	for (i = pointsL.begin(); i != pointsL.end() - 1; ++i)
	{
		if (!i->selected)
		{
			if ((i + 1)->selected)
			{
				POINTSL pl;
				vector_pointsL.push_back(pl);
			}
			else
			{
			}
		}
		else
		{
			(vector_pointsL.end() - 1)->push_back((*i));


		}


	}
	POINTSLV::iterator j;
	for (j = vector_pointsL.begin(); j != vector_pointsL.end();)
	{
		//cout<<j->size()<<endl;
		//剔除照片数小于10的航带
		if (j->size()<10)
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


	for (j = vector_pointsL.begin(); j != vector_pointsL.begin() + 1; ++j)
	{


		for (i = (*j).begin(); i<(*j).end() - 1; i += row_step)
		{
			dists.clear();

			int img_id;
			img_id = lexical_cast<int>(i->id);



			//belt_out<<images[img_id-1]<<endl;

			cout << i->id << ",";



			//find_nearest(i,j);
			//belt_out<<images[img_id-1]<<endl;

			row.clear();
			row.push_back(dist(i->id, 0, 0));
			for (belt_iterator = j + 1; belt_iterator != vector_pointsL.end(); ++belt_iterator)
			{
				dists.clear();
				POINTSL::iterator i1;
				for (i1 = (*belt_iterator).begin(); i1 != (*belt_iterator).end(); ++i1)
				{

					long d = i->distance(*i1);
					dist tempDist(i1->id, d, 0);
					dists.push_back(tempDist);

				}
				sort(dists.begin(), dists.end());
				for (int ii = 0; ii<1; ii++)
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
			for (row_iterator = row.begin(); row_iterator != row.end(); ++row_iterator)
			{
				cout << row_iterator->id << ",";
				//belt_out<<row_iterator->id<<",";
				tag.clear();
				tag = "selected ";
				tag += lexical_cast<std::string>(row_iterator->id);
				tag += " pairs_with ";
				valid = is_next_valid(row_iterator->id, row_step);
				if (valid)
				{
					tag += lexical_cast<std::string>(row_iterator->id + row_step);
				}
				if (row_iterator != row.end() - 1)
				{
					if (valid)
					{
						tag += "," + lexical_cast<std::string>((row_iterator + 1)->id);
					}
					else
					{
						tag += lexical_cast<std::string>((row_iterator + 1)->id);

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



				string cmd = "exiftool -F -m -overwrite_original -UserComment=\"" + tag + "\" "
					+ images[(row_iterator->id - 1)];
				//threads.add_thread(new boost::thread(boost::bind(&run,cmd)));
				//std::system(cmd.c_str());

				aExecutor.execute(new runexif(cmd, images[(row_iterator->id - 1)]));

			}
			cout << endl;
			//belt_out<<images[i->id-1]<<" ";                          //重要：从images[]数组调用时要将id-1，因为排id的时候没有0


			for (row_iterator = row.begin(); row_iterator != row.end(); ++row_iterator)
			{
				cout << images[(*row_iterator).id - 1] << endl;
				belt_out << images[(*row_iterator).id - 1] << " ";

			}
			cout << endl;


			aExecutor.wait();

			//belt_out<<endl;




			//threads.join_all();


		}


	}



	cout << endl << endl << "Filtered successfully    ;-)" << endl;


}