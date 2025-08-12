#ifndef PHONONRunActionMessenger_h
#define PHONONRunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PHONONRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for PHONONRunAction.
///
/// It implements commands:
/// - /PHONON/run/setOutputFile filename
class PHONONRunActionMessenger: public G4UImessenger
{
  public:
    PHONONRunActionMessenger(PHONONRunAction* runAction);
    virtual ~PHONONRunActionMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PHONONRunAction* fRunAction;

    G4UIdirectory* fRunDirectory;
    G4UIcmdWithAString* fOutputFileCmd;
    G4UIcmdWithABool* fUseHDF5Cmd;
};

#endif

