//
// $Id: jour1a.cc 37 2008-05-06 08:25:51Z jacquem $
//
//main.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

#include "SteppingVerbose.hh"

#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
int main(int argc,char** argv) {
 
  //my Verbose output class
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);
    
  //Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  //set mandatory initialization classes
  //
  DetectorConstruction* detector  = new DetectorConstruction();
  runManager->SetUserInitialization(detector);
    
  PhysicsList* physics            = new PhysicsList();
  runManager->SetUserInitialization(physics);
    
  PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(detector);
  runManager->SetUserAction(primary);

  //Initialize Geant4 kernel
  //
  runManager->Initialize();

  //get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();  

  if (argc!=1) {   	// Batch mode 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }
  
  else {          	// Define UI terminal for interactive mode
         
#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif
     
    G4UIsession* session = 0;
#ifdef G4UI_USE_TCSH
    session = new G4UIterminal(new G4UItcsh);      
#else
    session = new G4UIterminal();
#endif
    UI->ApplyCommand( "/control/execute vis.mac");                      
    session->SessionStart();
    delete session;
    
#ifdef G4VIS_USE
    delete visManager;
#endif    
  }

 //job termination
 //
 delete runManager;
 return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
