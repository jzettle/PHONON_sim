#ifndef PHONONEventMessenger_h
#define PHONONEventMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PHONONEventAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;


class PHONONEventMessenger: public G4UImessenger
{
  public:
    PHONONEventMessenger(PHONONEventAction* );
    virtual ~PHONONEventMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PHONONEventAction*  fEventAction;

    G4UIdirectory*           fEventDirectory;
    G4UIcmdWithAnInteger*    fEventOffsetCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif