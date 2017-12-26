
#ifndef NDDetectorConstruction_h
#define NDDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;


class NDDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  NDDetectorConstruction();
  ~NDDetectorConstruction();

  G4VPhysicalVolume *Construct();

private:
  G4bool  fCheckOverlaps;
  // Logical volumes & Physical volumes
  G4Box             *solidWorld;
  G4LogicalVolume   *logicWorld;
  G4VPhysicalVolume *physiWorld;

  //------------------------------------------------------------
  //ND----------------------------------------------------------
  G4Tubs            *solidHVOut;
  G4LogicalVolume   *logicHVOut;
  G4VPhysicalVolume *physiHVOut;
  
  G4Tubs            *solidSignalOut;
  G4LogicalVolume   *logicSignalOut;
  G4VPhysicalVolume *physiSignalOut;
  
  G4Tubs            *solidMuMetalShell;
  G4LogicalVolume   *logicMuMetalShell;
  G4VPhysicalVolume *physiMuMetalShell;
  
  G4Tubs            *solidPhotoTube;
  G4LogicalVolume   *logicPhotoTube;
  G4VPhysicalVolume *physiPhotoTube;

  G4Tubs            *solidAlBox;
  G4LogicalVolume   *logicAlBox;
  G4VPhysicalVolume *physiAlBox;
  
  G4Tubs            *solidLiquidSci;
  G4LogicalVolume   *logicLiquidSci;
  G4VPhysicalVolume *physiLiquidSci;
  
  G4Tubs            *solidGlass;
  G4LogicalVolume   *logicGlass;
  G4VPhysicalVolume *physiGlass;

  G4Tubs            *solidTND;
  G4LogicalVolume   *logicTND;
  G4VPhysicalVolume *physiTND;

};

#endif
