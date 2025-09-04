#ifndef PHONONSteppingAction_h
#define PHONONSteppingAction_h 1

#include "globals.hh"

#include "G4UserSteppingAction.hh"

class PHONONEventAction;


class PHONONSteppingAction : public G4UserSteppingAction
{
public:
  PHONONSteppingAction(PHONONEventAction *eventAction);
  virtual ~PHONONSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);

private:
    PHONONEventAction* fEventAction;

};

#endif