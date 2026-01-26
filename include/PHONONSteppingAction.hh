#ifndef PHONONSteppingAction_h
#define PHONONSteppingAction_h 1

#include "globals.hh"
#include "PHONONEventAction.hh"
#include "PHONONDetectorConstruction.hh"

#include "G4UserSteppingAction.hh"

class PHONONEventAction;
class PHONONDetectorConstruction;


class PHONONSteppingAction : public G4UserSteppingAction
{
public:
  PHONONSteppingAction(PHONONEventAction *eventAction, PHONONDetectorConstruction *detectorConstruction);
  virtual ~PHONONSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);

private:
    PHONONEventAction* fEventAction;
    PHONONDetectorConstruction* fDetectorConstruction;

};

#endif