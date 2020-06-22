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
/// \file EDScreenSD.cc
/// \brief Implementation of the EDScreenSD class
//

#include "EDScreenSD.hh"
#include "EDAnalysis.hh"

#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDScreenSD::EDScreenSD(const G4String& name, G4int ntupleId)
 : G4VSensitiveDetector(name),
   fNtupleId(ntupleId),
   fIdxInitialized(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDScreenSD::~EDScreenSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDScreenSD::Initialize(G4HCofThisEvent* /*hce*/)
{

  if ( !fIdxInitialized )
    {
      // -- Model index put for information (not used).
      // -- See examples/extended/electromagnetic/TestEm5 for more details
      fPhotoGamma = G4PhysicsModelCatalog::GetIndex("phot_fluo");   // fluorescence from photoelectric
      fComptGamma = G4PhysicsModelCatalog::GetIndex("compt_fluo");  // fluorescence from compton
      fPhotoAuger = G4PhysicsModelCatalog::GetIndex("phot_auger");  // auger e- from photoelectric
      fComptAuger = G4PhysicsModelCatalog::GetIndex("compt_auger"); // auger e- from compton
      fPixeGamma  = G4PhysicsModelCatalog::GetIndex("gammaPIXE");   // gamma from PIXE
      fPixeAuger  = G4PhysicsModelCatalog::GetIndex("e-PIXE");      // e- from PIXE

      G4cout << " +------------------ model indeces --------------+ " << G4endl;
      G4cout << "    phot_fluo   : " << fPhotoGamma << G4endl;
      G4cout << "    compt_fluo  : " << fComptGamma << G4endl;
      G4cout << "    phot_auger  : " << fPhotoAuger << G4endl;
      G4cout << "    compt_auger : " << fComptAuger << G4endl;
      G4cout << "    gammaPIXE   : " << fPixeGamma  << G4endl;
      G4cout << "    e-PIXE      : " << fPixeAuger  << G4endl;
      G4cout << " +-----------------------------------------------+ " << G4endl;
      
      // -- Print the full model catalog:
      G4cout << " Physics Model Catalog : " << G4endl;
      for ( G4int iEntry = 0 ; iEntry < G4PhysicsModelCatalog::Entries() ; iEntry++ )
	G4cout << " idx = " 
	       << iEntry << " "
	       <<  G4PhysicsModelCatalog::GetModelName(iEntry) << G4endl;

      fIdxInitialized = true;
    }



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool EDScreenSD::ProcessHits(G4Step* step, 
			       G4TouchableHistory* /*history*/)
{

  // Current track:
  const G4Track* track = step->GetTrack();
  // Track ID:
  G4int ID = track->GetTrackID();
  // code PDG:
  G4int pdgCode = track->GetDefinition()->GetPDGEncoding();

  // Remember preStepPoint:
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  // Obtain local coordinates:
  const G4VTouchable* touchable = preStepPoint->GetTouchable();
  G4ThreeVector globalPosition = preStepPoint->GetPosition();
  G4ThreeVector localPosition 
    = touchable->GetHistory()->GetTopTransform().TransformPoint(globalPosition);
  // // Example for obtaining the local direction:
  // G4ThreeVector globalDirection = preStepPoint->GetMomentumDirection();
  // G4ThreeVector localDirection 
  //   = touchable->GetHistory()->GetTopTransform().TransformAxis(localDirection);
  // Ekin:
  G4double Ekin =  preStepPoint->GetKineticEnergy();
  // Time
  G4double time = preStepPoint->GetGlobalTime();

  // creator model, provided only for:
  // - EM if gamma or e- are created as a results of PIXE or fluorescence
  // - hadronics for stopping processes. 
  // it is -1 otherwise.
  //  G4int creatorModelID = track->GetCreatorModelID();
  
  // Store hit in the ntuple
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(fNtupleId, 0, ID);
  analysisManager->FillNtupleIColumn(fNtupleId, 1, pdgCode);
  analysisManager->FillNtupleDColumn(fNtupleId, 2, Ekin/MeV);
  analysisManager->FillNtupleDColumn(fNtupleId, 3, localPosition.x()/cm);
  analysisManager->FillNtupleDColumn(fNtupleId, 4, localPosition.y()/cm);
  analysisManager->FillNtupleDColumn(fNtupleId, 5, time/ns);
  analysisManager->AddNtupleRow(fNtupleId);  

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDScreenSD::EndOfEvent(G4HCofThisEvent* /*hce*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
