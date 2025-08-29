//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B2EventAction.cc
/// \brief Implementation of the B2EventAction class

#include "PHONONEventAction.hh"
#include "PHONONScintSD.hh"
#include "PHONONRunAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4CMPElectrodeHit.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4PrimaryParticle.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4VHitsCollection.hh"
#include "G4ios.hh"
#include <limits>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHONONEventAction::PHONONEventAction()
: G4UserEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHONONEventAction::~PHONONEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHONONEventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHONONEventAction::EndOfEventAction(const G4Event* event)
{
  // get number of stored trajectories

  G4int eventID = event->GetEventID();

  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  G4RunManager* runManager = G4RunManager::GetRunManager();
  const G4Event* currentEvent = runManager->GetCurrentEvent();
  auto analysisManager = G4GenericAnalysisManager::Instance();
  if (!analysisManager->IsOpenFile()) {
    G4cerr << "Error: Analysis manager is not open." << G4endl;
    return;
  }
  //set file type to HDF5 if required
  //analysisManager->SetDefaultFileType("root");
  //G4cout << "Using HDF5 output format." << G4endl;

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  double primary_x = std::numeric_limits<double>::quiet_NaN(), primary_y = std::numeric_limits<double>::quiet_NaN(), primary_z = std::numeric_limits<double>::quiet_NaN();
  //grab position of primary from trajectory container
  if (n_trajectories > 0) {
    G4Trajectory* primaryTraj = (G4Trajectory*)(*trajectoryContainer)[0];
    if (primaryTraj) {
      G4ThreeVector primaryPos = primaryTraj->GetPoint(0)->GetPosition();
      primary_x = primaryPos.x();
      primary_y = primaryPos.y();
      primary_z = primaryPos.z();
    }
  }

  //extract primary particle information (this is nicer than going through the trajectory container)
  // Note: This assumes there is at least one primary vertex and one primary particle.
  G4PrimaryParticle* primaryParticle = event->GetPrimaryVertex()->GetPrimary(0);

  // periodic printing
  if ( eventID < 100 || eventID % 100 == 0) {
    G4cout << ">>> Event: " << eventID  << G4endl;
    if ( trajectoryContainer ) {
      G4cout << "    " << n_trajectories
             << " trajectories stored in this event." << G4endl;
    }
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    G4cout << "    "  
           << hc->GetSize() << " hits stored in this event" << G4endl;
  }

  // Get the hits collection ID (you'll need the SD name used when registering)
  G4int collectionID = sdManager->GetCollectionID("ScintChamberSD/ScintHitsCollection");
  if (collectionID < 0) {
    G4cerr << "Hits collection not found, proceeding!" << G4endl;
    return;
  }
  G4HCofThisEvent* HCofEvent = currentEvent->GetHCofThisEvent();

  // Access your specific hits collection type
  PHONONScintHitsCollection* myHitsCollection = 0;
  if (HCofEvent) {
      myHitsCollection = (PHONONScintHitsCollection*)(HCofEvent->GetHC(collectionID));
  }

  //open the output file for appending
  fOutputFile.open("hits.txt", std::ios::app);

  G4double totalNR = 0.0;
  G4double totalER = 0.0;

  if (myHitsCollection) {
    for (unsigned int i = 0; i < myHitsCollection->entries(); ++i) {
      PHONONScintHit* hit = (*myHitsCollection)[i];
      
      if (hit) {
      /*
        // Output hit information to the file
          fOutputFile << "Event ID: " << eventID
                      << ", Track ID: " << hit->GetTrackID()
                      << ", PDG Code: " << hit->GetPDGCode()
                      << ", Particle Name: " << hit->GetParticleName()
                      //<< ", Chamber Number: " << hit->GetChamberNb()
                      << ", Energy Deposit: " << hit->GetEdep()
                      << ", Position: " << hit->GetPos() << G4endl;    
        */
        if( (hit->GetParticleName().find("Li") != std::string::npos) || (hit->GetParticleName().find("Nb") != std::string::npos) || (hit->GetParticleName().find("O") != std::string::npos) ) 
        {
          totalNR += hit->GetEdep();
        }         
        // Fill the analysis ntuple
          analysisManager->FillNtupleDColumn(1, 0, eventID);
          analysisManager->FillNtupleDColumn(1, 1, hit->GetTrackID());
          analysisManager->FillNtupleDColumn(1, 2, hit->GetPDGCode());
          analysisManager->FillNtupleSColumn(1, 3, hit->GetParticleName());
          analysisManager->FillNtupleSColumn(1, 4, hit->GetParticleDefinition());
          analysisManager->FillNtupleDColumn(1, 5, hit->GetEdep());
          analysisManager->FillNtupleDColumn(1, 6, hit->GetPos().x());
          analysisManager->FillNtupleDColumn(1, 7, hit->GetPos().y());
          analysisManager->FillNtupleDColumn(1, 8, hit->GetPos().z());
          analysisManager->FillNtupleDColumn(1, 9, hit->GetMomentum().x());
          analysisManager->FillNtupleDColumn(1, 10, hit->GetMomentum().y());
          analysisManager->FillNtupleDColumn(1, 11, hit->GetMomentum().z());
          analysisManager->FillNtupleDColumn(1, 12, hit->GetMomentumDirection().x());
          analysisManager->FillNtupleDColumn(1, 13, hit->GetMomentumDirection().y());
          analysisManager->FillNtupleDColumn(1, 14, hit->GetMomentumDirection().z());
          analysisManager->FillNtupleDColumn(1, 15, hit->GetTime());
          analysisManager->AddNtupleRow(1);
        }
    }
  }
  
  if (primaryParticle) {
    G4int trackID = primaryParticle->GetTrackID();
    G4int pdgCode = primaryParticle->GetPDGcode();
    G4double kineticEnergy = primaryParticle->GetKineticEnergy();
    //G4ThreeVector position = primaryParticle->GetPosition();
    G4ThreeVector momentumDirection = primaryParticle->GetMomentumDirection();
    analysisManager->FillNtupleDColumn(0, 0, eventID);
    analysisManager->FillNtupleDColumn(0, 1, trackID);
    analysisManager->FillNtupleDColumn(0, 2, pdgCode);
    analysisManager->FillNtupleDColumn(0, 3, kineticEnergy);
    analysisManager->FillNtupleDColumn(0, 4, primary_x);
    analysisManager->FillNtupleDColumn(0, 5, primary_y);
    analysisManager->FillNtupleDColumn(0, 6, primary_z);
    analysisManager->FillNtupleDColumn(0, 7, momentumDirection.x());
    analysisManager->FillNtupleDColumn(0, 8, momentumDirection.y());
    analysisManager->FillNtupleDColumn(0, 9, momentumDirection.z());
    analysisManager->FillNtupleDColumn(0, 10, totalNR);
    analysisManager->FillNtupleDColumn(0, 11, totalER);
    analysisManager->AddNtupleRow(0);
  }

  //add phonon hit information to the output trees
  collectionID = sdManager->GetCollectionID("PhononElectrode/G4CMPElectrodeHit");
  if (collectionID < 0) {
    G4cerr << "Phonon Hits collection not found, proceeding!" << G4endl;
    return;
  }

  G4CMPElectrodeHitsCollection* phononHC = 0;
  if (HCofEvent) {
      phononHC = static_cast<G4CMPElectrodeHitsCollection*>(HCofEvent->GetHC(collectionID));
  } 
  if (phononHC) {
    for (unsigned int i = 0; i < phononHC->entries(); ++i) {
      G4CMPElectrodeHit* hit = (*phononHC)[i];
      if (hit) {
          analysisManager->FillNtupleDColumn(2, 0, eventID);
          analysisManager->FillNtupleDColumn(2, 1, hit->GetTrackID());
          analysisManager->FillNtupleSColumn(2, 2, hit->GetParticleName());
          analysisManager->FillNtupleDColumn(2, 3, hit->GetStartEnergy());
          analysisManager->FillNtupleDColumn(2, 4, hit->GetStartPosition().x());
          analysisManager->FillNtupleDColumn(2, 5, hit->GetStartPosition().y());
          analysisManager->FillNtupleDColumn(2, 6, hit->GetStartPosition().z());
          analysisManager->FillNtupleDColumn(2, 7, hit->GetStartTime());
          analysisManager->FillNtupleDColumn(2, 8, hit->GetEnergyDeposit()/eV);
          analysisManager->FillNtupleDColumn(2, 9, hit->GetWeight());
          analysisManager->FillNtupleDColumn(2, 10, hit->GetFinalPosition().x());
          analysisManager->FillNtupleDColumn(2, 11, hit->GetFinalPosition().y());
          analysisManager->FillNtupleDColumn(2, 12, hit->GetFinalPosition().z());
          analysisManager->FillNtupleDColumn(2, 13, hit->GetFinalTime());
          analysisManager->AddNtupleRow(2);
      }
    }
  }



  fOutputFile.close(); 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
