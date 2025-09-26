#include "PHONONTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4EventManager.hh"

PHONONTrackingAction::PHONONTrackingAction(){}
PHONONTrackingAction::~PHONONTrackingAction(){}

void PHONONTrackingAction::PreUserTrackingAction(const G4Track* g4Track)
{
    G4TrackingManager* trackingManager = G4EventManager::GetEventManager()->GetTrackingManager();
    trackingManager->SetStoreTrajectory(true);
}

void PHONONTrackingAction::PostUserTrackingAction(const G4Track* g4Track)
{
}