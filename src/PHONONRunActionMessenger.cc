#include "PHONONRunActionMessenger.hh"
#include "PHONONRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

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

  // Add command to set HDF5 output option
  fUseHDF5Cmd = new G4UIcmdWithABool("/PHONON/run/useHDF5", this);
  fUseHDF5Cmd->SetGuidance("Use HDF5 format for output files.");
  fUseHDF5Cmd->SetParameterName("useHDF5", false);
  fUseHDF5Cmd->SetDefaultValue(false);
  fUseHDF5Cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PHONONRunActionMessenger::~PHONONRunActionMessenger()
{
  delete fOutputFileCmd;
  delete fUseHDF5Cmd;
  delete fRunDirectory;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PHONONRunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fOutputFileCmd) {
    fRunAction->SetOutputFile(newValue);
    G4cout << "Output file set to: " << newValue << G4endl;
  }
  else if (command == fUseHDF5Cmd) {
    fRunAction->SetUseHDF5(newValue);
    G4cout << "Using HDF5 output: " << newValue << G4endl;
  }

}



