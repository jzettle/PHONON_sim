#ifndef PHONONStackingAction_h
#define PHONONStackingAction_h 1

#include "globals.hh"
#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"
#include "PHONONEventAction.hh"

class PHONONEventAction;

class PHONONStackingAction : public G4UserStackingAction

{
public:
  PHONONStackingAction(PHONONEventAction *eventAction);
  virtual ~PHONONStackingAction();

  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);

private:
    PHONONEventAction* fEventAction;
};

#endif