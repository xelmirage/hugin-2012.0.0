#include <hugin_config.h>
#include <hugin_version.h>
#include "panoinc_WX.h"
#include "panoinc.h"


#include <fstream>
#include <sstream>
#ifdef WIN32
 #include <getopt.h>
#else
 #include <unistd.h>
#endif

#include <hugin_basic.h>
#include <hugin_utils/stl_utils.h>
#include "PT/Panorama.h"
#include "PT/utils.h"
#include<boost/algorithm/string.hpp>
#include<boost/lexical_cast.hpp>
#include<hugin1/base_wx/huginConfig.cpp>

#include "base_wx/platform.h"
#include "base_wx/wxPlatform.h"
#include "SplitBlendPanel.h"

//#include "zthread/Runnable.h"
//#include "zthread/PoolExecutor.h"
//#include "Utils.h"
//#include<boost/lexical_cast.hpp>
// somewhere SetDesc gets defined.. this breaks wx/cmdline.h on OSX
#ifdef SetDesc
#undef SetDesc
#endif

#include <wx/cmdline.h>

#include <libxml/parser.h>

#include <libxml/tree.h>

using namespace std;
using namespace hugin_utils;
using namespace HuginBase;
using namespace AppBase;
using namespace boost;
//using namespace ZThread;



class SplitBlendFrame: public wxFrame
{
public:
    SplitBlendFrame(wxWindow * parent, const wxString& title, const wxPoint& pos, const wxSize& size);

    bool SplitBlend(wxString scriptFile, wxString outname,
                       HuginBase::PanoramaMakefilelibExport::PTPrograms progs,
                       bool doDeleteOnExit);
	
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    /** sets, if existing output file should be automatic overwritten */
    void SetOverwrite(bool doOverwrite);
	bool m_isStitching;
	bool m_isFinished;
	
	
private:
	vector<SplitBlendPanel*> stitchPanels;
	SplitBlendPanel* m_stitchPanel;
    
    wxString m_scriptFile;
    bool m_deleteOnExit;

    void OnProcessTerminate(wxProcessEvent & event);
    void OnCancel(wxCommandEvent & event);
	
   // SplitBlendPanel * m_stitchPanel;

    DECLARE_EVENT_TABLE()
};

enum
{
    ID_Quit = 1,
    ID_About   
};

BEGIN_EVENT_TABLE(SplitBlendFrame, wxFrame)
    EVT_MENU(ID_Quit,  SplitBlendFrame::OnQuit)
    EVT_MENU(ID_About, SplitBlendFrame::OnAbout)
    EVT_BUTTON(wxID_CANCEL, SplitBlendFrame::OnCancel)
    EVT_END_PROCESS(-1, SplitBlendFrame::OnProcessTerminate)
END_EVENT_TABLE()
class split_blend_App : public wxApp
{
	public:

    /** ctor.
     */
    split_blend_App();

    /** dtor.
     */
    virtual ~split_blend_App();

    /** pseudo constructor. with the ability to fail gracefully.
     */
    virtual bool OnInit();

    /** just for testing purposes */
    virtual int OnExit();
    
#ifdef __WXMAC__
    /** the wx calls this method when the app gets "Open file" AppleEvent */
    void MacOpenFile(const wxString &fileName);
#endif

private:
	vector<SplitBlendFrame*> frames;
    wxLocale m_locale;
	void split_blend_App::OnProcessTerminate(wxProcessEvent & event);
#ifdef __WXMAC__
    wxString m_macFileNameToOpenOnStart;
#endif
	DECLARE_EVENT_TABLE();

};


BEGIN_EVENT_TABLE(split_blend_App, wxApp)
    EVT_END_PROCESS(-1, split_blend_App::OnProcessTerminate)
END_EVENT_TABLE()


class MainFrame: public wxFrame
{
public:
    MainFrame(wxWindow * parent, const wxString& title, const wxPoint& pos, const wxSize& size);

    bool SplitBlend(wxString scriptFile, wxString outname,
                       HuginBase::PanoramaMakefilelibExport::PTPrograms progs,
                       bool doDeleteOnExit);
	
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    /** sets, if existing output file should be automatic overwritten */
    void SetOverwrite(bool doOverwrite);
	bool m_isStitching;
	bool superOverlay;
private:
	int finish_count;
	vector<SplitBlendFrame*> stitchFrames;
	SplitBlendPanel* m_stitchPanel;
	

    wxString m_scriptFile;
    bool m_deleteOnExit;
	bool all_set;
	wxString cmd;
    void OnProcessTerminate(wxProcessEvent & event);
    void OnCancel(wxCommandEvent & event);
	vigra::Size2D MainFrame::calc_split(vigra::Rect2D view,int num);



	void insertNewImageToKml(string name,xmlNodePtr* unode,vigra::Point2D upperleft,vigra::Point2D lowerright);
		// SplitBlendPanel * m_stitchPanel;
	string xytogps(int x,int y);
	void calc_a_b(string name);
	HuginBase::PanoramaMakefilelibExport::PTPrograms _progs;
	bool _doDeleteOnExit;
	double xa1,xa2,xb,ya1,ya2,yb;
	
	int onProcess;

	vector<string> parts;
	vector<string> outparts;

	vector<string>::iterator it,it_out;

	


    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(ID_Quit,  MainFrame::OnQuit)
    EVT_MENU(ID_About, MainFrame::OnAbout)
    EVT_BUTTON(wxID_CANCEL, MainFrame::OnCancel)
    EVT_END_PROCESS(-1, MainFrame::OnProcessTerminate)
END_EVENT_TABLE()


void FreeData(double **dat, double *d, int count)
{
    int i, j;
    free(d);
    for (i = 0; i < count; i ++)
        free(dat[i]);
    free(dat);
}
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