#include "PHONONEventAction.hh"
#include "PHONONEventMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

PHONONEventMessenger::PHONONEventMessenger(PHONONEventAction* Eve)
 : G4UImessenger(),
   fEventAction(Eve)
{
  fEventDirectory = new G4UIdirectory("/PHONONEvent/");
  fEventDirectory->SetGuidance("UI commands specific to this example.");

  fEventOffsetCmd = new G4UIcmdWithAnInteger("/PHONONEvent/offset",this);
  fEventOffsetCmd->SetGuidance("Define an event ID offset");
  fEventOffsetCmd->SetParameterName("eventOffset",false);
  fEventOffsetCmd->AvailableForStates(G4State_Idle);
}

PHONONEventMessenger::~PHONONEventMessenger()
{
  delete fEventOffsetCmd;
  delete fEventDirectory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHONONEventMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == fEventOffsetCmd )
   { fEventAction->SetEventOffset(fEventOffsetCmd->GetNewIntValue(newValue));}
}   
