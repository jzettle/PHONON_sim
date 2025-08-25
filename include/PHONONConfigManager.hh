/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef PHONONConfigManager_hh
#define PHONONConfigManager_hh 1

// $Id$
// File:  PhononConfigManager.hh
//
// Description:	Singleton container class for user configuration of G4CMP
//		phonon example. Looks for environment variables	at
//		initialization to set default values; active values may be
//		changed via macro commands (see PhononConfigMessenger).
//
// 20170816  M. Kelsey -- Extract hit filename from G4CMPConfigManager.

#include "globals.hh"

class PHONONConfigMessenger;


class PHONONConfigManager {
public:
  ~PHONONConfigManager();	// Must be public for end-of-job cleanup
  static PHONONConfigManager* Instance();   // Only needed by static accessors

  // Access current values
  static const G4String& GetHitOutput()  { return Instance()->Hit_file; }

  // Change values (e.g., via Messenger)
  static void SetHitOutput(const G4String& name)
    { Instance()->Hit_file=name; UpdateGeometry(); }

  static void UpdateGeometry();

private:
  PHONONConfigManager();		// Singleton: only constructed on request
  PHONONConfigManager(const PHONONConfigManager&) = delete;
  PHONONConfigManager(PHONONConfigManager&&) = delete;
  PHONONConfigManager& operator=(const PHONONConfigManager&) = delete;
  PHONONConfigManager& operator=(PHONONConfigManager&&) = delete;

  static PHONONConfigManager* theInstance;

private:
  G4String Hit_file;	// Output file of e/h hits ($G4CMP_HIT_FILE)

  PHONONConfigMessenger* messenger;
};

#endif	/* PhononConfigManager_hh */
