/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

// $Id$
// File:  PhononConfigManager.cc
//
// Description:	Singleton container class for user configuration of G4CMP
//		phonon example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see PhononConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "PHONONConfigManager.hh"
#include "PHONONConfigMessenger.hh"
#include "G4RunManager.hh"
#include <stdlib.h>


// Constructor and Singleton Initializer

PHONONConfigManager* PHONONConfigManager::theInstance = 0;

PHONONConfigManager* PHONONConfigManager::Instance() {
  if (!theInstance) theInstance = new PHONONConfigManager;
  return theInstance;
}

PHONONConfigManager::PHONONConfigManager()
  : Hit_file(getenv("G4CMP_HIT_FILE")?getenv("G4CMP_HIT_FILE"):"phonon_hits.txt"),
    messenger(new PHONONConfigMessenger(this)) {;}

PHONONConfigManager::~PHONONConfigManager() {
  delete messenger; messenger=0;
}


// Trigger rebuild of geometry if parameters change

void PHONONConfigManager::UpdateGeometry() {
  G4RunManager::GetRunManager()->ReinitializeGeometry(true);
}
