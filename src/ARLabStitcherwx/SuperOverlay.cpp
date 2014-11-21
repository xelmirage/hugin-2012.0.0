#include "SuperOverlay.h"


SuperOverlay::SuperOverlay(wxString imagePath, wxString kmlPath, wxString outPath) :
imagePath(imagePath), kmlPath(kmlPath), outPath(outPath)
{

	corners.insert(corners.end(), pointll());
	corners.insert(corners.end(), pointll());
	corners.insert(corners.end(), pointll());
	corners.insert(corners.end(), pointll());

	llFactor = 1000000000000000;
	lod = 256;
	blkSize = wxSize(256, 256);
}


SuperOverlay::~SuperOverlay()
{
}


bool SuperOverlay::build()
{
	int i,j;
	wxImage baseImage;
	baseImage.AddHandler(new wxTIFFHandler);
	baseImage.LoadFile(imagePath.GetFullPath(), wxBITMAP_TYPE_TIFF);
	if (!baseImage.IsOk())
	{
		return false;
	}
	int longSide;
	longSide = max(baseImage.GetHeight(), baseImage.GetWidth());

	readKml(kmlPath.GetFullPath());

	for (i = 0; i < corners.size(); ++i)
	{
		if (i == 0)
		{
			north = corners[0];
			south = corners[0];
			east = corners[0];
			west = corners[0];
		}
		else
		{
			if (corners[i].latitude > north.latitude) north = corners[i];
			if (corners[i].latitude < south.latitude) south = corners[i];
			if (corners[i].longitude > east.longitude) east = corners[i];
			if (corners[i].longitude < west.longitude) west = corners[i];
		}

	}
	theta = atan((north.longitude - west.longitude) / (north.latitude - west.latitude));
	



	//int northing0, northing3, easting0, easting3;
	//LLtoUTM(corners[0].getLongitude(), corners[0].getLatitude());
	//northing0 = UTMNorthing;
	//easting0 = UTMEasting;
	//LLtoUTM(corners[3].getLongitude(), corners[3].getLatitude());
	//northing3 = UTMNorthing;
	//easting3 = UTMEasting;

	double dx = corners[3].getLongitude() - corners[0].getLongitude();
	double dy = corners[3].getLatitude() - corners[0].getLatitude();
	double deg;
	if (dx >= 0)
	{
		if (dy >= 0)
		{
			double rad = atan((double)dx / (double)dy);
			deg = 360-rad * 180 / PI;
		}
		else
		{
			double rad = atan((double)dx / (double)dy);
			deg = 180-rad * 180 / PI+180;
		}
	}
	else
	{
		if (dy <= 0)
		{
			double rad = atan((double)dx / (double)dy);
			deg = 180-rad * 180 / PI ;
		}
		else
		{
			double rad = atan((double)dx / (double)dy);
			deg = -rad * 180 / PI ;
		}
	}

	rotate = deg;

	vector<vector<vector<pyramidNode>>> pyramid;
	wxDir outdir(outPath);
	int pyramidNo= 0;
	wxSize lastBlocks;
	wxSize baseImgSize;
	baseImgSize.x = baseImage.GetWidth();
	baseImgSize.y = baseImage.GetHeight();
	wxSize blocks = calc_split(baseImgSize, blkSize);

	while ((blocks.GetWidth()>1) || (blocks.GetHeight()>1))
	{
		
		baseImgSize.x = baseImage.GetWidth();
		baseImgSize.y = baseImage.GetHeight();

		blocks = calc_split(baseImgSize, blkSize);
		wxImage block;
		wxFileName outBlockName = wxFileName::DirName(outdir.GetName());
		wxString blockName;

		block.AddHandler(new wxTIFFHandler);
		calc_reg(baseImage.GetWidth(), baseImage.GetHeight());
		vector<vector<pyramidNode>> col;
		pyramidNode pnode;
		for (i = 0; i < blocks.GetWidth(); ++i)
		{
			vector<pyramidNode> row;
			
			for (j = 0; j < blocks.GetHeight(); ++j)
			{

				wxPoint p(i*blkSize.GetX(), j*blkSize.GetY());
				wxSize actrualSize = blkSize;
				if ((actrualSize.GetWidth() + p.x) > baseImgSize.GetWidth())
				{
					actrualSize.SetWidth(baseImgSize.GetWidth() - p.x);
				}
				if ((actrualSize.GetHeight() + p.y) > baseImgSize.GetHeight())
				{
					actrualSize.SetHeight(baseImgSize.GetHeight() - p.y);
				}


				block = baseImage.Size(actrualSize, -p, 50, 50, 50);


				blockName.Clear();
				blockName << imagePath.GetName() << "_Pyramid_"<<pyramidNo<<"_block_" << j << "_" << i;
				outBlockName.SetName(blockName);
				outBlockName.SetExt("tif");
				block.SaveFile(outBlockName.GetFullPath());

				
				double cornerLon, cornerLat;

				multiReg(p.x, p.y + actrualSize.GetHeight(), cornerLon, cornerLat);
				pnode.corners[0].setLongitude(cornerLon);
				pnode.corners[0].setLatitude(cornerLat);

				multiReg(p.x + actrualSize.GetWidth(), p.y + actrualSize.GetHeight(), cornerLon, cornerLat);
				pnode.corners[1].setLongitude(cornerLon);
				pnode.corners[1].setLatitude(cornerLat);

				multiReg(p.x + actrualSize.GetWidth(), p.y, cornerLon, cornerLat);
				pnode.corners[2].setLongitude(cornerLon);
				pnode.corners[2].setLatitude(cornerLat);


				multiReg(p.x, p.y, cornerLon, cornerLat);
				pnode.corners[3].setLongitude(cornerLon);
				pnode.corners[3].setLatitude(cornerLat);







				pnode.maxLod = -1;
				pnode.minLod = 128;
				pnode.rotation = rotate;
				pnode.image = outBlockName;
				pnode.kml = outBlockName;
				pnode.kml.SetExt("kml");
				pnode.networkNodes.clear();
				if (pyramidNo != 0)
				{
					pnode.networkNodes.push_back(pyramid.back()[i * 2][j * 2]);
					if ((j * 2+1) < lastBlocks.GetHeight())
					{
						pnode.networkNodes.push_back(pyramid.back()[i * 2][j * 2+1]);
						
					}
					if ((i * 2+1) < lastBlocks.GetWidth())
					{
						pnode.networkNodes.push_back(pyramid.back()[i * 2 + 1][j * 2]);
					}
					if (((j * 2+1) < lastBlocks.GetHeight()) && ((i * 2+1) < lastBlocks.GetWidth()))
					{
						pnode.networkNodes.push_back(pyramid.back()[i * 2 + 1][j * 2+1]);
					}

				}



				pnode.writeKml();
				row.push_back(pnode);


			}
			col.push_back(row);
		}
		pyramid.push_back(col);
		++pyramidNo;
		lastBlocks = blocks;
		baseImage.Rescale(baseImgSize.x / 2, baseImgSize.y / 2);
	}

	return true;
}

bool SuperOverlay::readKml(wxString kmlFileName)
{
	wxString stringCoordinates;
	wxXmlDocument doc;
	if (!doc.Load(kmlFileName))
		return false;

	if (doc.GetRoot()->GetName() != "kml")
		return false;

	wxXmlNode *child = doc.GetRoot()->GetChildren();
	if (child == NULL||child->GetName()!="Folder")
	{
		return false;
	}
	child = child->GetChildren();
	if (child == NULL || child->GetName() != "name")
	{
		return false;
	}
	else
	{
		while (child) 
		{
			if (child->GetName() == "GroundOverlay")
			{
				wxXmlNode *ovlchild = child->GetChildren();
				if (child == NULL )
				{
					return false;
				}
				while (ovlchild)
				{
					if (ovlchild->GetName() == "gx:LatLonQuad")
					{
						if (ovlchild->GetChildren()->GetName() == "coordinates")
						{
							stringCoordinates = ovlchild->GetChildren()->GetNodeContent();
							wxArrayString arrayCoordinates;
							stringCoordinates.Trim();
							arrayCoordinates=wxSplit(stringCoordinates, ' ');
							for (int i=0; i < arrayCoordinates.size(); ++i)
							{
								if (i >= corners.size())
								{
									break;
								}
								if (arrayCoordinates[i].size() > 6)
								{
									arrayCoordinates[i].Trim();
									wxArrayString lls = wxSplit(arrayCoordinates[i], ',');
									double x, y;
									try
									{
										x = boost::lexical_cast<double>(lls[0]);
										y = boost::lexical_cast<double>(lls[1]);
										corners[i].setLatitude(y);
										corners[i].setLongitude(x);
									}
									catch (boost::bad_lexical_cast& e)
									{
										wxMessageBox(e.what());
										return false;
									}
								}
								//wxArrayString arrayll = wxSplit(arrayCoordinates[i], ',');

							}
	
						}
					}
					ovlchild = ovlchild->GetNext();
				}
			}
			child=child->GetNext();
		}
		
	}
	
	

	return true;
}


wxSize SuperOverlay::calc_split(wxSize imageSize, wxSize blockSize)
{
	wxSize blocks;

	blocks.SetWidth(ceil((double)imageSize.GetWidth() / (double)blockSize.GetWidth()));
	blocks.SetHeight(ceil((double)imageSize.GetHeight() / (double)blockSize.GetHeight()));


	return blocks;
}


bool SuperOverlay::writeKML(int pyNo, int i, int j, wxString blkName)
{
	wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("kml"));
	root->AddAttribute("xmlns", "http://earth.google.com/kml/2.2");

	return true;
}
void SuperOverlay::calc_reg(int width,int height)
{
	int i;
	vector<vector<double>> mat;
	vector<double> row0;
	vector<double> row1;
	vector<double> row2;
	vector<double> row3;
	double x, y;
	mat.clear();


	x = 0;
	y = height;
	row3.push_back(x);
	row3.push_back(y);
	row3.push_back(corners[0].getLongitude());
	row3.push_back(corners[0].getLatitude());
	mat.push_back(row3);

	x = width;
	y = height;
	row2.push_back(x);
	row2.push_back(y);
	row2.push_back(corners[1].getLongitude());
	row2.push_back(corners[1].getLatitude());
	mat.push_back(row2);

	x = width;
	y = 0;
	row1.push_back(x);
	row1.push_back(y);
	row1.push_back(corners[2].getLongitude());
	row1.push_back(corners[2].getLatitude());
	mat.push_back(row1);

	x = 0;
	y = 0;
	row0.push_back(x);
	row0.push_back(y);
	row0.push_back(corners[3].getLongitude());
	row0.push_back(corners[3].getLatitude());
	mat.push_back(row0);

	

	

	

	double *_data;
	_data = (double*)malloc(3 * mat.size()*sizeof(double));
	double *answer = (double*)malloc(3 * (sizeof(double)));
	double SquarePoor[4];

	for (int i = 0; i<mat.size(); ++i)
	{
		*(_data + i * 3) = mat[i][0];
		*(_data + i * 3 + 1) = mat[i][1];
		*(_data + i * 3 + 2) = mat[i][2];

	}
	
	if (MultipleRegression(_data, mat.size(), 3, answer, SquarePoor) == 0)
	{
		xb = answer[0];
		xa1 = answer[1];
		xa2 = answer[2];
	}
	else
	{

	}

	for (int i = 0; i<mat.size(); ++i)
	{
		*(_data + i * 3) = mat[i][0];
		*(_data + i * 3 + 1) = mat[i][1];
		*(_data + i * 3 + 2) = mat[i][3];

	}
	if (MultipleRegression(_data, mat.size(), 3, answer, SquarePoor) == 0)
	{
		yb = answer[0];
		ya1 = answer[1];
		ya2 = answer[2];
	}


	free(_data);
	free(answer);
}
void SuperOverlay::multiReg(int x, int y, double& lon, double& lat)
{
	double longitude = xb + xa1*x + xa2*y;
	double latitude = yb + ya1*x + ya2*y;
	

	lat = latitude;
	lon = longitude;
}
void SuperOverlay::FreeData(double **dat, double *d, int count)
{
	int i, j;
	free(d);
	for (i = 0; i < count; i++)
		free(dat[i]);
	free(dat);
}
int SuperOverlay::LinearEquations(double *data, int count, double *Answer)
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
int SuperOverlay::MultipleRegression(double *data, int rows, int cols, double *Answer, double *SquarePoor)
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

void SuperOverlay::LLtoUTM(double Long, double Lat) {
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