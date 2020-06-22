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
/// \file EDRunAction.cc
/// \brief Implementation of the EDRunAction class

#include "EDRunAction.hh"
#include "EDAnalysis.hh"

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDRunAction::EDRunAction()
: G4UserRunAction()
{
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in B4Analysis.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  G4cout << "Using " << analysisManager->GetType() 
         << " analysis manager" << G4endl;

  // Creating histograms
  //
  for ( G4int i=0; i<10; ++i) {
    std::ostringstream os;
    os << i;
    G4String hname = "Layer";
    hname += os.str();
    G4String htitle = "Edep [MeV] in layer ";
    htitle += os.str();
    htitle += " in EmCalorimeter"; 
    // -- make histogram wider for this exercise:
    analysisManager->CreateH1(hname, htitle, 150, 0., 1500.);
  }
  // histogram 10 
  analysisManager->CreateH1("AllCharged", "Charged Edep [MeV] in all layers" , 
                            150, 0., 1500);
  // histogram 11 
  analysisManager->CreateH1("AllNeutral", "Neutral Edep [MeV] in all layers" , 
                            100, 0., 100);
  // histogram 12 
  analysisManager->CreateH1("EdepPrimary", "Edep [MeV] by primary in calorimeter" , 
                            150, 0., 1500);
  // histogram 13 
  analysisManager->CreateH1("EdepOthers", "Edep [MeV] by non-primary in calorimeter" , 
                            150, 0., 1500);


  // Creating ntuples
  // ntuple id = 0
  analysisManager->CreateNtuple("Chamber1", "Chamber 1 hits");
  analysisManager->CreateNtupleIColumn("Layer");   // column id = 0 
  analysisManager->CreateNtupleDColumn("Xpos");    // column id = 1 
  analysisManager->CreateNtupleDColumn("Ypos");    // column id = 2
  analysisManager->CreateNtupleDColumn("Zpos");    // column id = 3 
  analysisManager->FinishNtuple();
  // ntuple id = 1
  analysisManager->CreateNtuple("Chamber2", "Chamber 2 hits");
  analysisManager->CreateNtupleIColumn("Layer");   // column id = 0 
  analysisManager->CreateNtupleDColumn("Xpos");    // column id = 1 
  analysisManager->CreateNtupleDColumn("Ypos");    // column id = 2
  analysisManager->CreateNtupleDColumn("Zpos");    // column id = 3 
  analysisManager->FinishNtuple();
  // ntuple id = 2
  analysisManager->CreateNtuple("Screen", "Screen hits");
  analysisManager->CreateNtupleIColumn("ID");      // column id = 0
  analysisManager->CreateNtupleIColumn("PDG");     // column id = 1
  analysisManager->CreateNtupleDColumn("Ekin");    // column id = 2
  analysisManager->CreateNtupleDColumn("Xpos");    // column id = 3 
  analysisManager->CreateNtupleDColumn("Ypos");    // column id = 4
  analysisManager->CreateNtupleDColumn("time");    // column id = 5
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDRunAction::~EDRunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  // Open an output file
  //
  G4String fileName = "ED";
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDRunAction::EndOfRunAction(const G4Run* /*run*/)
{  
  // save histograms 
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
