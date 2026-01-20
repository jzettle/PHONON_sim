#include "PHONONStackingAction.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4PrimaryParticle.hh"
#include "G4GenericAnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

PHONONStackingAction::PHONONStackingAction(PHONONEventAction *eventAction) : G4UserStackingAction(),
  fEventAction(eventAction)
{}
PHONONStackingAction::~PHONONStackingAction(){}

G4ClassificationOfNewTrack PHONONStackingAction::ClassifyNewTrack(const G4Track* g4Track)
{

    int offset = 0;
    offset = fEventAction->GetEventOffset();

    G4RunManager* runManager = G4RunManager::GetRunManager();
    int eventID = runManager->GetCurrentEvent()->GetEventID();

    int totalEvents = runManager->GetNumberOfEventsToBeProcessed();
    eventID += totalEvents*offset;

    auto analysisManager = G4GenericAnalysisManager::Instance();

    int parentID = g4Track->GetParentID();
    if ((g4Track->GetParticleDefinition()->GetParticleName() == "phononL" || g4Track->GetParticleDefinition()->GetParticleName() == "phononTS" || g4Track->GetParticleDefinition()->GetParticleName() == "phononTF") && parentID == 1) {
        int trackID = g4Track->GetTrackID();
          
        G4double kineticEnergy = g4Track->GetKineticEnergy();
        G4ThreeVector momentum = g4Track->GetMomentum();
        G4String creator = g4Track->GetCreatorProcess()->GetProcessName();
        G4double weight = g4Track->GetWeight();
        analysisManager->FillNtupleDColumn(2, 0, eventID);
        analysisManager->FillNtupleDColumn(2, 1, trackID);
        analysisManager->FillNtupleSColumn(2, 2, g4Track->GetParticleDefinition()->GetParticleName());
        analysisManager->FillNtupleDColumn(2, 3, kineticEnergy);
        analysisManager->FillNtupleDColumn(2, 4, momentum.x());
        analysisManager->FillNtupleDColumn(2, 5, momentum.y());
        analysisManager->FillNtupleDColumn(2, 6, momentum.z());
        analysisManager->FillNtupleSColumn(2, 7, creator);
        analysisManager->FillNtupleDColumn(2, 8, weight);
        analysisManager->AddNtupleRow(2);
    }
    if ((g4Track->GetParticleDefinition()->GetParticleName() == "phononL" || g4Track->GetParticleDefinition()->GetParticleName() == "phononTS" || g4Track->GetParticleDefinition()->GetParticleName() == "phononTF") && parentID != 1) {
        int trackID = g4Track->GetTrackID();
        G4double kineticEnergy = g4Track->GetKineticEnergy();
        G4ThreeVector momentum = g4Track->GetMomentum();
        G4String creator = g4Track->GetCreatorProcess()->GetProcessName();
        G4double weight = g4Track->GetWeight();
        analysisManager->FillNtupleDColumn(7, 0, eventID);
        analysisManager->FillNtupleDColumn(7, 1, trackID);
        analysisManager->FillNtupleSColumn(7, 2, g4Track->GetParticleDefinition()->GetParticleName());
        analysisManager->FillNtupleDColumn(7, 3, kineticEnergy);
        analysisManager->FillNtupleDColumn(7, 4, momentum.x());
        analysisManager->FillNtupleDColumn(7, 5, momentum.y());
        analysisManager->FillNtupleDColumn(7, 6, momentum.z());
        analysisManager->FillNtupleSColumn(7, 7, creator);
        analysisManager->FillNtupleDColumn(7, 8, weight);
        analysisManager->AddNtupleRow(7);
    }

    return fUrgent;
}