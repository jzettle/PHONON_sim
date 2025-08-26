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
#include "PhononSensitivity.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4SubtractionSolid.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4GDMLParser.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "G4CMPLogicalBorderSurface.hh"
#include "G4CMPPhononElectrode.hh"
#include "G4CMPSurfaceProperty.hh"
#include "G4LatticeLogical.hh"
#include "G4LatticeManager.hh"
#include "G4LatticePhysical.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* PHONONDetectorConstruction::fMagFieldMessenger = 0;
 
PHONONDetectorConstruction::PHONONDetectorConstruction()
:G4VUserDetectorConstruction(),
 fLogicTarget(NULL), fLogicChamber(NULL), 
 fPbShieldMaterial(NULL), fWaterShieldMaterial(NULL), topSurfProp(0),
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
  delete topSurfProp;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* PHONONDetectorConstruction::Construct()
{

  // Have to completely remove all lattices to avoid warning on reconstruction
  G4LatticeManager::GetLatticeManager()->Reset();
  // Clear all LogicalSurfaces
  // NOTE: No need to redefine the G4CMPSurfaceProperties
  G4CMPLogicalBorderSurface::CleanSurfaceTable();

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

  fVacuumMaterial = nistManager->FindOrBuildMaterial("G4_Galactic");
  
  // Lead defined using NIST Manager
  fPbShieldMaterial = nistManager->FindOrBuildMaterial("G4_Pb");

  fWaterShieldMaterial = nistManager->FindOrBuildMaterial("G4_WATER");


  G4Element* elH = new G4Element("Hydrogen","H",1.,1.01*g/mole);
  G4Element* elC = new G4Element("Carbon","C",6.,12.01*g/mole);
  //double density = 0.874*g/cm3;
  //fChamberMaterial = new G4Material("EJ301",0.874*g/cm3,2);
  //fChamberMaterial->AddElement(elH, 303);
  //fChamberMaterial->AddElement(elC, 250);
  G4Element* elLi  = new G4Element("Li","Li", 3., 6.941*g/mole);
  G4Element* elO   = new G4Element("O","O" , 8., 16.*g/mole);
  G4Element* elNb  = new G4Element("Nb","Nb", 41., 92.9064*g/mole);
  G4Material* LN = new G4Material("LN", 4.65*g/cm3, 3);
  LN->AddElement(elLi, 1);
  LN->AddElement(elNb, 1);
  LN->AddElement(elO, 3);

  fChamberMaterial = LN; // Use LN as the chamber material
  //fChamberMaterial = nistManager->FindOrBuildMaterial("G4_Xe");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* PHONONDetectorConstruction::DefineVolumes()
{
  G4Material* air  = G4Material::GetMaterial("G4_AIR");

  // Sizes of the principal geometrical components (solids)

  G4double worldLength = 50*m; // World size

  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  G4GDMLParser parser;
  //G4String gdml_file = "gdml/scintillator_shield/ScintillatorShield.gdml";
  G4cout << "Reading GDML file at:" << fGDMLFile << G4endl;
  G4cout << "GDML: Reading World!" << G4endl;
  parser.Read(fGDMLFile.c_str());
  G4VPhysicalVolume* worldPhys = parser.GetWorldVolume();

  G4LogicalVolumeStore *logvolstore = G4LogicalVolumeStore::GetInstance();
  fLogicChamber = logvolstore->GetVolume("ScintLog");
  fSensorLogic = logvolstore->GetVolume("SensorLog");

  G4PhysicalVolumeStore *physvolstore = G4PhysicalVolumeStore::GetInstance();
  fScintPhys = physvolstore->GetVolume("ScintillatorVol");
  fAirPhys = physvolstore->GetVolume("AirVol");
  fSensorPhys = physvolstore->GetVolume("SensorVol");

  return worldPhys;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PHONONDetectorConstruction::ConstructSDandField()
{

  G4LatticeManager* LM = G4LatticeManager::GetLatticeManager();
  //G4LatticeLogical* GeLogical = LM->LoadLattice(fChamberMaterial, "Ge");
  G4LatticeLogical* NbLogical = LM->LoadLattice(fChamberMaterial, "Si");

  // G4LatticePhysical assigns G4LatticeLogical a physical orientation
  G4LatticePhysical* NbPhysical = new G4LatticePhysical(NbLogical);
  NbPhysical->SetMillerOrientation(1,0,0);
  LM->RegisterLattice(fScintPhys, NbPhysical);
  LM->RegisterLattice(fSensorPhys, NbPhysical);

  // Sensitive detectors

  G4String scintChamberSDname = "ScintChamberSD";
  PHONONScintSD* aScintSD = new PHONONScintSD(scintChamberSDname,
                                            "ScintHitsCollection");
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(aScintSD);
  fLogicChamber->SetSensitiveDetector(aScintSD);
  //SetSensitiveDetector("ScintLog",  aScintSD, true);
  //SetSensitiveDetector(fLogicChamber,  aScintSD);

  if (!electrodeSensitivity)
    electrodeSensitivity = new PhononSensitivity("PhononElectrode");
  SDman->AddNewDetector(electrodeSensitivity);
  //SetSensitiveDetector("ScintLog",  electrodeSensitivity, true);
  //SetSensitiveDetector(fSensorLogic,  electrodeSensitivity);
  //fLogicChamber->SetSensitiveDetector(electrodeSensitivity);
  fSensorLogic->SetSensitiveDetector(electrodeSensitivity);

  const G4double GHz = 1e9 * hertz; 

  //the following coefficients and cutoff values are not well-motivated
  //the code below is used only to demonstrate how to set these values.
  const std::vector<G4double> anhCoeffs = {0, 0, 0, 0, 0, 1.51e-14};
  const std::vector<G4double> diffCoeffs =
      {5.88e-2, 7.83e-4, -2.47e-6, 1.71e-8, -2.98e-11};
  const std::vector<G4double> specCoeffs =
      {0,928, -2.03e-4, -3.21e-6, 3.1e-9, 2.9e-13};

  const G4double anhCutoff = 520., reflCutoff = 350.;   // Units external

  //topSurfProp = new G4CMPSurfaceProperty("TopAlSurf", 1.0, 0.0, 0.0, 0.0,
					  	        //0.3, 1.0, 0.0, 0.0);
  topSurfProp = new G4CMPSurfaceProperty("TopAlSurf", 1.0, 0.0, 0.0, 0.0,
					  	        1.0, 1.0, 0.0, 0.0);
  //topSurfProp->AddScatteringProperties(anhCutoff, reflCutoff, anhCoeffs,
					 //diffCoeffs, specCoeffs, GHz, GHz, GHz);
  AttachPhononSensor(topSurfProp);
  
  new G4CMPLogicalBorderSurface("SubstrateSensor", fScintPhys, fSensorPhys,
				topSurfProp);
  new G4CMPLogicalBorderSurface("SensorSubstrate", fSensorPhys, fScintPhys,
				topSurfProp);
  
  wallSurfProp = new G4CMPSurfaceProperty("WallSurf", 0.0, 1.0, 0.0, 0.0,
					    	        1.0, 1.0, 0.0, 0.0);
  new G4CMPLogicalBorderSurface("SubstrateBorder", fAirPhys, fScintPhys,
				wallSurfProp);
  new G4CMPLogicalBorderSurface("BorderSubstrate", fScintPhys, fAirPhys,
				wallSurfProp);
  
  new G4CMPLogicalBorderSurface("SubstrateBorder", fAirPhys, fSensorPhys,
				wallSurfProp);
  new G4CMPLogicalBorderSurface("BorderSubstrate", fSensorPhys, fAirPhys,
				wallSurfProp);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHONONDetectorConstruction::SetGDMLFile(G4String filename)
{
  G4cout << "Setting GDML file to: " << filename << G4endl;
  fGDMLFile = filename;
}

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

void PHONONDetectorConstruction::
AttachPhononSensor(G4CMPSurfaceProperty *surfProp) {
  if (!surfProp) return;		// No surface, nothing to do

  // Specify properties of aluminum sensor, same on both detector faces
  // See G4CMPPhononElectrode.hh or README.md for property keys

  // Properties must be added to existing surface-property table
  auto sensorProp = surfProp->GetPhononMaterialPropertiesTablePointer();
  sensorProp->AddConstProperty("filmAbsorption", 0.20);    // True sensor area
  sensorProp->AddConstProperty("filmThickness", 600.*nm);
  sensorProp->AddConstProperty("gapEnergy", 173.715e-6*eV);
  sensorProp->AddConstProperty("lowQPLimit", 3.);
  sensorProp->AddConstProperty("phononLifetime", 242.*ps);
  sensorProp->AddConstProperty("phononLifetimeSlope", 0.29);
  sensorProp->AddConstProperty("vSound", 3.26*km/s);
  sensorProp->AddConstProperty("subgapAbsorption", 0.1);

  // Attach electrode object to handle KaplanQP interface
  surfProp->SetPhononElectrode(new G4CMPPhononElectrode);
}
  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
