#include "PHONONRunActionMessenger.hh"
#include "PHONONRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHONONRunActionMessenger::PHONONRunActionMessenger(PHONONRunAction* runAction)
 : G4UImessenger(),
   fRunAction(runAction)
{
  fRunDirectory = new G4UIdirectory("/PHONON/run/");
  fRunDirectory->SetGuidance("Run control commands");

  fOutputFileCmd = new G4UIcmdWithAString("/PHONON/run/setOutputFile", this);
  fOutputFileCmd->SetGuidance("Set output file name for the run.");
  fOutputFileCmd->SetParameterName("filename", false);
  fOutputFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PHONONRunActionMessenger::~PHONONRunActionMessenger()
{
  delete fOutputFileCmd;
  delete fRunDirectory;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PHONONRunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fOutputFileCmd) {
    fRunAction->SetOutputFile(newValue);
    G4cout << "Output file set to: " << newValue << G4endl;
  }
}



