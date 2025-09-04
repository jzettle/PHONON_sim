#include "PHONONSteppingAction.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4GenericAnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

PHONONSteppingAction::PHONONSteppingAction(PHONONEventAction *eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}
PHONONSteppingAction::~PHONONSteppingAction(){}

void PHONONSteppingAction::UserSteppingAction(const G4Step* step)
{
    
    // Get the pre-step and post-step points
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4StepPoint* postStepPoint = step->GetPostStepPoint();

    const G4VPhysicalVolume* preStepVolume = preStepPoint->GetPhysicalVolume(); //volume particle is in pre step
    const G4VPhysicalVolume* postStepVolume = postStepPoint->GetPhysicalVolume();
    
    // Get the volume names
    const G4String preVolumeName = preStepVolume->GetName();
    
    const G4Track* track = step->GetTrack();
    const G4ParticleDefinition* particle = track->GetDefinition();

    G4RunManager* runManager = G4RunManager::GetRunManager();
    int eventID = runManager->GetCurrentEvent()->GetEventID();
    int trackID = track->GetTrackID();
    int pdgCode = particle->GetPDGEncoding();
    double kineticEnergy = track->GetKineticEnergy()/MeV;
    
    if(postStepVolume)
    {
        const G4String postVolumeName = postStepVolume->GetName();
        auto analysisManager = G4GenericAnalysisManager::Instance();
    
        // Check if the particle is entering the "Lead" volume from the "World" volume
        if ((postVolumeName == "WaterShieldVol" && preVolumeName == "FullShieldVol") && particle->GetParticleName() == "neutron") {
            analysisManager->FillNtupleDColumn(3, 0, eventID);
            analysisManager->FillNtupleDColumn(3, 1, trackID);
            analysisManager->FillNtupleDColumn(3, 2, pdgCode);
            analysisManager->FillNtupleDColumn(3, 3, kineticEnergy);
            analysisManager->AddNtupleRow(3);
        }
        if((postVolumeName == "AirVol" && preVolumeName == "WaterShieldVol") && particle->GetParticleName() == "neutron") {
            analysisManager->FillNtupleDColumn(4, 0, eventID);
            analysisManager->FillNtupleDColumn(4, 1, trackID);
            analysisManager->FillNtupleDColumn(4, 2, pdgCode);
            analysisManager->FillNtupleDColumn(4, 3, kineticEnergy); 
            analysisManager->AddNtupleRow(4);
        }
    }
}