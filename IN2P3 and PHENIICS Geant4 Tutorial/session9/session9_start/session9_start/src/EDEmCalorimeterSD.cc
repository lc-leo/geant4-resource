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
// $Id$
//
/// \file EDEmCalorimeterSD.cc
/// \brief Implementation of the EDEmCalorimeterSD class
//

#include "EDEmCalorimeterSD.hh"
#include "EDAnalysis.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDEmCalorimeterSD::EDEmCalorimeterSD(const G4String& name, 
                                     const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(0)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDEmCalorimeterSD::~EDEmCalorimeterSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDEmCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection 
    = new EDEmCalorimeterHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  for (G4int i=0; i<14; ++i) {
    EDEmCalorimeterHit* newHit = new EDEmCalorimeterHit();
    newHit->SetLayerNumber(i);
    fHitsCollection->insert(newHit);
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool EDEmCalorimeterSD::ProcessHits(G4Step* step,
                                      G4TouchableHistory* /*history*/)
{
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  if ( edep==0. ) return false;     
  
  const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();
  G4VPhysicalVolume* layerPV = touchable->GetVolume(1);
  G4int layerNumber = layerPV->GetCopyNo();
  
  // Get hit accounting data for this layer
  EDEmCalorimeterHit* hit = (*fHitsCollection)[layerNumber];
  if ( ! hit ) {
    G4cerr << "Cannot access hit " << layerNumber << G4endl;
    exit(1);
  }         
  
  // Add values
  hit->AddEdep(edep);
  
  // Update hit accounting data for all layers either for charged or
  // for neutral particles
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.0 ) {
    EDEmCalorimeterHit* hitCharged = (*fHitsCollection)[10];
    // Add values
    hitCharged->AddEdep(edep);
  }
  else {
    EDEmCalorimeterHit* hitNeutral = (*fHitsCollection)[11];
    // Add values
    hitNeutral->AddEdep(edep);
  }

  // Update hit depending if track is primary or not:
  if ( step->GetTrack()->GetParentID() == 0 ) {
    EDEmCalorimeterHit* hitPrimary = (*fHitsCollection)[12];
    // Add values
    hitPrimary->AddEdep(edep);
  }
  else {
    EDEmCalorimeterHit* hitOthers = (*fHitsCollection)[13];
    // Add values
    hitOthers->AddEdep(edep);
  }

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDEmCalorimeterSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
  // Suppress heavy print:
  //  G4cout << "\n-------->" <<  fHitsCollection->GetName() 
  //         << ": in this event: " << G4endl;

  // G4int nofHits = fHitsCollection->entries();
  // suppress print for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  
  // Loop over hits
  // and fill each non-zero value in appropriate histogram
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  for ( G4int i=0; i<10; i++ )  {
    EDEmCalorimeterHit* hit = (EDEmCalorimeterHit*)(*fHitsCollection)[i];
    G4double edep = hit->GetEdep();
    if ( edep > 0. ) analysisManager->FillH1(i, edep/MeV);
  }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
