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
/// \file B2bDetectorConstruction.cc
/// \brief Implementation of the B2bDetectorConstruction class
 
#include "PHONONDetectorConstruction.hh"
#include "PHONONDetectorMessenger.hh"
#include "PHONONChamberParameterisation.hh"
#include "PHONONScintSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* PHONONDetectorConstruction::fMagFieldMessenger = 0;
 
PHONONDetectorConstruction::PHONONDetectorConstruction()
:G4VUserDetectorConstruction(),
 fLogicTarget(NULL), fLogicChamber(NULL), 
 fPbShieldMaterial(NULL), fWaterShieldMaterial(NULL),
 fChamberMaterial(NULL), fStepLimit(NULL), 
 fCheckOverlaps(true)
{
  fMessenger = new PHONONDetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PHONONDetectorConstruction::~PHONONDetectorConstruction()
{
  delete fStepLimit;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* PHONONDetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHONONDetectorConstruction::DefineMaterials()
{
  // Material definition 

  G4NistManager* nistManager = G4NistManager::Instance();

  // Air defined using NIST Manager
  nistManager->FindOrBuildMaterial("G4_AIR");
  
  // Lead defined using NIST Manager
  fPbShieldMaterial = nistManager->FindOrBuildMaterial("G4_Pb");

  fWaterShieldMaterial = nistManager->FindOrBuildMaterial("G4_WATER");

  // Xenon gas defined using NIST Manager

  G4Element* elH = new G4Element("Hydrogen","H",1.,1.01*g/mole);
  G4Element* elC = new G4Element("Carbon","C",6.,12.01*g/mole);
  //double density = 0.874*g/cm3;
  fChamberMaterial = new G4Material("EJ301",0.874*g/cm3,2);
  fChamberMaterial->AddElement(elH, 303);
  fChamberMaterial->AddElement(elC, 250);
  //fChamberMaterial = nistManager->FindOrBuildMaterial("G4_Xe");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* PHONONDetectorConstruction::DefineVolumes()
{
  G4Material* air  = G4Material::GetMaterial("G4_AIR");

  // Sizes of the principal geometrical components (solids)
  
  //G4int NbOfChambers = 5;
  //G4double chamberSpacing = 80*cm; // from chamber center to center!

  //G4double chamberWidth = 20.0*cm; // width of the chambers
  //G4double targetLength =  5.0*cm; // full length of Target
  
  //G4double trackerLength = (NbOfChambers+1)*chamberSpacing;

  //G4double worldLength = 1.2 * (2*targetLength + trackerLength);
  G4double worldLength = 10*m; // World size

  //G4double targetRadius  = 0.5*targetLength;   // Radius of Target
  //targetLength = 0.5*targetLength;             // Half length of the Target  
  //G4double trackerSize   = 0.5*trackerLength;  // Half length of the Tracker

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // World

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  G4Box* worldS 
    = new G4Box("world",                                    //its name
                worldLength/2,worldLength/2,worldLength/2); //its size
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,   //its solid
                 air,      //its material
                 "World"); //its name
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps 


  // Register the world volume in the auto-delete store
  //G4AutoDelete::Register(worldPV);

  double totalThickness = 100*cm; // Total thickness of the full shield structure
  double PbShieldThickness = 10*cm; // Thickness of the lead shield
  double WaterShieldThickness = 20*cm; // Thickness of the water shield
  double ScintThickness = totalThickness - (PbShieldThickness*2.0) - (WaterShieldThickness*2.0);
  // Lead shield
  G4Box* PbShieldS
     = new G4Box("PbShield", totalThickness/2, totalThickness/2, totalThickness/2);
  G4LogicalVolume* PbShieldLV
      = new G4LogicalVolume(PbShieldS, fPbShieldMaterial, "PbShield", 0, 0, 0);
  G4VPhysicalVolume* PbShieldPV
     = new G4PVPlacement(0,              // no rotation
                    G4ThreeVector(0, 0, PbShieldThickness/2), // at (x,y,z)
                    PbShieldLV,    // its logical volume
                    "PbShield",    // its name
                    worldLV,       // its mother volume
                    false,         // no boolean operations
                    0,             // copy number
                    fCheckOverlaps); // checking overlaps    
  
  G4Box* WaterShieldS
     = new G4Box("WaterShield", (totalThickness-WaterShieldThickness)/2, (totalThickness-WaterShieldThickness)/2, (totalThickness-WaterShieldThickness)/2);
  G4LogicalVolume* WaterShieldLV
      = new G4LogicalVolume(WaterShieldS, fWaterShieldMaterial, "WaterShield", 0, 0, 0);
  G4VPhysicalVolume* WaterShieldPV
     = new G4PVPlacement(0,              // no rotation
                    G4ThreeVector(0, 0, 0), // at (x,y,z)
                    WaterShieldLV,  // its logical volume
                    "WaterShield",  // its name   
                    PbShieldLV,       // its mother volume
                    false,         // no boolean operations
                    0,             // copy number
                    fCheckOverlaps); // checking overlaps 

  //Scintillator Chamber
  G4Box* chamberS
    = new G4Box("ScintChamber", 0.5*ScintThickness, 0.5*ScintThickness, 0.5*ScintThickness);
  fLogicChamber 
    = new G4LogicalVolume(chamberS, fChamberMaterial, "Chamber", 0, 0, 0);
  G4VPhysicalVolume* chamberPV
    = new G4PVPlacement(0,              // no rotation
                    G4ThreeVector(0, 0, 0), // at (x,y,z)
                    fLogicChamber, // its logical volume      
                    "Chamber",     // its name
                    WaterShieldLV, // its mother volume
                    false,         // no boolean operations
                    0,             // copy number
                    fCheckOverlaps); // checking overlaps

  G4double maxStep = 0.5*cm; // Maximum step size in the chamber
  fStepLimit = new G4UserLimits(maxStep);
  fLogicChamber->SetUserLimits(fStepLimit);

  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PHONONDetectorConstruction::ConstructSDandField()
{
  // Sensitive detectors

  G4String scintChamberSDname = "ScintChamberSD";
  PHONONScintSD* aScintSD = new PHONONScintSD(scintChamberSDname,
                                            "ScintHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aScintSD);
  SetSensitiveDetector( fLogicChamber,  aScintSD );

  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  //G4ThreeVector fieldValue = G4ThreeVector();
  //fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  //fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  //G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHONONDetectorConstruction::SetTargetMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial =
              nistManager->FindOrBuildMaterial(materialName);
  /*
  if (fTargetMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fTargetMaterial = pttoMaterial;
        if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
        G4cout
          << G4endl 
          << "----> The target is made of " << materialName << G4endl;
     } else {
        G4cout
          << G4endl 
          << "-->  WARNING from SetTargetMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
     */
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHONONDetectorConstruction::SetChamberMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  G4Material* pttoMaterial =
              nistManager->FindOrBuildMaterial(materialName);
  /*
  if (fChamberMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fChamberMaterial = pttoMaterial;
        if (fLogicChamber) fLogicChamber->SetMaterial(fChamberMaterial);
        G4cout
          << G4endl 
          << "----> The chambers are made of " << materialName << G4endl;
     } else {
        G4cout
          << G4endl
          << "-->  WARNING from SetChamberMaterial : "
          << materialName << " not found" << G4endl;
     }
  }
     */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PHONONDetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
