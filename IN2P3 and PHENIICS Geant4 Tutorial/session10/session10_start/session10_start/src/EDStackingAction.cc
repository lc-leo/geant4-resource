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
// $Id: EDStackingAction.cc 68058 2013-03-13 14:47:43Z gcosmo $
// 
/// \file EDStackingAction.cc
/// \brief Implementation of the EDStackingAction class

#include "EDStackingAction.hh"

#include "G4Track.hh"
#include "G4GenericMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDStackingAction::EDStackingAction()
 : G4UserStackingAction(),
   fMessenger(0),
   fKillSecondaries(false)
{ 
// Add code to to define /B4/stacking/killSecondaries commands 
// using generic messenger class

  // Generic messenger
  // Define /B4/event commands using generic messenger class
  fMessenger 
    = new G4GenericMessenger(this, "/ED/stacking/", "Stacking control");

  // Define /B4/event/setPrintModulo command
  fMessenger
    ->DeclareProperty("killSecondaries", 
                      fKillSecondaries, 
                      "Activate/Inactivate killing all secondary particles");
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDStackingAction::~EDStackingAction()
{ 
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
EDStackingAction::ClassifyNewTrack(const G4Track* track)
{
  // Implement code to kill all secondary particles
  // if the option for killing secondaries is activated
  if ( fKillSecondaries && track->GetParentID() > 0 ) {
    return fKill;
  }
  else {
    return fUrgent;
  }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
