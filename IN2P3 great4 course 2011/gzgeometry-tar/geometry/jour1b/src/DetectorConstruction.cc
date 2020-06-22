//
// $Id: DetectorConstruction.cc 142 2008-08-17 13:57:11Z maire $
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

#include "G4VisAttributes.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{  
  DefineMaterials();
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
  //define geometrical parameters
  //
  G4double cabinRadius = 50*cm;       
  G4double cabinLength = 80*cm;
  
  G4double spacecraftThickness = 2*cm;    
  G4double spacecraftRadius = cabinRadius + spacecraftThickness;       
  G4double spacecraftLength = cabinLength + 2*spacecraftThickness;
  
  G4double floorThickness = 2*cm;
  G4double floorYcenter   = - 0.5*cabinRadius;
  G4double floorYtop      = floorYcenter + 0.5*floorThickness;
  G4double floorYbottom   = floorYcenter - 0.5*floorThickness;      
  G4double floorWidth =
          2*std::sqrt(cabinRadius*cabinRadius - floorYbottom*floorYbottom);
  G4double floorLength    = cabinLength;
    
  m_worldSizeXY = 2.2*spacecraftRadius;
  m_worldSizeZ  = 1.1*spacecraftLength;
    
  G4double astronautHeight = 50*cm; 
  G4double astronautTopWidth = 40*cm; 
  G4double astronautBottomWidth = 60*cm;
  G4double astronautPosition = floorYtop + 0.5*astronautHeight;
  
    
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
                         0*cm, spacecraftRadius,	//r1, r2
			 0.5*spacecraftLength,		//half-length 
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
                         0*cm, cabinRadius,		//r1, r2
			 0.5*cabinLength,		//half-length 
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
  solidFloor = new G4Box("Floor",				     //name
                    floorWidth/2,floorLength/2,floorThickness/2);    //size
		    
  G4LogicalVolume*  
  logicFloor = new G4LogicalVolume(solidFloor, m_floorMaterial, "Floor");
  
  //rotation matrix to place the floor within cabin
  G4RotationMatrix* rotm1 = new G4RotationMatrix();
  rotm1->rotateX(90*deg);

              new G4PVPlacement(rotm1,			//rotation
                                 G4ThreeVector(0,floorYcenter,0), //position 
                                 logicFloor,		//logicalVolume 
                                 "Floor",		//name 
                                 logicCabin, 		//mother
                                 false,			//no boolean
                                 0);			//copy nb
				 
  // Astronaut
  //
   G4Trd*
   solidAstronaut = new G4Trd("Astronaut",              //name                            
               0.5 * astronautBottomWidth,		//dx1
               0.5 * astronautTopWidth, 		//dx2
               0.5 * astronautBottomWidth,		//dy1
               0.5 * astronautTopWidth, 		//dy2
               0.5 * astronautHeight); 			//dz

			 
  G4LogicalVolume*    
  logicAstronaut = new G4LogicalVolume(solidAstronaut,	//solid
                                   m_astronautMaterial,	//material
				   "Astronaut");	//name
				   
  //rotation matrix to place astronaut within cabin
  G4RotationMatrix* rotm2 = new G4RotationMatrix();
  rotm2->rotateX(90*deg);
  
  //translation vector to place astronaut within cabin
  G4ThreeVector position = G4ThreeVector(0.,astronautPosition,0.);				   
					  
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
    << "---> The world is in " << m_worldMaterial->GetName()           << "\n"
    << "---> The spacecraft is in " << m_spacecraftMaterial->GetName() << "\n"
    << "---> The cabin is in " << m_cabinMaterial->GetName()           << "\n"
    << "---> The floor is in " << m_floorMaterial->GetName()           << "\n"     
    << "---> The astronaut is in " << m_astronautMaterial->GetName();
  G4cout << "\n---------------------------------------------------------\n";
  G4cout << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
