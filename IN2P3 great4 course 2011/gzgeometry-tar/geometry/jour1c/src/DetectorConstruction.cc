//
// $Id: DetectorConstruction.cc 163 2008-09-15 15:44:56Z maire $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{  
  DefineMaterials();
  
  //define geometrical parameters
  //
  m_cabinRadius = 50*cm;       
  m_cabinLength = 80*cm;
  m_spacecraftThickness = 2*cm;
  
  m_floorThickness = 2*cm;

  m_astronautHeight = 50*cm;
  m_astronautTopWidth = 40*cm;    
  m_astronautBottomWidth = 60*cm;  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
  //
  // define simple materials
  //
  G4double Z,A,density;
  
  G4Material* Al =
  new G4Material("Aluminium", Z=13., A=26.98*g/mole, density=2.700*g/cm3);
  
  //
  // build compounds from Elements
  //
  G4Element* H  = new G4Element("Hydrogen" ,"H" , Z=1., A= 1.01*g/mole);  
  G4Element* N  = new G4Element("Nitrogen" ,"N" , Z=7., A=14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"   ,"O" , Z=8., A=16.00*g/mole);

  G4int ncomponents, natoms;
  G4double fractionmass;  
  
  G4Material* Air = 
  new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=70.*perCent);
  Air->AddElement(O, fractionmass=30.*perCent);

  G4Material* H2O = 
  new G4Material("Water", density= 1.000*g/cm3, ncomponents=2);
  H2O->AddElement(H, natoms=2);
  H2O->AddElement(O, natoms=1);
        
  // Vacuum standard definition...
  //
  G4Material* Vacuum =
  new G4Material("Galactic", Z=1., A=1.01*g/mole, density= 1.e-10*g/cm3,
                           kStateGas, 2.73*kelvin, 3.e-18*pascal);
			   
  //associate materials
  //
  m_worldMaterial = Vacuum;
  m_spacecraftMaterial = m_floorMaterial = Al;
  m_cabinMaterial = Air;
  m_astronautMaterial = H2O;
			     
  // Print
  //
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
  //define geometrical parameters
  //    
  m_spacecraftRadius = m_cabinRadius + m_spacecraftThickness;       
  m_spacecraftLength = m_cabinLength + 2*m_spacecraftThickness;
  
  m_floorYcenter   = - 0.5*m_cabinRadius;
  G4double floorYtop      = m_floorYcenter + 0.5*m_floorThickness;
  G4double floorYbottom   = m_floorYcenter - 0.5*m_floorThickness;      
  m_floorWidth =
          2*std::sqrt(m_cabinRadius*m_cabinRadius - floorYbottom*floorYbottom);
  m_floorLength    = m_cabinLength;
    
  m_worldSizeXY = 2.2*m_spacecraftRadius;
  m_worldSizeZ  = 1.1*m_spacecraftLength;
  
  m_astronautPosition = floorYtop + 0.5*m_astronautHeight;
  
    
  // World volume
  //

  G4Box*
  solidWorld = new G4Box("World",				   //its name
		 m_worldSizeXY/2,m_worldSizeXY/2,m_worldSizeZ/2);  //its size
  
  G4LogicalVolume*  
  logicWorld = new G4LogicalVolume(solidWorld,	        //its solid
				   m_worldMaterial,	//its material
				   "World");		//its name

  m_physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 logicWorld,		//its logical volume
                                 "World",		//its name
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number
				 
  // SpaceCraft volume
  //
  G4Tubs*
  solidSpacecraft = new G4Tubs("Spacecraft",		//name
                         0*cm, m_spacecraftRadius,	//r1, r2
			 0.5*m_spacecraftLength,	//half-length 
			 0., twopi);			//teta1, teta2
			 
  G4LogicalVolume*    
  logicSpacecraft = new G4LogicalVolume(solidSpacecraft,   //solid
                                    m_spacecraftMaterial,  //material
					"Spacecraft");     //name
  
                    new G4PVPlacement(0,		//no rotation
                                  G4ThreeVector(0,0,0),	//position
				  logicSpacecraft,	//logicalVolume
				  "Spacecraft",		//name
				   logicWorld,		//mother
				   false,		//no boolean
				   0);			//copy nb
  
  // Cabin
  //
  G4Tubs*
  solidCabin = new G4Tubs("Cabin",			//name
                         0*cm, m_cabinRadius,		//r1, r2
			 0.5*m_cabinLength,		//half-length 
			 0., 360*deg);			//teta1, teta2
			 
  G4LogicalVolume*    
  logicCabin = new G4LogicalVolume(solidCabin,		//solid
                                   m_cabinMaterial,	//material
				   "Cabin");		//name
					  
               new G4PVPlacement(0,			//no rotation
                                 G4ThreeVector(0,0,0),	//position
				 logicCabin,		//logicalVolume
				 "Cabin",		//name
				  logicSpacecraft,	//mother
				  false,		//no boolean
				  0);			//copy nb
  

  // Floor
  //
  G4Box*
  solidFloor = new G4Box("Floor",					 //name
                    m_floorWidth/2,m_floorLength/2,m_floorThickness/2);	 //size
		    
  G4LogicalVolume*  
  logicFloor = new G4LogicalVolume(solidFloor, m_floorMaterial, "Floor");
  
  //rotation matrix to place the floor within cabin
  G4RotationMatrix* rotm1 = new G4RotationMatrix();
  rotm1->rotateX(90*deg);

              new G4PVPlacement(rotm1,			//rotation
                                 G4ThreeVector(0,m_floorYcenter,0), //position 
                                 logicFloor,		//logicalVolume 
                                 "Floor",		//name 
                                 logicCabin, 		//mother
                                 false,			//no boolean
                                 0);			//copy nb
				 
  // Astronaut
  //
  
  G4Trd*
   solidAstronaut = new G4Trd("Astronaut",              //name
               0.5 * m_astronautBottomWidth,            //dx1
               0.5 * m_astronautTopWidth,               //dx2
               0.5 * m_astronautBottomWidth,            //dy1
               0.5 * m_astronautTopWidth,               //dy2
               0.5 * m_astronautHeight);                //dz

			 
  G4LogicalVolume*    
  logicAstronaut = new G4LogicalVolume(solidAstronaut,	//solid
                                   m_astronautMaterial,	//material
				   "Astronaut");	//name
				   
  //rotation matrix to place astronaut within cabin
  G4RotationMatrix* rotm2 = new G4RotationMatrix();
  rotm2->rotateX(90*deg);
  
  //translation vector to place astronaut within cabin
  G4ThreeVector position = G4ThreeVector(0.,m_astronautPosition,0.);				   
					  
               new G4PVPlacement(rotm2,			//no rotation
                                 position,		//position
				 logicAstronaut,	//logicalVolume
				 "Astronaut",		//name
				  logicCabin,		//mother
				  false,		//no boolean
				  0);			//copy nb
  //colour attributes
  //
  
  G4VisAttributes* colourWhite= new G4VisAttributes(G4Colour(1.,1.,1.)); 
  colourWhite->SetVisibility(true);
  colourWhite->SetForceSolid(false);
  
  G4VisAttributes* colourGrey= new G4VisAttributes(G4Colour(0.5,0.5,0.5)); 
  colourGrey->SetVisibility(true);
  colourGrey->SetForceSolid(false);
  
  G4VisAttributes* colourCyan= new G4VisAttributes(G4Colour(0.,1.,1.)); 
  colourCyan->SetVisibility(true);
  colourCyan->SetForceSolid(false);

  G4VisAttributes* colourMagenta= new G4VisAttributes(G4Colour(1.,0.,1.)); 
  colourMagenta->SetVisibility(true);
  colourMagenta->SetForceSolid(true);

  G4VisAttributes* colourBlue= new G4VisAttributes(G4Colour(0.,0.,1.)); 
  colourBlue->SetVisibility(true);
  colourBlue->SetForceSolid(false);
  
  logicWorld->SetVisAttributes(colourGrey);
  logicSpacecraft->SetVisAttributes(colourCyan);
  logicCabin->SetVisAttributes(colourCyan);
  logicFloor->SetVisAttributes(colourWhite);
  logicAstronaut->SetVisAttributes(colourBlue);
    
  //print parameters
  //
  PrintParameters();
  
  // always return Physical World
  //
  return m_physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
  G4cout << "\n---------------------------------------------------------\n";
  G4cout 
    << "---> World : (" << m_worldSizeXY/m << " X " << m_worldSizeXY/m << " X "
                        << m_worldSizeZ/m << ") m of "
                        << m_worldMaterial->GetName() << G4endl;
  G4cout
    << "---> Spacecraft : radius = " << G4BestUnit(m_spacecraftRadius,"Length")
    << "\t length = " << G4BestUnit(m_spacecraftLength,"Length")
    << "\t thickness = " << G4BestUnit(m_spacecraftThickness,"Length")        
    << "\t material : " << m_spacecraftMaterial->GetName() << G4endl;
  G4cout
    << "---> Cabin : radius = " << G4BestUnit(m_cabinRadius,"Length")
    << "\t length = " << G4BestUnit(m_cabinLength,"Length")
    << "\t material : " << m_cabinMaterial->GetName() << G4endl;
  G4cout
    << "---> Floor : thickness = " << G4BestUnit(m_floorThickness,"Length")
    << "\t width = " << G4BestUnit(m_floorWidth,"Length")    
    << "\t length = " << G4BestUnit(m_floorLength,"Length")
    << "\t material : " << m_floorMaterial->GetName() << G4endl;
  G4cout
    << "---> Astronaut : height = " << G4BestUnit(m_astronautHeight,"Length")
    << "\t bottom width = " << G4BestUnit(m_astronautBottomWidth,"Length")
    << "\t top width = " << G4BestUnit(m_astronautTopWidth,"Length")
    << "\t material : " << m_astronautMaterial->GetName() << G4endl;            
  G4cout << "\n---------------------------------------------------------\n";
  G4cout << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) m_worldMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetSpacecraftMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) m_spacecraftMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCabinMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) m_cabinMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetFloorMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) m_floorMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAstronautMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) m_astronautMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
