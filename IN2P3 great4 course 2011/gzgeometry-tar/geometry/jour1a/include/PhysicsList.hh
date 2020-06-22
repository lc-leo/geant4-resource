//
// $Id: PhysicsList.hh 38 2008-05-06 08:29:16Z jacquem $
//
// PhysicsList.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VUserPhysicsList
{
  public:
    PhysicsList();
   ~PhysicsList();

  protected:
    void ConstructParticle();
    void ConstructProcess();
 
    void SetCuts();

  protected:
    void ConstructEM();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

 
