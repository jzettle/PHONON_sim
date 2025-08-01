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
/// \file B2RunAction.cc
/// \brief Implementation of the B2RunAction class

#include "PHONONRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHONONRunAction::PHONONRunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each 1000 events
  G4RunManager::GetRunManager()->SetPrintProgress(100);     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHONONRunAction::~PHONONRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHONONRunAction::BeginOfRunAction(const G4Run*)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  G4cout << "Opening Analysis Manager for run." << G4endl;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("no_shield_10kneutrons.root");
  //G4cout << "Analysis Manager created." << G4endl;
  //analysisManager->SetVerboseLevel(1);
  //analysisManager->SetNtupleMerging(true);  // Required for parallel run (multithreading)
  //create ntuple for primary particles and sensitive detector hits
  analysisManager->CreateNtuple("Primary", "Primary Particle Information"); 
  analysisManager->CreateNtupleDColumn("EventID");
  analysisManager->CreateNtupleDColumn("TrackID");
  analysisManager->CreateNtupleDColumn("PDGCode");
  analysisManager->CreateNtupleDColumn("PrimaryEnergy");
  analysisManager->CreateNtupleDColumn("PrimaryPosX");
  analysisManager->CreateNtupleDColumn("PrimaryPosY");
  analysisManager->CreateNtupleDColumn("PrimaryPosZ");
  analysisManager->CreateNtupleDColumn("PrimaryMomentumX");
  analysisManager->CreateNtupleDColumn("PrimaryMomentumY");
  analysisManager->CreateNtupleDColumn("PrimaryMomentumZ");
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("Hits", "Hit Data");
  analysisManager->CreateNtupleDColumn("EventID");
  analysisManager->CreateNtupleDColumn("TrackID");
  analysisManager->CreateNtupleDColumn("PDGCode");
  analysisManager->CreateNtupleSColumn("ParticleName");
  analysisManager->CreateNtupleSColumn("ParticleDefinition");
  analysisManager->CreateNtupleDColumn("Edep");
  analysisManager->CreateNtupleDColumn("PositionX");
  analysisManager->CreateNtupleDColumn("PositionY");
  analysisManager->CreateNtupleDColumn("PositionZ");
  analysisManager->CreateNtupleDColumn("HitTime");
  analysisManager->FinishNtuple();
  G4cout << "Analysis Manager created." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHONONRunAction::EndOfRunAction(const G4Run* )
{
  // Actions to perform at the end of the run
  // For example, close output files or finalize statistics
  G4cout << "Run ended successfully." << G4endl;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (!analysisManager->IsOpenFile()) {
    G4cerr << "Error: Analysis manager is not open." << G4endl;
    return;
  }
  analysisManager->Write();
  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
