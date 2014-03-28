#include"split_blend.h"







SplitBlendFrame::SplitBlendFrame(wxWindow * parent, const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(parent, -1, title, pos, size), m_isStitching(false)
{
	wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
	m_stitchPanel = new SplitBlendPanel(this);

	topsizer->Add(m_stitchPanel, 1, wxEXPAND | wxALL, 2);

	topsizer->Add( new wxButton(this, wxID_CANCEL, _("Cancel")),
		0, wxALL | wxALIGN_RIGHT, 10);

#ifdef __WXMSW__
	// wxFrame does have a strange background color on Windows..
	this->SetBackgroundColour(m_stitchPanel->GetBackgroundColour());
#endif

	SetSizer( topsizer );
	    topsizer->SetSizeHints( this );   // set size hints to honour minimum size
	m_deleteOnExit=false;
	m_isFinished=false;

}

void SplitBlendFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox( wxString::Format(_("ARLabSplitBlender. Application to stitch large hugin project files.\n Version: %s"), wxT(DISPLAY_VERSION)),
		wxT("About ARLabSplitBlender"),
		wxOK | wxICON_INFORMATION );
}
void SplitBlendFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	DEBUG_TRACE("");
	if (m_isStitching) {
		m_stitchPanel->CancelStitch();
		m_isStitching = false;
	}
	Close();
}
void SplitBlendFrame::OnCancel(wxCommandEvent& WXUNUSED(event))
{
	DEBUG_TRACE("");
	if (m_isStitching) {
		m_stitchPanel->CancelStitch();
		m_isStitching = false;
	} else {
		Close();
	}
}

void SplitBlendFrame::OnProcessTerminate(wxProcessEvent & event)
{
	if (! m_isStitching) {
		// canceled stitch
		// TODO: Cleanup files?
		
		
		Close();
	} else {
		m_isStitching = false;
		if (event.GetExitCode() != 0)
		{
			if(wxMessageBox(_("Error during stitching\nPlease report the complete text to the bug tracker on https://bugs.launchpad.net/hugin.\n\nDo you want to save the log file?"),
				_("Error during stitching"), wxICON_ERROR | wxYES_NO )==wxYES)
			{
				wxString defaultdir = wxConfigBase::Get()->Read(wxT("/actualPath"),wxT(""));
				wxFileDialog dlg(this,
					_("Specify log file"),
					defaultdir, wxT(""),
					_("Log files (*.log)|*.log|All files (*)|*"),
					wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
				dlg.SetDirectory(wxConfigBase::Get()->Read(wxT("/actualPath"),wxT("")));
				if (dlg.ShowModal() == wxID_OK)
				{
					wxConfig::Get()->Write(wxT("/actualPath"), dlg.GetDirectory());  // remember for later
					m_stitchPanel->SaveLog(dlg.GetPath());
				};

			}
			
		} else {
			if(m_deleteOnExit)
			{
				wxRemoveFile(m_scriptFile);
			};
			m_isFinished=true;
			if (this->GetParent()) {

				event.SetEventObject( this );

				this->GetParent()->GetEventHandler()->ProcessEvent( event );

				Close();
			}
		}
	}

}
bool SplitBlendFrame::SplitBlend(wxString scriptFile, wxString outname,
	HuginBase::PanoramaMakefilelibExport::PTPrograms progs, bool doDeleteOnExit)
{
	
	

	/*for(it=parts.begin();it!=parts.end();++it)
	{
	}*/
	








	 if (! m_stitchPanel->StitchProject(scriptFile, outname, progs)) {
        return false;
    }
    m_isStitching = true;
    m_scriptFile=scriptFile;
    m_deleteOnExit=doDeleteOnExit;
    return true;
}
void SplitBlendFrame::SetOverwrite(bool doOverwrite)
{
	 m_stitchPanel->SetOverwrite(doOverwrite);
}
split_blend_App::split_blend_App()
{
	DEBUG_TRACE("ctor");
}

split_blend_App::~split_blend_App()
{
	DEBUG_TRACE("dtor");
	DEBUG_TRACE("dtor end");
}
bool split_blend_App::OnInit()
{
	// Required to access the preferences of hugin
	SetAppName(wxT("hugin"));

#if defined __WXMSW__
	int localeID = wxConfigBase::Get()->Read(wxT("language"), (long) wxLANGUAGE_DEFAULT);
	m_locale.Init(localeID);
#else
	m_locale.Init(wxLANGUAGE_DEFAULT);
#endif
	// setup the environment for the different operating systems
#if defined __WXMSW__
	wxString huginExeDir = getExePath(argv[0]);

	wxString huginRoot;
	wxFileName::SplitPath(huginExeDir, &huginRoot, NULL, NULL);

	// locale setup
	m_locale.AddCatalogLookupPathPrefix(huginRoot + wxT("/share/locale"));

	PanoramaMakefilelibExport::PTPrograms progs = getPTProgramsConfig(huginExeDir, wxConfigBase::Get());
	progs.enblend=getProgram(wxConfigBase::Get(),huginExeDir,wxT("multiblend_x64"),wxT("enblend"));
#else
	// add the locale directory specified during configure
	m_locale.AddCatalogLookupPathPrefix(wxT(INSTALL_LOCALE_DIR));
	PTPrograms progs = getPTProgramsConfig(wxT(""), wxConfigBase::Get());
#endif
#if defined __WXMAC__ && defined MAC_SELF_CONTAINED_BUNDLE
	{
		wxString thePath = MacGetPathToBundledResourceFile(CFSTR("locale"));
		if(thePath != wxT(""))
			m_locale.AddCatalogLookupPathPrefix(thePath);
		else {
			wxMessageBox(_("Translations not found in bundle"), _("Fatal Error"));
			return false;
		}
	}
#endif

	static const wxCmdLineEntryDesc cmdLineDesc[] =
	{
		//On wxWidgets 2.9, wide characters don't work here.
		//On previous versions, the wxT macro is required for unicode builds.
#if wxCHECK_VERSION(2,9,0)
		{ wxCMD_LINE_SWITCH, "h", "help", "show this help message",
		wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
		{ wxCMD_LINE_OPTION, "o", "output",  "output prefix" },
		{ wxCMD_LINE_SWITCH, "d", "delete",  "delete pto file after stitching" },
		{ wxCMD_LINE_SWITCH, "w", "overwrite", "overwrite existing files" },
		{ wxCMD_LINE_PARAM,  NULL, NULL, "<project>",
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
		{ wxCMD_LINE_NONE }
#else 
		{ wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("show this help message"),
		wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
		{ wxCMD_LINE_OPTION, wxT("o"), wxT("output"),  wxT("output prefix") },
		{ wxCMD_LINE_SWITCH, wxT("d"), wxT("delete"),  wxT("delete pto file after stitching") },
		{ wxCMD_LINE_SWITCH, wxT("w"), wxT("overwrite"), wxT("overwrite existing files") },
		{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("<project>"),
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
		{ wxCMD_LINE_NONE }
#endif 
	};
	wxCmdLineParser parser(cmdLineDesc, argc, argv);
	switch ( parser.Parse() ) {
	case -1: // -h or --help was given, and help displayed so exit
		return false;
		break;
	case 0:  // all is well
		break;
	default:
		wxLogError(_("Syntax error in parameters detected, aborting."));
		return false;
		break;
	}
	wxString scriptFile;
#ifdef __WXMAC__
	m_macFileNameToOpenOnStart = wxT("");
	wxYield();
	scriptFile = m_macFileNameToOpenOnStart;

	// bring myself front (for being called from command line)
	{
		ProcessSerialNumber selfPSN;
		OSErr err = GetCurrentProcess(&selfPSN);
		if (err == noErr)
		{
			SetFrontProcess(&selfPSN);
		}
	}
#endif

	if( parser.GetParamCount() == 0 && wxIsEmpty(scriptFile)) 
	{
		::wxMessageBox("empty input file","error");

	}
	else if(wxIsEmpty(scriptFile)) {
		scriptFile = parser.GetParam(0);
		cout << "********************* script file: " << (const char *)scriptFile.mb_str(wxConvLocal) << endl;
		if (! wxIsAbsolutePath(scriptFile)) {
			scriptFile = wxGetCwd() + wxFileName::GetPathSeparator() + scriptFile;
		}
		::wxMessageBox(scriptFile,"bingo");

	}

	wxString outname;
	if ( !parser.Found(wxT("o"), &outname) ) {
		::wxMessageBox("empty output file","error");
	}
	// check output filename
	wxFileName outfn(outname);
	wxString ext = outfn.GetExt();
	// remove extension if it indicates an image file
	if (ext.CmpNoCase(wxT("jpg")) == 0 || ext.CmpNoCase(wxT("jpeg")) == 0|| 
		ext.CmpNoCase(wxT("tif")) == 0|| ext.CmpNoCase(wxT("tiff")) == 0 || 
		ext.CmpNoCase(wxT("png")) == 0 || ext.CmpNoCase(wxT("exr")) == 0 ||
		ext.CmpNoCase(wxT("pnm")) == 0 || ext.CmpNoCase(wxT("hdr"))) 
	{
		outfn.ClearExt();
		outname = outfn.GetFullPath();
	}

	


	wxFileName basename(scriptFile);
	
	
	this->GetEvtHandlerEnabled();
	MainFrame *frame;
	frame = new MainFrame(NULL, wxT("SplitBlender"), wxDefaultPosition, wxSize(640,600) );
	
	frame->Show( true );
	frame->SetTitle("SplitBlender");
	frame->SetOverwrite(parser.Found(wxT("w")));
	frame->SetSize(wxSize(640,640));
	bool n = frame->SplitBlend(scriptFile,outname, progs, parser.Found(wxT("d")));



	
	








	
}
int split_blend_App::OnExit()
{
	SplitBlendFrame *frame;
	frame = new SplitBlendFrame(NULL, wxT("SplitBlender"), wxDefaultPosition, wxSize(640,600) );
	
	frame->Show( true );
	DEBUG_TRACE("");
	return 0;
}
void split_blend_App::OnProcessTerminate(wxProcessEvent & event)
{
	::wxMessageBox("process end","end message");
}
#ifdef __WXMAC__
// wx calls this method when the app gets "Open file" AppleEvent 
void split_blend_App::MacOpenFile(const wxString &fileName)
{
	m_macFileNameToOpenOnStart = fileName;
}
#endif
// make wxwindows use this class as the main application

IMPLEMENT_APP(split_blend_App)



	MainFrame::MainFrame(wxWindow * parent, const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(parent, -1, title, pos, size), m_isStitching(false)
{
	wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
	m_stitchPanel = new SplitBlendPanel(this);

	topsizer->Add(m_stitchPanel, 1, wxEXPAND | wxALL, 2);

	topsizer->Add( new wxButton(this, wxID_CANCEL, _("Cancel")),
		0, wxALL | wxALIGN_RIGHT, 10);

#ifdef __WXMSW__
	// wxFrame does have a strange background color on Windows..
	this->SetBackgroundColour(m_stitchPanel->GetBackgroundColour());
#endif

	SetSizer( topsizer );
	    topsizer->SetSizeHints( this );   // set size hints to honour minimum size
	m_deleteOnExit=false;
	all_set=false;
	finish_count=0;


}
void MainFrame::OnQuit(wxCommandEvent& event)
{
}
void MainFrame::OnAbout(wxCommandEvent& event)
{
}
void MainFrame::SetOverwrite(bool doOverwrite)
{
}
void MainFrame::OnProcessTerminate(wxProcessEvent & event)
{
	
	//vector<SplitBlendFrame*>::iterator it;
	bool can_start=true;
	SplitBlendFrame* frame;

	++finish_count;
	--onProcess;
	if(it!=parts.end())
	{
		frame = new SplitBlendFrame(NULL, wxT("SplitBlender"), wxDefaultPosition, wxSize(640,600) );
		stitchFrames.push_back(frame);
		frame->Show( true );
//		SetTopWindow( frame );
		frame->SetTitle(wxString::Format(_("%s - Stitching"), (*it)));
		frame->SetOverwrite(true);
		frame->SetSize(wxSize(640,640));
		frame->SetParent(this);
		bool n = frame->SplitBlend(*it,*it_out, _progs,_doDeleteOnExit);
		
		//if (!n) return n;
		//aExecutor.execute(new StitchRunnable(*it,*it_out,this,progs,doDeleteOnExit));

//		
//		
		//cmd+=*it_out;
		//int pid=m_stitchPanel->GetPid();
		//while(wxProcess::Exists(pid))
		////{
			//cout<<"processing......."<<endl;
		//}




		++it;
		++it_out;
		++onProcess;
	}



	if(finish_count==stitchFrames.size())
	{
		::wxMessageBox(cmd,"command");
		m_stitchPanel->FinalBlend(cmd);

	}
	else if(finish_count>stitchFrames.size())
	{
		Close();
	}
	//if(all_set)
	//{
	//	//
	//	for(it=stitchFrames.begin();it!=stitchFrames.end();++it)
	//	{
	//		if(!(*it)->m_isFinished)
	//		{
	//			can_start=false;
	//			return;
	//		}

	//	}
	//	if(can_start)
	//	{
	//		::wxMessageBox(cmd,"command");
	//		m_stitchPanel->FinalBlend(cmd);
	//		all_set=false;
	//	}
	//}
	
	//vector<SplitBlendPanel*>
	//m_stitchPanel->
}
void MainFrame::OnCancel(wxCommandEvent & event)
{
}
vigra::Size2D MainFrame::calc_split(vigra::Rect2D view,int num)
{
	const int maxlimit=10;
	vigra::Size2D slice_matrix;
	if (num<maxlimit)
	{
		slice_matrix.setHeight(1);
		slice_matrix.setWidth(1);
		return slice_matrix;
	}
	else{
		int NrOfSlices=num/maxlimit;


		double width=view.right()-view.left();
		double height=view.bottom()-view.top();
		slice_matrix.setHeight((int)sqrt((double)NrOfSlices)+1);
		slice_matrix.setWidth((int)sqrt((double)NrOfSlices)+1);


		cout<<"width:"<<slice_matrix.width()<<"parts, height:"<<slice_matrix.height()<<"parts"<<endl;
		double SliceRatio=(width/slice_matrix.width())/(height/slice_matrix.height());
		while(SliceRatio<0.5||SliceRatio>2)
		{
			if (SliceRatio<0.5)
			{
				slice_matrix.setWidth(slice_matrix.width()-1);
				slice_matrix.setHeight(slice_matrix.height()+2);

			}
			else if (SliceRatio>2)
			{
				slice_matrix.setWidth(slice_matrix.width()+2);
				slice_matrix.setHeight(slice_matrix.height()-1);
			}
			SliceRatio=(width/slice_matrix.width())/(height/slice_matrix.height());
			cout<<"width:"<<slice_matrix.width()<<"parts, height:"<<slice_matrix.height()<<"parts"<<endl;
		}
		cout<<"final---width:"<<slice_matrix.width()<<"parts, height:"<<slice_matrix.height()<<"parts"<<endl;


		return slice_matrix;
	}
}

string MainFrame::xytogps(int x,int y)
{
	double latitude=47.5722256384-0.0000017544*x-0.0000005952*y;
	double longitude= 87.8209535664 + 0.0000008831*x  -0.0000025883*y;
	string out;
	out.clear();
	out.append(::lexical_cast<string>(longitude));
	out.append(",");
	out.append(::lexical_cast<string>(latitude));
	out.append(",0 ");
	return out;

}


void MainFrame::insertNewImageToKml(string name,xmlNodePtr* unode,vigra::Point2D upperleft,vigra::Point2D lowerright)
{
	xmlNodePtr node =xmlNewNode(NULL, BAD_CAST "GroundOverlay");
	xmlAddChild(*unode,node);
	xmlNewTextChild(node, NULL, BAD_CAST "name", BAD_CAST name.c_str());
	xmlNodePtr icon =xmlNewNode(NULL, BAD_CAST "Icon");
	xmlAddChild(node,icon);
	xmlNewTextChild(icon, NULL, BAD_CAST "href", BAD_CAST name.c_str());
	xmlNewTextChild(icon, NULL, BAD_CAST "viewBoundScale", BAD_CAST "0.75");
	xmlNodePtr llq =xmlNewNode(NULL, BAD_CAST "gx:LatLonQuad");
	xmlAddChild(node,llq);
	string coords;
	coords.clear();
	string temp;
	temp.clear();
	
	
	temp=xytogps(upperleft.x,lowerright.y);
	coords+=temp;

	temp=xytogps(lowerright.x,lowerright.y);
	coords+=temp;

	temp=xytogps(lowerright.x,upperleft.y);
	coords+=temp;

	temp=xytogps(upperleft.x,upperleft.y);
	coords+=temp;

	xmlNewTextChild(llq, NULL, BAD_CAST "coordinates", BAD_CAST coords.c_str());


}



bool MainFrame::SplitBlend(wxString scriptFile, wxString outname,
	HuginBase::PanoramaMakefilelibExport::PTPrograms progs,
	bool doDeleteOnExit)
{
	_progs=progs;
	_doDeleteOnExit=doDeleteOnExit;
	wxFileName basename(scriptFile);
	
	int num;
	HuginBase::Panorama pano;
	ifstream prjfile((const char *)scriptFile.mb_str(HUGIN_CONV_FILENAME));
	if (!prjfile.good()) {
		cerr << "could not open script : " <<scriptFile << endl;
		return 1;
	}

	pano.setFilePrefix(hugin_utils::getPathPrefix(scriptFile.ToStdString()));
	DocumentData::ReadWriteError err = pano.readData(prjfile);
	if (err != DocumentData::SUCCESSFUL) {
		cerr << "error while parsing panos tool script: " << scriptFile << endl;
		cerr << "DocumentData::ReadWriteError code: " << err << endl;
		return 1;
	}
	PanoramaOptions opt = pano.getOptions();
	vigra::Rect2D roi=opt.getROI();
	vigra::Size2D size=opt.getSize();

	num=::getImagesinROI(pano,pano.getActiveImages()).size();
	int active=pano.getActiveImages().size();


	//::wxMessageBox("crop to "+ lexical_cast<string>(roi.left()) +"," + lexical_cast<string>( roi.top()) + "," +  lexical_cast<string>(roi.right()) + "," +  lexical_cast<string>(roi.bottom()) ,
	//"size:"+ lexical_cast<string>(roi.right()-roi.left())+"x"+ lexical_cast<string>(roi.bottom()-roi.top())+"In roi:"+ lexical_cast<string>(num));
	cout << "crop to " << roi.left() << "," << roi.top() << "," << roi.right() << "," << roi.bottom() << endl;
	cout<<"size:"<<roi.right()-roi.left()<<"x"<<roi.bottom()-roi.top()<<endl;


	vigra::Size2D slices;
	slices=calc_split(roi,num);

	int slice_height=(roi.bottom()-roi.top())/slices.height();
	int slice_width=(roi.right()-roi.left())/slices.width();
	//string kmlname=scriptFile.substr(0,scriptFile.length()-4).append(".kml");
	//std::ofstream kml(kmlname);


	xmlDocPtr doc=xmlNewDoc(BAD_CAST"1.0");

	xmlNodePtr kml=xmlNewNode(NULL,BAD_CAST"kml");
	xmlDocSetRootElement(doc,kml);

	xmlNewProp(kml,BAD_CAST"xmlns",BAD_CAST "http://www.opengis.net/kml/2.2");
	xmlNewProp(kml,BAD_CAST"xmlns:gx",BAD_CAST "http://www.google.com/kml/ext/2.2");
	xmlNewProp(kml,BAD_CAST"xmlns:kml",BAD_CAST "http://www.opengis.net/kml/2.2" );
	xmlNewProp(kml,BAD_CAST"xmlns:atom",BAD_CAST "http://www.w3.org/2005/Atom");


	
	xmlNodePtr Folder = xmlNewNode(NULL,BAD_CAST"Folder");
	xmlAddChild(kml,Folder);
	

	xmlNewTextChild(Folder, NULL, BAD_CAST "name", BAD_CAST "uav");  
    xmlNewTextChild(Folder, NULL, BAD_CAST "open", BAD_CAST "1");  
	
	int top,bottom,left,right;
	for(int i=0;i<slices.width();++i)
	{
		for(int j=0;j<slices.height();++j)
		{
			left=roi.left()+i*slice_width;
			right=roi.left()+(i+1)*slice_width+10;
			if (right>roi.right())
			{
				right=roi.right();
			}

			top=roi.top()+j*slice_height;
			bottom=roi.top()+(j+1)*slice_height+10;
			if(bottom>roi.bottom())
			{
				bottom=roi.bottom();
			}
			cout<<"--------------"<<i<<"---"<<j<<"------------"<<endl;
			cout << "crop to " << left << "," << right << "," << top << "," << bottom << endl;
			cout<<"size:"<<right-left<<"x"<<bottom-top<<endl;
			HuginBase::Panorama temp_pano=pano;
			PanoramaOptions temp_opt = temp_pano.getOptions();
			vigra::Rect2D  temp_roi=opt.getROI();

			vigra::Point2D upperleft,lowerright;
			upperleft.x=left;
			upperleft.y=top;
			lowerright.x=right;
			lowerright.y=bottom;
			temp_roi.setUpperLeft(upperleft);
			temp_roi.setLowerRight(lowerright);

			temp_opt.setROI(temp_roi);

			temp_pano.setOptions(temp_opt);
			temp_pano=temp_pano.getSubset(::getImagesinROI(temp_pano,temp_pano.getActiveImages()));
			string out_suffix=lexical_cast<string>(i)+"_"+lexical_cast<string>(j);
			string output=scriptFile.substr(0,scriptFile.length()-4).append("_dist_"+out_suffix+".pto");
			
			ofstream of(output.c_str());

			temp_pano.writeData(of);
			parts.push_back(output);


			boost::filesystem::path outpath(outname);
			boost::filesystem::path oname=(output+".tif");
			string outpart=outpath.branch_path().string()+"/"+oname.filename().string();

			outparts.push_back(outpart);
			//outparts.push_back(output+".tif");
			cout<<"j finish,out to "<<output<<endl;

			

			insertNewImageToKml(oname.string()+".tif",
				&Folder,upperleft,lowerright);





		}

		cout<<"i finish"<<endl;
	}

	
	
	cmd=progs.enblend+progs.enblend_opts+" -o "+outname+".tif ";
	
	int nRel = xmlSaveFile(outname+".kml ",doc);

	 xmlFreeDoc(doc);



	SplitBlendFrame* frame;

	for(it=outparts.begin();it!=outparts.end();++it)
	{
		cmd+=*it+".tif ";
	}
	
	it=parts.begin();
	it_out=outparts.begin();
	/*frame = new SplitBlendFrame(NULL, wxT("SplitBlender"), wxDefaultPosition, wxSize(640,600) );
	frame->Show( true );

	frame->SetTitle("SplitBlend MainFrame");
	frame->SetOverwrite(doDeleteOnExit);
	frame->SetSize(wxSize(320,240));
	frame->SetParent(this);
	bool n = frame->SplitBlend(*it,*it_out, progs, doDeleteOnExit);*/
	int _cores=getCPUCount();


	//PoolExecutor aExecutor(_cores);
	onProcess=0;

	for(it=parts.begin();it!=parts.end();++it)
	{
		//::wxMessageBox(*it+"\n"+*it_out,"output");
		frame = new SplitBlendFrame(NULL, wxT("SplitBlender"), wxDefaultPosition, wxSize(640,600) );
		stitchFrames.push_back(frame);
		frame->Show( true );
//		SetTopWindow( frame );
		frame->SetTitle(wxString::Format(_("%s - Stitching"), (*it)));
		frame->SetOverwrite(true);
		frame->SetSize(wxSize(640,640));
		frame->SetParent(this);
		bool n = frame->SplitBlend(*it,*it_out, progs,doDeleteOnExit);
		
		if (!n) return n;
		//aExecutor.execute(new StitchRunnable(*it,*it_out,this,progs,doDeleteOnExit));

//		
//		
		//cmd+=*it_out;
		//int pid=m_stitchPanel->GetPid();
		//while(wxProcess::Exists(pid))
		////{
			//cout<<"processing......."<<endl;
		//}





		++it_out;
		++onProcess;
		if(onProcess>=_cores)
		{
			++it;
			break;
		}
	}
	all_set=true;
	//aExecutor.wait();


}

