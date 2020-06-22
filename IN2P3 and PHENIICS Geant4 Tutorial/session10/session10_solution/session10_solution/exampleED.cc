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
/// \file exampleED.cc
/// \brief Main program of the ED example

#include "EDDetectorConstruction.hh"
#include "EDActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"
#include "G4GDMLParser.hh"
#include "G4GenericBiasingPhysics.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " exampleB4a [-m macro ] [-p physList ] [-u UIsession] [-g GDMLfile] [-b on/off (biasing on or offf)]" << G4endl;
    G4cerr << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Evaluate arguments
  //
  if ( argc > 9 ) {
    PrintUsage();
    return 1;
  }
  
  G4String macro;
  G4String session;
  G4String physicsListName;
  G4String gdmlFileName;
  G4String biasing = "on";
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
    else if ( G4String(argv[i]) == "-p" ) physicsListName = argv[i+1];
    else if ( G4String(argv[i]) == "-g" ) gdmlFileName = argv[i+1];
    else if ( G4String(argv[i]) == "-b" ) biasing = argv[i+1];
    else {
      PrintUsage();
      return 1;
    }
  }  

  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv, session);
  }
 
  // Construct the run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(2);
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new EDDetectorConstruction());

  // Physics list
  if ( physicsListName.size() == 0 ) physicsListName = "FTFP_BERT";
  G4PhysListFactory physListFactory;
  if ( ! physListFactory.IsReferencePhysList(physicsListName)) {
    G4cerr << " Physics list " << physicsListName 
           << " is not defined." << G4endl;
    return 1;
  }  
  
  G4VModularPhysicsList* physicsList 
    = physListFactory.GetReferencePhysList(physicsListName);
  physicsList->SetVerboseLevel(1);

  // modify the modular physics list to activate biasing:
  if ( biasing == "on" )
    {
      G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
      biasingPhysics->Bias("neutron");
      physicsList->RegisterPhysics(biasingPhysics);
    }

  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new EDActionInitialization());

  // Initialize G4 kernel
  //
  runManager->Initialize();
  
  // Export in GDML
  if ( gdmlFileName.size() ) {
    G4GDMLParser parser;
    G4LogicalVolume* worldLV
      = G4TransportationManager::GetTransportationManager()
          ->GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume();
    parser.Write(gdmlFileName, worldLV); 
  }  
  
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if ( macro.size() ) {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else {
    // interactive mode : define UI session
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
