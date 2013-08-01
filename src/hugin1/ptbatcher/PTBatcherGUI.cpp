// -*- c-basic-offset: 4 -*-

/** @file PTBatcherGUI.cpp
 *
 *  @brief Batch processor for Hugin with GUI
 *
 *  @author Marko Kuder <marko.kuder@gmail.com>
 *
 *  $Id: PTBatcherGUI.cpp 3322 2008-08-16 5:00:07Z mkuder $
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "PTBatcherGUI.h"
#ifdef __WXMSW__
#include "wx/cshelp.h"
#endif
#include "lensdb/LensDB.h"

// make wxwindows use this class as the main application
IMPLEMENT_APP(PTBatcherGUI)

BEGIN_EVENT_TABLE(PTBatcherGUI, wxApp)
    EVT_LIST_ITEM_ACTIVATED(XRCID("project_listbox"),PTBatcherGUI::OnItemActivated)
    EVT_KEY_DOWN(PTBatcherGUI::OnKeyDown)
END_EVENT_TABLE()

bool PTBatcherGUI::OnInit()
{
    // Required to access the preferences of hugin
    SetAppName(wxT("hugin"));

#if defined __WXMSW__
    int localeID = wxConfigBase::Get()->Read(wxT("language"), (long) wxLANGUAGE_DEFAULT);
    m_locale.Init(localeID);
    // initialize help provider
    wxHelpControllerHelpProvider* provider = new wxHelpControllerHelpProvider;
    wxHelpProvider::Set(provider);

#else
    m_locale.Init(wxLANGUAGE_DEFAULT);
#endif

    // setup the environment for the different operating systems
#if defined __WXMSW__
    wxString huginExeDir = getExePath(argv[0]);

    wxString huginRoot;
    wxFileName::SplitPath(huginExeDir, &huginRoot, NULL, NULL);
    m_xrcPrefix = wxString(huginRoot + wxT("/share/hugin/xrc/"));
    // lensfun database init
    wxString lensfunDBPath=huginRoot + wxT("/share/lensfun");
    HuginBase::LensDB::LensDB::GetSingleton().SetMainDBPath(std::string(lensfunDBPath.mb_str(HUGIN_CONV_FILENAME)));

    // locale setup
    m_locale.AddCatalogLookupPathPrefix(huginRoot + wxT("/share/locale"));

#elif defined __WXMAC__ && defined MAC_SELF_CONTAINED_BUNDLE
    {
        wxString exec_path = MacGetPathToBundledResourceFile(CFSTR("xrc"));
        if(exec_path != wxT(""))
        {
            m_xrcPrefix = exec_path + wxT("/");
        }
        else
        {
            wxMessageBox(_("xrc directory not found in bundle"), _("Fatal Error"));
            return false;
        }

        wxString thePath = MacGetPathToBundledResourceFile(CFSTR("locale"));
        if(thePath != wxT(""))
        {
            m_locale.AddCatalogLookupPathPrefix(thePath);
        }
        else
        {
            wxMessageBox(_("Translations not found in bundle"), _("Fatal Error"));
            return false;
        }
        thePath = MacGetPathToBundledResourceFile(CFSTR("lensfun"));
        if (thePath == wxT("")) {
            wxMessageBox(_("lensfun directory not found in bundle"),
                            _("Fatal Error"));
            return false;
        }
        HuginBase::LensDB::LensDB::GetSingleton().SetMainDBPath(std::string(thePath.mb_str(HUGIN_CONV_FILENAME)));
    }
#else
    // add the locale directory specified during configure
    m_xrcPrefix = wxT(INSTALL_XRC_DIR);
    m_locale.AddCatalogLookupPathPrefix(wxT(INSTALL_LOCALE_DIR));
#endif

    // set the name of locale recource to look for
    m_locale.AddCatalog(wxT("hugin"));

    const wxString name = wxString::Format(_T("PTBatcherGUI-%s"), wxGetUserId().c_str());
    m_checker = new wxSingleInstanceChecker(name+wxT(".lock"),wxFileName::GetTempDir());
    bool IsFirstInstance=(!m_checker->IsAnotherRunning());

    if(IsFirstInstance)
    {
        if ( ! wxFile::Exists(m_xrcPrefix + wxT("/batch_frame.xrc")) )
        {
            wxMessageBox(_("xrc directory not found, hugin needs to be properly installed\nTried Path:") + m_xrcPrefix , _("Fatal Error"));
            return false;
        }
        // initialize image handlers
        wxInitAllImageHandlers();

        // Initialize all the XRC handlers.
        wxXmlResource::Get()->InitAllHandlers();
        wxXmlResource::Get()->AddHandler(new ProjectListBoxXmlHandler());
        // load XRC files
        wxXmlResource::Get()->Load(m_xrcPrefix + wxT("batch_frame.xrc"));
        wxXmlResource::Get()->Load(m_xrcPrefix + wxT("batch_toolbar.xrc"));
        wxXmlResource::Get()->Load(m_xrcPrefix + wxT("batch_menu.xrc"));
        wxXmlResource::Get()->Load(m_xrcPrefix + wxT("lensdb_dialogs.xrc"));
    };

    // parse arguments
    static const wxCmdLineEntryDesc cmdLineDesc[] =
    {
        //On wxWidgets 2.9, wide characters don't work here.
        //On previous versions, the wxT macro is required for unicode builds.
#if wxCHECK_VERSION(2,9,0)
        {
            wxCMD_LINE_SWITCH, "h", "help", "show this help message",
            wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP
        },
        { wxCMD_LINE_SWITCH, "b", "batch",  "run batch immediately" },
        { wxCMD_LINE_SWITCH, "p", "parallel",  "run batch projects in parallel" },
        { wxCMD_LINE_SWITCH, "o", "overwrite",  "overwrite previous files without asking" },
        { wxCMD_LINE_SWITCH, "s", "shutdown",  "shutdown computer after batch is complete" },
        { wxCMD_LINE_SWITCH, "v", "verbose",  "show verbose output when processing projects" },
        { wxCMD_LINE_SWITCH, "a", "assistant", "run the assistant on the given projects" },
        {
            wxCMD_LINE_PARAM,  NULL, NULL, _("stitch_project.pto [output prefix]|assistant_project.pto"),
            wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL + wxCMD_LINE_PARAM_MULTIPLE
        },
        { wxCMD_LINE_NONE }
#else
        {
            wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("show this help message"),
            wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP
        },
        { wxCMD_LINE_SWITCH, wxT("b"), wxT("batch"),  wxT("run batch immediately") },
        { wxCMD_LINE_SWITCH, wxT("p"), wxT("parallel"),  wxT("run batch projects in parallel") },
        { wxCMD_LINE_SWITCH, wxT("o"), wxT("overwrite"),  wxT("overwrite previous files without asking") },
        { wxCMD_LINE_SWITCH, wxT("s"), wxT("shutdown"),  wxT("shutdown computer after batch is complete") },
        { wxCMD_LINE_SWITCH, wxT("v"), wxT("verbose"),  wxT("show verbose output when processing projects") },
        { wxCMD_LINE_SWITCH, wxT("a"), wxT("assistant"), wxT("run the assistant on the given projects") },
        {
            wxCMD_LINE_PARAM,  NULL, NULL, _("stitch_project.pto [output prefix]|assistant_project.pto"),
            wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL + wxCMD_LINE_PARAM_MULTIPLE
        },
        { wxCMD_LINE_NONE }
#endif
    };
    wxCmdLineParser parser(cmdLineDesc, argc, argv);

    switch ( parser.Parse() )
    {
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

    wxClient client;
    wxConnectionBase* conn;
    wxString servername;
#ifdef __WINDOWS__
    servername=name;
#else
    servername=wxFileName::GetTempDir()+wxFileName::GetPathSeparator()+name+wxT(".ipc");
#endif
    if(IsFirstInstance)
    {
        m_frame = new BatchFrame(&m_locale,m_xrcPrefix);
        m_frame->RestoreSize();
#ifdef __WXMSW__
        provider->SetHelpController(&m_frame->GetHelpController());
        m_frame->GetHelpController().Initialize(m_xrcPrefix+wxT("data/hugin_help_en_EN.chm"));
#endif
        SetTopWindow(m_frame);
        if(!(m_frame->IsStartedMinimized()))
        {
            m_frame->Show(true);
        }
        else
        {
            m_frame->SetStatusInformation(_("PTBatcherGUI started"), true);
        };
        m_server = new BatchIPCServer();
        if (!m_server->Create(servername))
        {
            delete m_server;
            m_server = NULL;
        };
    }
    else
    {
        conn=client.MakeConnection(wxEmptyString, servername, IPC_START);
        if(!conn)
        {
            return false;
        }
    };
    //m_frame->SetLocaleAndXRC(&m_locale,m_xrcPrefix);
    //projectsRunning=0;
    unsigned int count = 0;
    if(parser.Found(wxT("a")))
    {
        //added assistant files
        while(parser.GetParamCount()>count)
        {
            wxString param = parser.GetParam(count);
            count++;
            wxFileName name(param);
            name.MakeAbsolute();
            if(name.FileExists())
            {
                //only add existing pto files
                if(name.GetExt().CmpNoCase(wxT("pto"))==0)
                {
                    if(IsFirstInstance)
                    {
                        m_frame->AddToList(name.GetFullPath(),Project::DETECTING);
                    }
                    else
                    {
                        conn->Request(wxT("D ")+name.GetFullPath());
                    };
                };
            };
        };
    }
    else
    {
        bool projectSpecified = false;
        //we collect all parameters - all project files <and their output prefixes>
        while(parser.GetParamCount()>count)
        {
            wxString param = parser.GetParam(count);
            count++;
            if(!projectSpecified)	//next parameter must be new script file
            {
                wxFileName name(param);
                name.MakeAbsolute();
                if(IsFirstInstance)
                {
                    m_frame->AddToList(name.GetFullPath());
                }
                else
                {
                    conn->Request(wxT("A ")+name.GetFullPath());
                }
                projectSpecified = true;
            }
            else	//parameter could be previous project's output prefix
            {
                wxFileName fn(param);
                fn.MakeAbsolute();
                if(!fn.HasExt())	//if there is no extension we have a prefix
                {
                    if(IsFirstInstance)
                    {
                        m_frame->ChangePrefix(-1,fn.GetFullPath());
                    }
                    else
                    {
                        conn->Request(wxT("P ")+fn.GetFullPath());
                    }
                    projectSpecified = false;
                }
                else
                {
                    wxString ext = fn.GetExt();
                    //we may still have a prefix, but with added image extension
                    if (ext.CmpNoCase(wxT("jpg")) == 0 || ext.CmpNoCase(wxT("jpeg")) == 0||
                            ext.CmpNoCase(wxT("tif")) == 0|| ext.CmpNoCase(wxT("tiff")) == 0 ||
                            ext.CmpNoCase(wxT("png")) == 0 || ext.CmpNoCase(wxT("exr")) == 0 ||
                            ext.CmpNoCase(wxT("pnm")) == 0 || ext.CmpNoCase(wxT("hdr")) == 0)
                    {
                        //extension will be removed before stitch, so there is no need to do it now
                        if(IsFirstInstance)
                        {
                            m_frame->ChangePrefix(-1,fn.GetFullPath());
                        }
                        else
                        {
                            conn->Request(wxT("P ")+fn.GetFullPath());
                        }
                        projectSpecified = false;
                    }
                    else //if parameter has a different extension we presume it is a new script file
                    {
                        //we add the new project
                        if(IsFirstInstance)
                        {
                            m_frame->AddToList(fn.GetFullPath());
                        }
                        else
                        {
                            conn->Request(wxT("A ")+fn.GetFullPath());
                        }
                        projectSpecified = true;
                    }
                } //else of if(!fn.HasExt())
            }
        }
    }

#ifdef __WXMAC__
    m_macFileNameToOpenOnStart = wxT("");
    wxYield();
    if(m_macFileNameToOpenOnStart != wxT(""))
    {
        wxFileName fn(m_macFileNameToOpenOnStart);
        m_frame->AddToList(fn.GetFullPath());
    }
#endif

    if(IsFirstInstance)
    {
        wxConfigBase* config=wxConfigBase::Get();
        if (parser.Found(wxT("p")))
        {
            config->Write(wxT("/BatchFrame/ParallelCheck"), 1l);
        }
        if (parser.Found(wxT("s")))
        {
            config->Write(wxT("/BatchFrame/ShutdownCheck"), 1l);
        }
        if (parser.Found(wxT("o")))
        {
            config->Write(wxT("/BatchFrame/OverwriteCheck"), 1l);
        }
        if (parser.Found(wxT("v")))
        {
            config->Write(wxT("/BatchFrame/VerboseCheck"), 1l);
        }
        config->Flush();
    }
    else
    {
        if (parser.Found(wxT("p")))
        {
            conn->Request(wxT("SetParallelCheck"));
        }
        if (parser.Found(wxT("s")))
        {
            conn->Request(wxT("SetShutdownCheck"));
        }
        if (parser.Found(wxT("o")))
        {
            conn->Request(wxT("SetOverwriteCheck"));
        }
        if (parser.Found(wxT("v")))
        {
            conn->Request(wxT("SetVerboseCheck"));
        }
        conn->Request(wxT("BringWindowToTop"));
        if(parser.Found(wxT("b")))
        {
            conn->Request(wxT("RunBatch"));
        }
        conn->Disconnect();
        delete conn;
        delete m_checker;
        return false;
    };
    m_frame->SetCheckboxes();
    m_frame->PropagateDefaults();
    //deactivate verbose output if started minimized
    if(m_frame->IsStartedMinimized())
    {
        m_frame->SetInternalVerbose(false);
    };
    if (parser.Found(wxT("b")) )
    {
        m_frame->RunBatch();
    }
    return true;
}

int PTBatcherGUI::OnExit()
{
    HuginBase::LensDB::LensDB::Clean();
    delete m_checker;
    delete m_server;
    return 0;
}

void PTBatcherGUI::OnItemActivated(wxListEvent& event)
{
    wxCommandEvent dummy;
    m_frame->OnButtonOpenWithHugin(dummy);
}

void PTBatcherGUI::OnKeyDown(wxKeyEvent& event)
{
    wxCommandEvent dummy;
    switch(event.GetKeyCode())
    {
        case WXK_DELETE:
            m_frame->OnButtonRemoveFromList(dummy);
            break;
        case WXK_INSERT:
            m_frame->OnButtonAddToStitchingQueue(dummy);
            break;
        case WXK_ESCAPE:
            m_frame->OnButtonCancel(dummy);
            break;
        default:
            event.Skip();
            break;
    }

}

#ifdef __WXMAC__
// wx calls this method when the app gets "Open file" AppleEvent
void PTBatcherGUI::MacOpenFile(const wxString& fileName)
{
    if(!m_frame)
    {
        m_macFileNameToOpenOnStart = fileName;
    }
    else
    {
        wxFileName fn(fileName);
        m_frame->AddToList(fn.GetFullPath());
    }
}
#endif

#if wxCHECK_VERSION(2,9,0)
#define RETURNEMPTYSTRING return wxEmptyString
const void* BatchIPCConnection::OnRequest(const wxString& topic, const wxString& item, size_t* size, wxIPCFormat format)
#else
#define RETURNEMPTYSTRING return wxT("")
wxChar* BatchIPCConnection::OnRequest(const wxString& topic, const wxString& item, int* size, wxIPCFormat format)
#endif
{
    *size=wxNO_LEN;
    BatchFrame* MyBatchFrame=wxGetApp().GetFrame();
    if(item.Left(1)==wxT("A"))
    {
        MyBatchFrame->AddToList(item.Mid(2));
        RETURNEMPTYSTRING;
    };
    if(item.Left(1)==wxT("D"))
    {
        MyBatchFrame->AddToList(item.Mid(2),Project::DETECTING);
        RETURNEMPTYSTRING;
    };
    if(item.Left(1)==wxT("P"))
    {
        MyBatchFrame->ChangePrefix(-1,item.Mid(2));
        RETURNEMPTYSTRING;
    };
    wxCommandEvent event;
    event.SetInt(1);
    if(item==wxT("SetParallelCheck"))
        if(!MyBatchFrame->GetCheckParallel())
        {
            MyBatchFrame->OnCheckParallel(event);
            MyBatchFrame->SetCheckboxes();
        };
    if(item==wxT("SetShutdownCheck"))
        if(!MyBatchFrame->GetCheckShutdown())
        {
            MyBatchFrame->OnCheckShutdown(event);
            MyBatchFrame->SetCheckboxes();
        };
    if(item==wxT("SetOverwriteCheck"))
        if(!MyBatchFrame->GetCheckOverwrite())
        {
            MyBatchFrame->OnCheckOverwrite(event);
            MyBatchFrame->SetCheckboxes();
        };
    if(item==wxT("SetVerboseCheck"))
        if(!MyBatchFrame->GetCheckVerbose())
        {
            MyBatchFrame->OnCheckVerbose(event);
            MyBatchFrame->SetCheckboxes();
        };
    if(item==wxT("BringWindowToTop"))
    {
        MyBatchFrame->RequestUserAttention();
    }
    if(item==wxT("RunBatch"))
    {
        wxCommandEvent myEvent(wxEVT_COMMAND_TOOL_CLICKED ,XRCID("tool_start"));
        MyBatchFrame->GetEventHandler()->AddPendingEvent(myEvent);
    };
    RETURNEMPTYSTRING;
};

wxConnectionBase* BatchIPCServer::OnAcceptConnection (const wxString& topic)
{
    if(topic==IPC_START)
    {
        return new BatchIPCConnection;
    }
    return NULL;
};
