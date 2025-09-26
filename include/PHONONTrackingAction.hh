#ifndef PHONONTrackingAction_h
#define PHONONTrackingAction_h 1

#include "globals.hh"
#include "G4Track.hh"
#include "G4UserTrackingAction.hh"

class PHONONTrackingAction : public G4UserTrackingAction
{
public:

  PHONONTrackingAction();
  virtual ~PHONONTrackingAction();
  virtual void PreUserTrackingAction(const G4Track* g4Track);
  virtual void PostUserTrackingAction(const G4Track* g4Track);
  
private:
};

#endif
    