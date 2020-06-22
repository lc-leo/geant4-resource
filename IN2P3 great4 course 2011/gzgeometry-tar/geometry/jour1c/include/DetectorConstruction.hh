//
// $Id: DetectorConstruction.hh 130 2008-07-03 13:15:35Z jacquem $
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
  
     void  SetWorldMaterial(G4String);                                  
     void  SetSpacecraftMaterial(G4String);
     void  SetCabinMaterial(G4String);     
     void  SetFloorMaterial(G4String);
     void  SetAstronautMaterial(G4String);
     
     void  SetWorldSizeXY(G4double val)         {m_worldSizeXY = val;};
     void  SetWorldSizeZ(G4double val)          {m_worldSizeZ = val;};
     void  SetSpacecraftRadius(G4double val)    {m_spacecraftRadius = val;};
     void  SetSpacecraftLength(G4double val)    {m_spacecraftLength = val;};
     void  SetSpacecraftThickness(G4double val) {m_spacecraftThickness = val;};
     void  SetCabinRadius(G4double val)         {m_cabinRadius = val;};
     void  SetCabinLength(G4double val)         {m_cabinLength = val;};
     void  SetFloorThickness(G4double val)      {m_floorThickness = val;};
     void  SetFloorWidth(G4double val)          {m_floorWidth = val;};
     void  SetFloorLength(G4double val)         {m_floorLength = val;};
     void  SetFloorYcenter(G4double val)        {m_floorYcenter = val;};
     void  SetAstronautHeight(G4double val)     {m_astronautHeight = val;};
     void  SetAstronautTopWidth(G4double val)   {m_astronautTopWidth = val;};
     void  SetAstronautBottomWidth(G4double val){m_astronautBottomWidth = val;};
     void  SetAstronautPosition(G4double val)   {m_astronautPosition = val;};
     	  	            
     G4VPhysicalVolume* Construct();
     void               UpdateGeometry();
          
  public:
    
     G4Material*  GetWorldMaterial()       {return m_worldMaterial;};                                  
     G4Material*  GetSpacecraftMaterial()  {return m_spacecraftMaterial;};
     G4Material*  GetCabinMaterial()       {return m_cabinMaterial;};     
     G4Material*  GetFloorMaterial()       {return m_floorMaterial;};
     G4Material*  GetAstronautMaterial()   {return m_astronautMaterial;};
     
     G4double     GetWorldSizeXY()         {return m_worldSizeXY;};
     G4double     GetWorldSizeZ()          {return m_worldSizeZ;};
     G4double     GetSpacecraftRadius()    {return m_spacecraftRadius;};            
     G4double     GetSpacecraftLength()    {return m_spacecraftLength;};    
     G4double     GetSpacecraftThickness() {return m_spacecraftThickness;};
     G4double     GetCabinRadius()         {return m_cabinRadius;};            
     G4double     GetCabinLength()         {return m_cabinLength;};         
     G4double     GetFloorThickness()      {return m_floorThickness;};     
     G4double     GetFloorWidth()          {return m_floorWidth;};     
     G4double     GetFloorLength()         {return m_floorLength;};
     G4double     GetFloorYcenter()        {return m_floorYcenter;};     
     G4double     GetAstronautHeight()     {return m_astronautHeight;};
     G4double     GetAstronautTopWidth()   {return m_astronautTopWidth;};
     G4double     GetAstronautBottomWidth(){return m_astronautBottomWidth;};
     G4double     GetAstronautPosition()   {return m_astronautPosition;};
            
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
          
     G4double m_worldSizeXY;
     G4double m_worldSizeZ;
     
     G4double m_spacecraftRadius;       
     G4double m_spacecraftLength;
     G4double m_spacecraftThickness;
  
     G4double m_cabinRadius;       
     G4double m_cabinLength;
  
     G4double m_floorThickness;
     G4double m_floorWidth;
     G4double m_floorLength;
     G4double m_floorYcenter;     

     G4double m_astronautHeight;
     G4double m_astronautTopWidth;
     G4double m_astronautBottomWidth;
     G4double m_astronautPosition;
     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
