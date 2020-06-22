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

#include "G4LogicalVolumeStore.hh"
#include "Randomize.hh"
#include "EDBiasingOperationSplitAndKill.hh"


EDBiasingOperationSplitAndKill::EDBiasingOperationSplitAndKill(G4String name)
  : G4VBiasingOperation(name),
    fParticleChange(),
    fParticleChangeForNothing()
{
  // Remember volume in which biasing occurs:
  fShieldSlice = G4LogicalVolumeStore::GetInstance()->GetVolume("ShieldSlice");
}

EDBiasingOperationSplitAndKill::~EDBiasingOperationSplitAndKill()
{}


G4double EDBiasingOperationSplitAndKill::DistanceToApplyOperation( const G4Track*,
								   G4double,
								   G4ForceCondition* condition)
{
  // -- return "infinite" distance for interaction, but asks for GenerateBiasingFinalState
  // -- being called anyway, returning the "Forced" flag.
  *condition = Forced;
  return DBL_MAX;
}



G4VParticleChange*  EDBiasingOperationSplitAndKill::GenerateBiasingFinalState( const G4Track* track,
									       const G4Step*  step )
{
  
  // Check if step is limited by the geometry:
  // and check we're not exiting from the shield (ie, PostStepPoint being outside of shield):
  //
  // (check of current step # of track is inelegant, but is to fix a "feature"
  // that a cloned track can wrongly be seen in the wrong volume, because of numerical
  // precision issue. In this case it makes a tiny step, which should be disregarded).
  if ( ( step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary ) &&
       ( step->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume() == fShieldSlice ) &&
       ( track->GetCurrentStepNumber() != 1 ) )
    {
      // Get the copy number of volumes, at pre and post step points:
      G4int preCopyNo  =  step->GetPreStepPoint() ->GetTouchable()->GetReplicaNumber();
      G4int postCopyNo =  step->GetPostStepPoint()->GetTouchable()->GetReplicaNumber();
      
      if ( postCopyNo > preCopyNo )
	{
	  // --------------------------------------------------------------------------------------------------
	  // Splitting: Here, we are moving "forward", ie slice numbers go as i -> i+1. We split by a factor 2:
	  // --------------------------------------------------------------------------------------------------
	  
	  // Get track weight:
	  G4double initialWeight = track->GetWeight();
	  
	  // The "particle change" is the object to be used to communicate to the
	  // tracking the update of the primary state and/or creation secondary tracks.
	  fParticleChange.Initialize(*track);
	  
	  // make currect track weight to be divded by two:
	  fParticleChange.ProposeParentWeight( initialWeight/2.0 );
	  
	  // now make a clone of this track (this is the actual splitting): we will then
	  // have the primary and a clone of it, hence the splitting by a factor two.
	  fParticleChange.SetSecondaryWeightByProcess(true); // -- tricky : take it as is ;)
	  // we allocate for one secondary creation:
	  fParticleChange.SetNumberOfSecondaries(1);
	  // we create a copy of the primary track:
	  G4Track* clone = new G4Track( *track );
	  // we give it the proper weight:
	  clone->SetWeight(  initialWeight/2.0 );
	  // and finally pass it to the "particle change":
	  fParticleChange.AddSecondary( clone );
	  
	  // this new final state is returned to the tracking;
	  return &fParticleChange;
	  
	}

      //      else if ( postCopyNo < preCopyNo )
      //
      //      	{
      //      	  // --------------------------------------------------------------------------------
      //      	  // Killing: Here, we are moving backward ( i+1 -> i ) : we make a russian roulette:
      //      	  // --------------------------------------------------------------------------------
      //      	  // The exercise here:
      //      	  //     - decide to kill with 50% probability (russian roulette)
      //      	  //     - if track survives, make its weight to be multiplied by 2
      //      	  //     - else makes it to be killed
      //          ***** implement a russian roulette here *****
      //      	}
      //      else
      //      	{
      //      	  // postCopyNo == preCopyNo : should not happen
      //      	  G4cout << " !!!!!! Should not happen !!!!!!! " << G4endl;
      //      	}

    }

  // Here, the step was not limited by the geometry (but certainly by a physics process).
  // We do nothing: ie we make no change to the current track.
  fParticleChangeForNothing.Initialize(*track);
  return &fParticleChangeForNothing;
  
}
