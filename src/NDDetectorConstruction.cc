
#include "NDDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"


NDDetectorConstruction::NDDetectorConstruction()
: G4VUserDetectorConstruction(),
  fCheckOverlaps(true)
{
}


NDDetectorConstruction::~NDDetectorConstruction()
{
}


G4VPhysicalVolume* NDDetectorConstruction::Construct()
{
  //=========================================================================
  // Define Materials && Elements
  //=========================================================================

  G4String symbol;		// a = mass of a mole;
  G4double a, z, density;	// z = mean number of protons;
  G4int iz, n;		        // iz = number of protons in an isotope;
  G4int ncomponents, natoms;	// n = number of nucleons in an isotope;
  G4double abundance, fractionmass;

  G4Element *H  = new G4Element("Hydrogen", symbol="H",  z=1.,  a=1.00794*g/mole);
  G4Element *B  = new G4Element("Boron",    symbol="B",  z=5.,  a=10.82*g/mole);
  G4Element *C  = new G4Element("Carbon",   symbol="C",  z=6.,  a=12.011*g/mole);
  G4Element *Zn = new G4Element("Zinc",     symbol="Zn", z=30., a=65.38*g/mole);
  G4Element *N  = new G4Element("Nitrogen", symbol="N",  z=7.,  a=14.01*g/mole);
  G4Element *O  = new G4Element("Oxigen",   symbol="O",  z=8.,  a=16.00*g/mole);
  G4Element *S  = new G4Element("Sulfur",   symbol="S",  z=16., a=30.065*g/mole);
  G4Element *Si = new G4Element("Silicon",  symbol="Si", z=14., a=28.09*g/mole);
  G4Element *Ni = new G4Element("Nickel",   symbol="Ni", z=28., a=58.69*g/mole);
  G4Element *Na = new G4Element("Sodium",   symbol="Na", z=11., a=22.99*g/mole);
  G4Element *Fe = new G4Element("Iron",     symbol="Fe", z=26., a=55.85*g/mole);

  G4Material *Al = new G4Material("Aluminium", z=13., a=26.98*g/mole,  density=2.70*g/cm3);
  G4Material *Cu = new G4Material("Copper",    z=29., a=63.55*g/mole,  density=8.96*g/cm3);

  G4Material *Air = new G4Material("Air", density=1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=0.7);
  Air->AddElement(O, fractionmass=0.3);

  G4Material *ZnS = new G4Material("Zinc_Sulfide", density=4.09*g/cm3, ncomponents=2);
  ZnS->AddElement(Zn, natoms=1);
  ZnS->AddElement(S,  natoms=1);
  
  G4Isotope *Li6 = new G4Isotope("Li6", iz=3, n=6, a=6.015*g/mole);
  G4Element *elLi6 = new G4Element("enriched Lithium6", symbol="Li6", ncomponents=1);
  elLi6->AddIsotope(Li6, abundance=100.*perCent);
  G4Isotope *Li7 = new G4Isotope("Li7", iz=3, n=7, a=7.016*g/mole);
  G4Element *elLi7 = new G4Element("enriched Lithium7", symbol="Li7", ncomponents=1);
  elLi7->AddIsotope(Li7, abundance=100.*perCent);

  G4Material *Li = new G4Material("Lithium", density= 0.011*g/cm3, ncomponents=2);
  Li->AddElement(elLi6, fractionmass=0.95);
  Li->AddElement(elLi7, fractionmass=0.05);
  
  G4Material *Al2O3 = new G4Material("Aluminum_Oxide", density=3.95*g/cm3, ncomponents=2);
  Al2O3->AddMaterial(Al, fractionmass=53*perCent);
  Al2O3->AddElement(O,  natoms=3);
  
  G4Material *B2O3 = new G4Material("Boron_Trioxide", density=2.46*g/cm3, ncomponents=2);
  B2O3->AddElement(B, natoms=2);
  B2O3->AddElement(O, natoms=3);

  G4Material *Na2O = new G4Material("Sodium_Oxide", density=2.27*g/cm3, ncomponents=2);
  Na2O->AddElement(Na, natoms=2);
  Na2O->AddElement(O,  natoms=1);


  G4Material *SiO2 = new G4Material("quartz", density=2.200*g/cm3, ncomponents=2);
  SiO2->AddElement(Si, natoms=1);
  SiO2->AddElement(O,  natoms=2);

  // BC-501A Liquid Scintillator
  G4Material *liquidSci = new G4Material("BC-501A", density=0.874*g/cm3, ncomponents=2);
  liquidSci->AddElement(H, natoms=1212);
  liquidSci->AddElement(C, natoms=1000);
  
  // Phototube
  // UNKNOWN CHEMICAL COMPOSITION
  // MU-Metal (75% nickel + 15% iron + 10% copper and molybdenum)
  G4Material *muMetal = new G4Material("Mu_Metal", density=8.747*g/cm3, ncomponents=3);
  muMetal->AddElement(Ni,  fractionmass=0.75);
  muMetal->AddElement(Fe,  fractionmass=0.15);
  muMetal->AddMaterial(Cu, fractionmass=0.1);
    
  // PYREX glass (81% SiO2 + 13% B2O3 + 4% Na2O + 2% Al2O3)
  G4Material *pyrexGlass = new G4Material("Pyrex_Glass", density=2.23*g/cm3, ncomponents=4);
  pyrexGlass->AddMaterial(SiO2,  fractionmass=81.*perCent);
  pyrexGlass->AddMaterial(B2O3,  fractionmass=13.*perCent);
  pyrexGlass->AddMaterial(Na2O,  fractionmass=4.*perCent);
  pyrexGlass->AddMaterial(Al2O3, fractionmass=2.*perCent);
    
  // BC-702 Thermal Neutron Detector
  G4Material *BC702 = new G4Material("BC-702", density=4.10*g/cm3, ncomponents=2);
  BC702->AddMaterial(ZnS,  fractionmass=99.73*perCent);
  BC702->AddMaterial(Li,  fractionmass=0.27*perCent);
  
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  //=========================================================================
  // World
  //=========================================================================
  
  solidWorld = new G4Box("World", 150.*cm, 150.*cm, 150.*cm);
  logicWorld = new G4LogicalVolume(solidWorld, Air, "World");
  physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, fCheckOverlaps);
  
  //=========================================================================
  //
  // Neutron Detector
  //
  //=========================================================================

  //
  // HV out
  // 
  solidHVOut = new G4Tubs("HVOut", 0.*cm, 0.5*cm, 0.6*cm, 0.*deg, 360.*deg);
  logicHVOut = new G4LogicalVolume(solidHVOut, Al, "HVOut");
  physiHVOut = new G4PVPlacement(0, G4ThreeVector(0., -1.9*cm, (-4.109-8.109)*cm),
					logicHVOut, "HVOut", logicWorld,
					false, 0);
  //
  // HV out
  // 
  solidSignalOut = new G4Tubs("SignalOut", 0.*cm, 0.4*cm, 0.4*cm, 0.*deg, 360.*deg);
  logicSignalOut = new G4LogicalVolume(solidSignalOut, Al, "SignalOut");
  physiSignalOut = new G4PVPlacement(0, G4ThreeVector(0., 1.9*cm, (-4.109-8.109)*cm),
					logicSignalOut, "SignalOut", logicWorld,
					false, 0);
  //
  // Mu-Metal
  // 
  solidMuMetalShell = new G4Tubs("MuMetalShell", 0.*cm, 2.95*cm, 8.109*cm, 0.*deg, 360.*deg);
  logicMuMetalShell = new G4LogicalVolume(solidMuMetalShell, muMetal, "MuMetalShell");
  physiMuMetalShell = new G4PVPlacement(0, G4ThreeVector(0., 0., -4.109*cm),
					logicMuMetalShell, "MuMetalShell", logicWorld,
					false, 0);
    
  //
  // Phototube
  //
  solidPhotoTube = new G4Tubs("PhotoTube", 0.*cm, 2.54*cm, 5.65*cm, 0.*deg, 360.*deg);
  logicPhotoTube = new G4LogicalVolume(solidPhotoTube, Air, "PhotoTube"); // <--- material=Air (for temporary use)
  physiPhotoTube = new G4PVPlacement(0, G4ThreeVector(0., 0., 2.459*cm),
				     logicPhotoTube, "PhotoTube", logicMuMetalShell,
				     false, 0);

  // 
  // Al Box
  //
  solidAlBox = new G4Tubs("AlBox", 0.*cm, 3.81*cm, 3.575*cm, 0.*deg, 360.*deg);
  logicAlBox = new G4LogicalVolume(solidAlBox, Al, "AlBox");
  physiAlBox = new G4PVPlacement(0, G4ThreeVector(0., 0., 7.575*cm),
				 logicAlBox, "AlBox", logicWorld,
				 false, 0);

  // 
  // Liquid Scintillator
  //
  solidLiquidSci = new G4Tubs("LSci", 0.*cm, 2.54*cm, 2.54*cm, 0.*deg, 360.*deg);
  logicLiquidSci = new G4LogicalVolume(solidLiquidSci, liquidSci, "LSci");
  physiLiquidSci = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.415*cm),
				     logicLiquidSci, "LSci", logicAlBox,
				     false, 0);
   
  //
  // Glass
  //
  solidGlass = new G4Tubs("Glass", 0.*cm, 2.825*cm, 0.3175*cm, 0.*deg, 360.*deg);
  logicGlass = new G4LogicalVolume(solidGlass, pyrexGlass, "Glass");
  physiGlass = new G4PVPlacement(0, G4ThreeVector(0., 0., 2.4425*cm),
				 logicGlass, "Glass", logicAlBox,
				 false, 0);
    
  //
  // Thermal Neutron Detector
  //
  solidTND = new G4Tubs("TND", 0.*cm, 2.38*cm, 0.3175*cm, 0.*deg, 360.*deg);
  logicTND = new G4LogicalVolume(solidTND, BC702, "TND");
  physiTND = new G4PVPlacement(0, G4ThreeVector(0., 0., 3.0775*cm),
			       logicTND, "TND", logicAlBox,
			       false, 0);
  
  //=========================================================================
  // Visualization attributes
  //=========================================================================
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

  G4VisAttributes *outVisAtt = new G4VisAttributes(G4Colour(1., 1., 1., 0.9));
  outVisAtt->SetForceSolid(true);
  logicHVOut->SetVisAttributes(outVisAtt);
  logicSignalOut->SetVisAttributes(outVisAtt);
  
  G4VisAttributes *alVisAtt = new G4VisAttributes(G4Colour(1., 1., 1., 0.1));
  alVisAtt->SetForceSolid(true);
  logicAlBox->SetVisAttributes(alVisAtt);
  
  G4VisAttributes *mumetalVisAtt = new G4VisAttributes(G4Colour(1., 1., 0., 0.1));
  mumetalVisAtt->SetForceSolid(true);
  logicMuMetalShell->SetVisAttributes(mumetalVisAtt);

  G4VisAttributes *phototubeVisAtt = new G4VisAttributes(G4Colour(1., 1., 1., 0.1));
  phototubeVisAtt->SetForceSolid(true);
  logicPhotoTube->SetVisAttributes(phototubeVisAtt);

  G4VisAttributes *lsciVisAtt = new G4VisAttributes(G4Colour(1., 0., 1.));
  lsciVisAtt->SetForceSolid(true);
  logicLiquidSci->SetVisAttributes(lsciVisAtt);

  G4VisAttributes *glassVisAtt = new G4VisAttributes(G4Colour(1., 1., 1., 0.5));
  glassVisAtt->SetForceSolid(true);
  logicGlass->SetVisAttributes(glassVisAtt);

  G4VisAttributes *tndVisAtt = new G4VisAttributes(G4Colour(0., 1., 1.));
  tndVisAtt->SetForceSolid(true);
  logicTND->SetVisAttributes(tndVisAtt);

  return physiWorld;
}

