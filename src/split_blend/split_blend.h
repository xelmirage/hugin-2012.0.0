#include <hugin_config.h>
#include <hugin_version.h>

#include <fstream>
#include <sstream>
#ifdef WIN32
 #include <getopt.h>
#else
 #include <unistd.h>
#endif

#include <hugin_basic.h>
#include <hugin_utils/stl_utils.h>

using namespace std;
using namespace hugin_utils;
using namespace HuginBase;
using namespace AppBase;