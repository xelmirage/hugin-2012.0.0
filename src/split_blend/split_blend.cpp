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
	return 0;
}