//
// $Id: PrimaryGeneratorAction.hh 127 2008-07-03 12:41:24Z jacquem $
//
// PrimaryGeneratorAction.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;
class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
   ~PrimaryGeneratorAction();

  public:  
    void GeneratePrimaries(G4Event*);
         
    G4ParticleGun* GetParticleGun() {return m_particleGun;}
    
  private:
    G4ParticleGun*             m_particleGun;
    DetectorConstruction*      m_detector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


