#include "PHONONSteppingAction.hh"
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

PHONONSteppingAction::PHONONSteppingAction(PHONONEventAction *eventAction, PHONONDetectorConstruction *detectorConstruction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fDetectorConstruction(detectorConstruction)
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

    //attempt to get event offset from event action
    int offset = 0;
    offset = fEventAction->GetEventOffset();
    G4String geometryType = fDetectorConstruction->GetGeometryType();

    int totalEvents = runManager->GetNumberOfEventsToBeProcessed();
    eventID += totalEvents*offset;

    if(postStepVolume)
    {
        const G4String postVolumeName = postStepVolume->GetName();
        auto analysisManager = G4GenericAnalysisManager::Instance();
    
        // Check if the particle is entering the "Water" volume from the "Lead" volume
        if ((postVolumeName == "WaterShieldVol" && preVolumeName == "FullShieldVol") && particle->GetParticleName() == "neutron") {
            const G4Event* currentEvent = runManager->GetCurrentEvent();
            G4PrimaryParticle* primaryParticle = currentEvent->GetPrimaryVertex()->GetPrimary(0);
            double primaryEnergy = primaryParticle->GetKineticEnergy();
            analysisManager->FillNtupleDColumn(4, 0, eventID);
            analysisManager->FillNtupleDColumn(4, 1, trackID);
            analysisManager->FillNtupleDColumn(4, 2, pdgCode);
            analysisManager->FillNtupleDColumn(4, 3, kineticEnergy);
            analysisManager->FillNtupleDColumn(4, 4, position_x);
            analysisManager->FillNtupleDColumn(4, 5, position_y);
            analysisManager->FillNtupleDColumn(4, 6, position_z);
            analysisManager->FillNtupleDColumn(4, 7, primaryEnergy);
            analysisManager->AddNtupleRow(4);
        }
        // Check if the particle is entering the "Fridge" volume from the "Water" volume
        if((postVolumeName == "AirVol" && preVolumeName == "WaterShieldVol") && particle->GetParticleName() == "neutron") {
            const G4Event* currentEvent = runManager->GetCurrentEvent();
            G4PrimaryParticle* primaryParticle = currentEvent->GetPrimaryVertex()->GetPrimary(0);
            double primaryEnergy = primaryParticle->GetKineticEnergy();
            analysisManager->FillNtupleDColumn(5, 0, eventID);
            analysisManager->FillNtupleDColumn(5, 1, trackID);
            analysisManager->FillNtupleDColumn(5, 2, pdgCode);
            analysisManager->FillNtupleDColumn(5, 3, kineticEnergy); 
            analysisManager->FillNtupleDColumn(5, 4, position_x);
            analysisManager->FillNtupleDColumn(5, 5, position_y);
            analysisManager->FillNtupleDColumn(5, 6, position_z);
            analysisManager->FillNtupleDColumn(5, 7, primaryEnergy);
            analysisManager->AddNtupleRow(5);
        }
        G4String checkVolume = "";
        if(geometryType == "scintillator") {
            checkVolume = "ScintillatorVol";
        }
        else
            checkVolume = "SubstrateVol";
        // Check if the particle is entering the "LiNbO3" volume from the "Fridge" volume
        if((postVolumeName.find(checkVolume) != std::string::npos && preVolumeName == "AirVol") && particle->GetParticleName() == "neutron") {
            const G4Event* currentEvent = runManager->GetCurrentEvent();
            G4PrimaryParticle* primaryParticle = currentEvent->GetPrimaryVertex()->GetPrimary(0);
            double primaryEnergy = primaryParticle->GetKineticEnergy();
            analysisManager->FillNtupleDColumn(6, 0, eventID);
            analysisManager->FillNtupleDColumn(6, 1, trackID);
            analysisManager->FillNtupleDColumn(6, 2, pdgCode);
            analysisManager->FillNtupleDColumn(6, 3, kineticEnergy); 
            analysisManager->FillNtupleDColumn(6, 4, position_x);
            analysisManager->FillNtupleDColumn(6, 5, position_y);
            analysisManager->FillNtupleDColumn(6, 6, position_z);
            analysisManager->FillNtupleDColumn(6, 7, primaryEnergy);
            analysisManager->AddNtupleRow(6);
        }
        if((postVolumeName.find(checkVolume) != std::string::npos && preVolumeName == "AirVol") && particle->GetParticleName() == "gamma") {
            const G4Event* currentEvent = runManager->GetCurrentEvent();
            G4PrimaryParticle* primaryParticle = currentEvent->GetPrimaryVertex()->GetPrimary(0);
            double primaryEnergy = primaryParticle->GetKineticEnergy();
            analysisManager->FillNtupleDColumn(8, 0, eventID);
            analysisManager->FillNtupleDColumn(8, 1, trackID);
            analysisManager->FillNtupleDColumn(8, 2, pdgCode);
            analysisManager->FillNtupleDColumn(8, 3, kineticEnergy); 
            analysisManager->FillNtupleDColumn(8, 4, position_x);
            analysisManager->FillNtupleDColumn(8, 5, position_y);
            analysisManager->FillNtupleDColumn(8, 6, position_z);
            analysisManager->FillNtupleDColumn(8, 7, primaryEnergy);
            analysisManager->AddNtupleRow(8);
        }
    }
}

