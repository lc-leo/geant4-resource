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
// $Id$
//
/// \file EDDetectorConstruction.cc
/// \brief Implementation of the EDDetectorConstruction class

#include "EDDetectorConstruction.hh"
#include "EDChamberSD.hh"
#include "EDEmCalorimeterSD.hh"
#include "EDScreenSD.hh"
#include "EDMagneticField.hh"

#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4ProductionCuts.hh"
#include "G4FieldManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDDetectorConstruction::EDDetectorConstruction()
: G4VUserDetectorConstruction(),
  fMagneticLogical(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDDetectorConstruction::~EDDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* EDDetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nistManager = G4NistManager::Instance();

  // Build materials
  G4Material* air = nistManager->FindOrBuildMaterial("G4_AIR");
  G4Material* argon = nistManager->FindOrBuildMaterial("G4_Ar");
  G4Material* csi = nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  G4Material* copper = nistManager->FindOrBuildMaterial("G4_Cu");
       // There is no need to test if materials were built/found
       // as G4NistManager would issue an error otherwise
       // Try the code with "XYZ".      

  // Print all materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;         
  
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double hx = 10*m;
  G4double hy = 3.*m;
  G4double hz = 10*m;
  
  // Make world larger for this example:
  hx += 5*m;
  hy += 5*m;
  hz += 5*m;

  // world volume
  G4Box* worldS = new G4Box("World", hx, hy, hz); 
      
  G4LogicalVolume* worldLV                         
    = new G4LogicalVolume(worldS, air, "World");
                                   
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      worldLV,               //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //
  // Tube
  //
  G4double rmin = 0.;
  G4double rmax = 1.*m;
  hz = 1.*m;
  G4double phimin = 0.;
  G4double dphi = 360.*deg;
  
  // tube volume
  G4VSolid* tubeS = new G4Tubs("Tube", rmin, rmax, hz, phimin, dphi);
  
  G4LogicalVolume* tubeLV
    = new G4LogicalVolume(tubeS, air, "Tube");
 
  // Rotation
  G4RotationMatrix* tubeRotation = new G4RotationMatrix();
  tubeRotation->rotateX(90.*deg); 

  new G4PVPlacement(tubeRotation, 
                    G4ThreeVector(),       //at (0,0,0)
                    tubeLV,                //its logical volume
                    "Tube",                //its name
                    worldLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking

  // The magnetic field will be assigned to the Tube volume
  fMagneticLogical = tubeLV;                   

  // First arm
  hx = 1.5*m;
  hy = 1.*m;
  hz = 3.*m;
  G4VSolid* firstArmS = new G4Box("FirstArmS", hx, hy, hz);
  G4LogicalVolume* firstArmLV
    = new G4LogicalVolume(firstArmS, air, "FirstArm");
    
  G4double zpos = -5.*m;   
  new G4PVPlacement(0, 
                    G4ThreeVector(0, 0, zpos), 
                    firstArmLV,            //its logical volume
                    "FirstArm",            //its name
                    worldLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking

  // Drift chambers in First arm
  hx = 1.*m;
  hy = 30.*cm;
  hz = 1.*cm;
  G4VSolid* chamber1S = new G4Box("Chamber1S", hx, hy, hz);
  G4LogicalVolume* chamber1LV
    = new G4LogicalVolume(chamber1S, argon, "Chamber1");

  G4double dz = 0.5*m;
  for (G4int i=0; i<5; ++i) {
    G4double zposi = (i-2)*dz;  
    new G4PVPlacement(0, 
                    G4ThreeVector(0, 0, zposi),
                    chamber1LV,            //its logical volume
                    "Chamber1",            //its name
                    firstArmLV,            //its mother  volume
                    false,                 //no boolean operation
                    i,                     //copy number
                    checkOverlaps);        //overlaps checking
  }

  // Wire plane in drift chambers in First arm
  hx = 1.*m;
  hy = 30.*cm;
  hz = 0.1*mm;
  G4VSolid* wirePlane1S = new G4Box("WirePlane1S", hx, hy, hz);
  G4LogicalVolume* wirePlane1LV
    = new G4LogicalVolume(wirePlane1S, copper, "WirePlane1");
  new G4PVPlacement(0, 
                    G4ThreeVector(),       //at (0,0,0)
                    wirePlane1LV,          //its logical volume
                    "WirePlane1",          //its name
                    chamber1LV,            //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking

  // Second arm
  hx = 1.5*m;
  hy = 1.*m;
  hz = 3.*m;
  G4VSolid* secondArmS = new G4Box("SecondArmS", hx, hy, hz);
  G4LogicalVolume* secondArmLV
    = new G4LogicalVolume(secondArmS, air, "SecondArm");
  
  // Rotation
  G4RotationMatrix* secondArmRotation = new G4RotationMatrix();
  G4double armAngle = 30.*deg;
  secondArmRotation->rotateY(armAngle); 

  // Position 
  zpos = 5.*m;  
  G4ThreeVector secondArmPosition(0, 0, zpos);
  secondArmPosition.rotateY(-armAngle); 
  new G4PVPlacement(secondArmRotation, 
                    secondArmPosition, 
                    secondArmLV,           //its logical volume
                    "SecondArm",           //its name
                    worldLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking

  // Drift chambers in Second arm
  hx = 1.5*m;
  hy = 30.*cm;
  hz = 1.*cm;
  dz = 0.5*m;
  G4VSolid* chamber2S = new G4Box("Chamber2S", hx, hy, hz);
  G4LogicalVolume* chamber2LV
    = new G4LogicalVolume(chamber2S, argon, "Chamber2");
  for (G4int i=0; i<5; ++i) {
    G4double zposi = (i-2)*dz;  
    new G4PVPlacement(0, 
                    G4ThreeVector(0, 0, zposi),
                    chamber2LV,            //its logical volume
                    "Chamber2",            //its name
                    secondArmLV,            //its mother  volume
                    false,                 //no boolean operation
                    i,                     //copy number
                    checkOverlaps);        //overlaps checking
  }

  // Wire plane in drift chambers in Second arm
  hx = 1.5*m;
  hy = 30.*cm;
  hz = 0.1*mm;
  G4VSolid* wirePlane2S = new G4Box("WirePlane2S", hx, hy, hz);
  G4LogicalVolume* wirePlane2LV
    = new G4LogicalVolume(wirePlane2S, copper, "WirePlane2");
  new G4PVPlacement(0, 
                    G4ThreeVector(),       //at (0,0,0)
                    wirePlane2LV,          //its logical volume
                    "WirePlane2",          //its name
                    chamber2LV,            //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking


  // EM calorimeter
  hx = 1.5*m;
  hy = 30.*cm;
  hz = 15.*cm;
  G4VSolid* emCalorimeterS = new G4Box("EmCalorimeterS", hx, hy, hz);
  G4LogicalVolume* emCalorimeterLV
    = new G4LogicalVolume(emCalorimeterS, csi, "EmCalorimeter");

  zpos = 2.*m;
  new G4PVPlacement(0, 
                    G4ThreeVector(0, 0, zpos),
                    emCalorimeterLV,       //its logical volume
                    "EmCalorimeter",       //its name
                    secondArmLV,           //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking
  
  hx = 15.*cm;
  hy = 30.*cm;
  hz = 15.*cm;
  G4VSolid* emLayerS = new G4Box("EmLayerS", hx, hy, hz);
  G4LogicalVolume* emLayerLV
    = new G4LogicalVolume(emLayerS, csi, "EmLayer");

/*
  for (G4int i=0; i<10; ++i) {
    G4double xpos = -1.5*m + (2*i+1)*hz;  
    new G4PVPlacement(0, 
                    G4ThreeVector(xpos, 0, 0),
                    emLayerLV,             //its logical volume
                    "EmLayer",             //its name
                    emCalorimeterLV,       //its mother  volume
                    false,                 //no boolean operation
                    i,                     //copy number
                    checkOverlaps);        //overlaps checking
  }                  
*/

  G4int nofLayers = 10;
  G4double layerThickness = 2*hx;
  new G4PVReplica("EmLayer",        // its name
                  emLayerLV,        // its logical volume
                  emCalorimeterLV,  // its mother
                  kXAxis,           // axis of replication
                  nofLayers,        // number of replica
                  layerThickness);  // witdth of replica

  hx = 15.*cm;
  hy = 10.*cm;
  hz = 15.*cm;
  G4VSolid* emCellS = new G4Box("EmCellS", hx, hy, hz);
  G4LogicalVolume* emCellLV
    = new G4LogicalVolume(emCellS, csi, "EmCell");

  nofLayers = 3;
  layerThickness = 2*hy;
  new G4PVReplica("EmCell",        // its name
                  emCellLV,        // its logical volume
                  emLayerLV,       // its mother
                  kYAxis,           // axis of replication
                  nofLayers,        // number of replica
                  layerThickness);  // witdth of replica


  // Define region
  G4Region* emCalorimeterRegion = new G4Region("EmCalorimeterRegion");
  emCalorimeterLV->SetRegion(emCalorimeterRegion);
  emCalorimeterRegion->AddRootLogicalVolume(emCalorimeterLV);

  // Set production cuts to the region
  // (there is no UI command available for this_
  G4ProductionCuts* cuts = new G4ProductionCuts() ;
  cuts->SetProductionCut(0.03*mm, "gamma");
  cuts->SetProductionCut(0.02*mm, "e-");
  cuts->SetProductionCut(0.01*mm, "e+");
  emCalorimeterRegion->SetProductionCuts(cuts);


 // Make a thin screen for counting:
  hx = 5.0*m;
  hy = 3.0*m;
  hz = 1.*mm;
  G4VSolid* screenS = new G4Box("screenS", hx, hy, hz);
  G4LogicalVolume* screenLV
    = new G4LogicalVolume(screenS, air, "Screen");
  
  // Rotation
  G4RotationMatrix* screenRotation = new G4RotationMatrix();
  *screenRotation = *secondArmRotation;
  
  // Position 
  zpos = 5.*m + 3.*m + 2.5*m + 1.*mm;
  G4ThreeVector screenPosition(0, 0, zpos);
  screenPosition.rotateY(-armAngle); 
  new G4PVPlacement(screenRotation, 
		    screenPosition, 
		    screenLV,              //its logical volume
		    "Screen",              //its name
		    worldLV,               //its mother  volume
		    false,                 //no boolean operation
		    0,                     //copy number
		    checkOverlaps);        //overlaps checking

  
  //always return the physical World
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDDetectorConstruction::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // 
  // Sensitive detectors
  //
  EDChamberSD* chamber1SD 
    = new EDChamberSD("Chamber1SD", "Chamber1HitsCollection", 0);
  G4SDManager::GetSDMpointer()->AddNewDetector(chamber1SD);
  SetSensitiveDetector("WirePlane1", chamber1SD);

  EDChamberSD* chamber2SD 
    = new EDChamberSD("Chamber2SD", "Chamber2HitsCollection", 1);
  G4SDManager::GetSDMpointer()->AddNewDetector(chamber2SD);
  SetSensitiveDetector("WirePlane2", chamber2SD);

  EDEmCalorimeterSD* calorimeterSD 
    = new EDEmCalorimeterSD("EmCalorimeterSD", "EmCalorimeterHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(calorimeterSD);
  SetSensitiveDetector("EmCell", calorimeterSD);

  EDScreenSD* screenSD = new EDScreenSD("ScreenSD", 2);
  G4SDManager::GetSDMpointer()->AddNewDetector(screenSD);
  SetSensitiveDetector("Screen", screenSD);

  // Create magnetic field and set it to Tube using the function
  // void G4LogicalVolume::SetFieldManager(G4FieldManager *pFieldMgr, G4bool forceToAllDaughters);
  EDMagneticField* magneticField = new EDMagneticField();

  G4FieldManager* fieldManager = new G4FieldManager();
  fieldManager->SetDetectorField(magneticField);
  fieldManager->CreateChordFinder(magneticField);

  // Set field to a logical volume
  G4bool forceToAllDaughters = true;
  fMagneticLogical->SetFieldManager(fieldManager, forceToAllDaughters);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
