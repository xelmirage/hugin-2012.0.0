// -*- c-basic-offset: 4 -*-

/** @file huginConfig.h
 *
 *  @brief functions for interaction with the hugin configuration file
 *
 *  @author Pablo d'Angelo <pablo.dangelo@web.de>
 *
 *  $Id$
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

#ifndef _Hgn1_huginConfig_H
#define _Hgn1_huginConfig_H

#include <hugin_shared.h>
#include <panoinc_WX.h>

#include "PT/Panorama.h"
#include <algorithms/panorama_makefile/PanoramaMakefilelibExport.h>
#include <algorithms/assistant_makefile/AssistantMakefilelibExport.h>

namespace PT {
struct PTPrograms : public HuginBase::PanoramaMakefilelibExport::PTPrograms {};
struct AssistantPrograms : public HuginBase::AssistantMakefilelibExport::AssistantPrograms {};
}

WXIMPEX PT::PTPrograms getPTProgramsConfig(wxString huginRoot, wxConfigBase * config);
WXIMPEX PT::AssistantPrograms getAssistantProgramsConfig(wxString huginRoot, wxConfigBase * config);

/** get the path to the directory where the currently running executable is
 *  stored.
 *
 *  Should work well on Windows, might work on OSX and does not work on
 *  Unix.
 *
 */
WXIMPEX wxString getExePath(wxString argv0);

#endif
