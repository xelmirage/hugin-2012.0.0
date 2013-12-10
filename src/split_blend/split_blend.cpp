#include"split_blend.h"

static void usage(const char * name)
{
    cerr << name << ": optimize image positions" << endl
         << "autooptimiser version " << DISPLAY_VERSION << endl
         << endl
         << "Usage:  " << name << " [options] input.pto" << endl
         << "   To read a project from stdio, specify - as input file." << endl
         << endl
         << "  Options:" << endl
         << "     -o file.pto  output file. If obmitted, stdout is used." << endl
         << endl
         << "    Optimisation options (if not specified, no optimisation takes place)" << std::endl
         << "     -a       auto align mode, includes various optimisation stages, depending" << endl
         << "               on the amount and distribution of the control points" << endl
         << "     -p       pairwise optimisation of yaw, pitch and roll, starting from" << endl
         << "              first image" << endl
         << "     -m       Optimise photometric parameters" << endl
         << "     -n       Optimize parameters specified in script file (like PTOptimizer)" << endl
         << endl
         << "    Postprocessing options:" << endl
         << "     -l       level horizon (works best for horizontal panos)" << endl
         << "     -s       automatically select a suitable output projection and size" << endl
         << "    Other options:" << endl
         << "     -q       quiet operation (no progress is reported)" << endl
         << "     -v HFOV  specify horizontal field of view of input images." << endl
         << "               Used if the .pto file contains invalid HFOV values" << endl
         << "               (autopano-SIFT writes .pto files with invalid HFOV)" << endl
         << endl
         << "   When using -a -l -m and -s options together, a similar operation to the \"Align\"" << endl
         << "    button in hugin is performed." << endl
         << endl;
}
int main(int argc, char *argv[])
{
	// parse arguments
    const char * optstring = "o:";
    int c;
	string output;
	while ((c = getopt (argc, argv, optstring)) != -1)
	{
		switch (c) {
		case 'o':
			output = optarg;
			break;
		default:
			abort ();
		}
	}

	if (argc - optind != 1) {
		usage(argv[0]);
		return 1;
	}

	string input=argv[optind];
	Panorama pano;
	ifstream prjfile(input.c_str());
	if (!prjfile.good()) {
		cerr << "could not open script : " << input << endl;
		return 1;
	}

	pano.setFilePrefix(hugin_utils::getPathPrefix(input));
	DocumentData::ReadWriteError err = pano.readData(prjfile);
	if (err != DocumentData::SUCCESSFUL) {
		cerr << "error while parsing panos tool script: " << input << endl;
		cerr << "DocumentData::ReadWriteError code: " << err << endl;
		return 1;
	}
	PanoramaOptions opt = pano.getOptions();
	vigra::Rect2D roi=opt.getROI();
	vigra::Size2D size=opt.getSize();
	cout << "crop to " << roi.left() << "," << roi.top() << "," << roi.right() << "," << roi.bottom() << endl;
	cout<<"size:"<<roi.right()-roi.left()<<"x"<<roi.bottom()-roi.top()<<endl;
	
	UIntSet activeImgs = pano.getActiveImages();
	for(UIntSet::iterator it=activeImgs.begin();it!=activeImgs.end();++it)
	{
		cout<<*it<<",";
	}
	cout<<endl;



	return 0;
}