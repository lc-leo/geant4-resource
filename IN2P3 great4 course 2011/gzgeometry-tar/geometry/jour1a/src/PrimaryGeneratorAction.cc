//
// $Id: PrimaryGeneratorAction.cc 126 2008-07-03 12:24:17Z jacquem $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
:m_detector(det)					       
{
  m_particleGun  = new G4ParticleGun(1);
  G4ParticleDefinition* particle
           = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
  m_particleGun->SetParticleDefinition(particle);
  m_particleGun->SetParticleEnergy(10*eV);
  m_particleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));     
  m_particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete m_particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  //
  m_particleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

