// -*- c-basic-offset: 4 -*-
/** @file AssistantPanel.h
 *
 *  @author Pablo d'Angelo <pablo.dangelo@web.de>
 *
 *  $Id$
 *
 *  This is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _ASSISTANTPANEL_H
#define _ASSISTANTPANEL_H

#include <panodata/StandardImageVariableGroups.h>

using namespace PT;

/** The assistant panel. Kind of a helper panel.
 *
 *  Idea shamelessly stolen from PTGui
 *
 */
class AssistantPanel: public wxPanel, public PT::PanoramaObserver
{
public:
    AssistantPanel();

    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxT("panel"));

    void Init(PT::Panorama * pano);

    virtual ~AssistantPanel(void) ;

    /** this is called whenever the panorama has changed.
     *
     *  This function must now update all the gui representations
     *  of the panorama to display the new state.
     *
     *  Functions that change the panororama must not update
     *  the GUI directly. The GUI should always be updated
     *  to reflect the current panorama state in this function.
     *
     *  This avoids unnessecary close coupling between the
     *  controller and the view (even if they sometimes
     *  are in the same object). See model view controller
     *  pattern.
     */
    virtual void panoramaChanged(PT::Panorama &pano);
    void panoramaImagesChanged(PT::Panorama &pano, const PT::UIntSet & imgNr);

private:
    // a window event
    //void OnSize(wxSizeEvent & e);

    /** the model */
    Panorama * m_pano;
    HuginBase::StandardImageVariableGroups * m_variable_groups;

    // event handlers
    void OnLoadImages( wxCommandEvent & e );
    void OnAlign( wxCommandEvent & e );
    void OnAlignSendToBatch(wxCommandEvent & e);
    void OnCreate( wxCommandEvent & e );
    void OnExifToggle( wxCommandEvent & e);
    void OnLoadLens(wxCommandEvent & e);
    void OnLensTypeChanged (wxCommandEvent & e);
    void OnFocalLengthChanged(wxCommandEvent & e);
    void OnCropFactorChanged(wxCommandEvent & e);

    wxPanel *  m_panel;
    wxStaticText * m_imagesText;
    wxButton * m_alignButton;
    wxButton * m_alignBatchButton;
    wxStaticText * m_alignText;
    wxButton * m_createButton;
    wxCheckBox * m_exifToggle;
    wxChoice   * m_lensTypeChoice;
    wxTextCtrl * m_focalLengthText;
    wxTextCtrl * m_cropFactorText;
    wxButton   * m_loadLensButton;

    int m_degDigits;

    bool m_noImage;

    DECLARE_EVENT_TABLE()

    DECLARE_DYNAMIC_CLASS(AssistantPanel)
};

/** xrc handler */
class AssistantPanelXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(AssistantPanelXmlHandler)

public:
    AssistantPanelXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};



#endif // _ASSISTANTPANEL_H
