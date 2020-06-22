//
// $Id: DetectorConstruction.hh 127 2008-07-03 12:41:24Z jacquem $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

  public:
     	  	            
     G4VPhysicalVolume* Construct();
     
  public:  
                             
     G4Material*  GetSpacecraftMaterial() {return m_spacecraftMaterial;};
     G4Material*  GetCabinMaterial()      {return m_cabinMaterial;};     
     G4Material*  GetFloorMaterial()      {return m_floorMaterial;};
     G4Material*  GetAstronautMaterial()  {return m_astronautMaterial;};
     
     G4double     GetWorldSizeXY()        {return m_worldSizeXY;};
     G4double     GetWorldSizeZ()         {return m_worldSizeZ;};
  
     void PrintParameters();
             
  private:
    
     void               DefineMaterials();
     G4VPhysicalVolume* ConstructVolumes();
                                 
  private:
     
     G4Material*  m_worldMaterial;
     G4Material*  m_spacecraftMaterial;
     G4Material*  m_cabinMaterial;     
     G4Material*  m_floorMaterial;
     G4Material*  m_astronautMaterial;     
     
     G4VPhysicalVolume*    m_physiWorld;     
     G4double m_worldSizeXY, m_worldSizeZ;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
