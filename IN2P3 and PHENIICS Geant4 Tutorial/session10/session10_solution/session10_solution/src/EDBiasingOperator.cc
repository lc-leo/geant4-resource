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
// $Id: EDBiasingOperator.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file EDBiasingOperator.cc
/// \brief Implementation of the EDBiasingOperator class


#include "EDBiasingOperator.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDBiasingOperator::EDBiasingOperator()
 : G4VBiasingOperator("EDBiasingOperator")
{
  fSplitAndKillOperation = new EDBiasingOperationSplitAndKill("splitter");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDBiasingOperator::~EDBiasingOperator()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation*
EDBiasingOperator::ProposeNonPhysicsBiasingOperation(const G4Track*,
						     const G4BiasingProcessInterface*)
{
  // Here, we always return the split and kill biasing operation:
  return fSplitAndKillOperation;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation* 
EDBiasingOperator::ProposeOccurenceBiasingOperation(const G4Track*, 
						    const G4BiasingProcessInterface*)
{
  return 0;
}

