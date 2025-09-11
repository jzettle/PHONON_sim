#include "PHONONSteppingAction.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4GenericAnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
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
    G4ThreeVector position = track->GetPosition();
    double norm = position.mag();
    double position_x = position.x()/norm; //normalize position vector to get direction
    double position_y = position.y()/norm;
    double position_z = position.z()/norm;

    if(postStepVolume)
    {
        const G4String postVolumeName = postStepVolume->GetName();
        auto analysisManager = G4GenericAnalysisManager::Instance();
    
        // Check if the particle is entering the "Water" volume from the "Lead" volume
        if ((postVolumeName == "WaterShieldVol" && preVolumeName == "FullShieldVol") && particle->GetParticleName() == "neutron") {
            analysisManager->FillNtupleDColumn(3, 0, eventID);
            analysisManager->FillNtupleDColumn(3, 1, trackID);
            analysisManager->FillNtupleDColumn(3, 2, pdgCode);
            analysisManager->FillNtupleDColumn(3, 3, kineticEnergy);
            analysisManager->FillNtupleDColumn(3, 4, position_x);
            analysisManager->FillNtupleDColumn(3, 5, position_y);
            analysisManager->FillNtupleDColumn(3, 6, position_z);
            analysisManager->AddNtupleRow(3);
        }
        // Check if the particle is entering the "Fridge" volume from the "Water" volume
        if((postVolumeName == "AirVol" && preVolumeName == "WaterShieldVol") && particle->GetParticleName() == "neutron") {
            analysisManager->FillNtupleDColumn(4, 0, eventID);
            analysisManager->FillNtupleDColumn(4, 1, trackID);
            analysisManager->FillNtupleDColumn(4, 2, pdgCode);
            analysisManager->FillNtupleDColumn(4, 3, kineticEnergy); 
            analysisManager->FillNtupleDColumn(4, 4, position_x);
            analysisManager->FillNtupleDColumn(4, 5, position_y);
            analysisManager->FillNtupleDColumn(4, 6, position_z);
            analysisManager->AddNtupleRow(4);
        }
        // Check if the particle is entering the "LiNbO3" volume from the "Fridge" volume
        if((postVolumeName == "ScintillatorVol" && preVolumeName == "AirVol") && particle->GetParticleName() == "neutron") {
            analysisManager->FillNtupleDColumn(5, 0, eventID);
            analysisManager->FillNtupleDColumn(5, 1, trackID);
            analysisManager->FillNtupleDColumn(5, 2, pdgCode);
            analysisManager->FillNtupleDColumn(5, 3, kineticEnergy); 
            analysisManager->FillNtupleDColumn(5, 4, position_x);
            analysisManager->FillNtupleDColumn(5, 5, position_y);
            analysisManager->FillNtupleDColumn(5, 6, position_z);
            analysisManager->AddNtupleRow(5);
        }
    }
}