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
/// \file B2TrackerHit.hh
/// \brief Definition of the B2TrackerHit class

#ifndef PHONONScintHit_h
#define PHONONScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

#include <fstream>

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class PHONONScintHit : public G4VHit
{
  public:
   PHONONScintHit();
    PHONONScintHit(const PHONONScintHit&);
    virtual ~PHONONScintHit();

    // operators
    const PHONONScintHit& operator=(const PHONONScintHit&);
    G4bool operator==(const PHONONScintHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetPDGCode  (G4int pdg)        { fPDGCode = pdg; };
    //void SetChamberNb(G4int chamb)      { fChamberNb = chamb; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetParticleName(const G4String& name) { fParticleName = name; };
    void SetParticleDefinition(const G4String& def) { fParticleDefinition = def; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetPDGCode() const     { return fPDGCode; };
    G4String GetParticleName() const { return fParticleName; };
    G4String GetParticleDefinition() const { return fParticleDefinition; };
    //G4int GetChamberNb() const   { return fChamberNb; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    G4double GetTime() const { return fTime; }
    void SetTime(G4double time) { fTime = time; }

  private:

      G4int         fTrackID;
      G4int         fPDGCode; // Particle PDG code, initialized to -1000
      G4String      fParticleName; // Particle name, initialized to empty string
      G4String      fParticleDefinition; // Particle definition, initialized to empty string
      //G4int         fChamberNb;
      G4double      fEdep;
      G4ThreeVector fPos;
      G4double      fTime; // Time of hit, initialized to -1000
      std::ofstream fOutputFile; // Output file for hit collection information
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<PHONONScintHit> PHONONScintHitsCollection;

extern G4ThreadLocal G4Allocator<PHONONScintHit>* PHONONScintHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* PHONONScintHit::operator new(size_t)
{
  if(!PHONONScintHitAllocator)
      PHONONScintHitAllocator = new G4Allocator<PHONONScintHit>;
  return (void *) PHONONScintHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PHONONScintHit::operator delete(void *hit)
{
  PHONONScintHitAllocator->FreeSingle((PHONONScintHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
