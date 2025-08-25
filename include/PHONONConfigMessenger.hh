/***********************************************************************\
 * This software is licensed under the terms of the GNU General Public *
 * License version 3 or later. See G4CMP/LICENSE for the full license. *
\***********************************************************************/

#ifndef PHONONConfigMessenger_hh
#define PHONONConfigMessenger_hh 1

// $Id$
// File:  PhononConfigMessenger.hh
//
// Description:	Macro command defitions to set user configuration in
//		PhononConfigManager.
//
// 20170816  Michael Kelsey

#include "G4UImessenger.hh"

class PHONONConfigManager;
class G4UIcmdWithAString;
class G4UIcommand;


class PHONONConfigMessenger : public G4UImessenger {
public:
  PHONONConfigMessenger(PHONONConfigManager* theData);
  virtual ~PHONONConfigMessenger();

  void SetNewValue(G4UIcommand* cmd, G4String value);

private:
  PHONONConfigManager* theManager;
  G4UIcmdWithAString* hitsCmd;

private:
  PHONONConfigMessenger(const PHONONConfigMessenger&);	// Copying is forbidden
  PHONONConfigMessenger& operator=(const PHONONConfigMessenger&);
};

#endif /* PHONONConfigMessenger_hh */
