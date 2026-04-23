#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), fLogicDetector(nullptr) {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    auto nist = G4NistManager::Instance();
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* wallMat  = nist->FindOrBuildMaterial("G4_Pb");

    // World
    auto solidWorld = new G4Box("World", 2*m, 2*m, 2*m);
    auto logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

    // Thông số nhà
    G4double hWidth = 80*cm, hHeight = 60*cm, hDepth = 80*cm, thick = 10*cm;

    // Thân nhà rỗng
    auto outBox = new G4Box("outBox", hWidth/2, hHeight/2, hDepth/2);
    auto inBox  = new G4Box("inBox", (hWidth-2*thick)/2, (hHeight-2*thick)/2, (hDepth-2*thick)/2);
    auto wall   = new G4SubtractionSolid("Wall", outBox, inBox);
    auto lWall  = new G4LogicalVolume(wall, wallMat, "Wall");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), lWall, "Wall", logicWorld, false, 0, true);

    // Mái nhà rỗng (Trd xoay ngang)
    G4double rHeight = 30*cm;
    auto outR = new G4Trd("outR", hWidth/2, 0.1*mm, hDepth/2, hDepth/2, rHeight/2);
    auto inR  = new G4Trd("inR", (hWidth-2*thick)/2, 0.1*mm, (hDepth-2*thick)/2, (hDepth-2*thick)/2, (rHeight-thick)/2);
    auto roof = new G4SubtractionSolid("Roof", outR, inR);
    auto lRoof = new G4LogicalVolume(roof, wallMat, "Roof");
    
    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateX(90*deg);
    new G4PVPlacement(rot, G4ThreeVector(0, (hHeight+rHeight)/2, 0), lRoof, "Roof", logicWorld, false, 0, true);

    // Máy dò (Không khí bên trong nhà)
    auto sDet = new G4Box("sDet", (hWidth-2*thick)/2-1*mm, (hHeight-2*thick)/2-1*mm, (hDepth-2*thick)/2-1*mm);
    fLogicDetector = new G4LogicalVolume(sDet, worldMat, "Detector");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), fLogicDetector, "Detector", logicWorld, false, 0, true);

    // Màu sắc
    lWall->SetVisAttributes(new G4VisAttributes(G4Colour(0.5, 0.5, 0.5))); // Chì
    lRoof->SetVisAttributes(new G4VisAttributes(G4Colour(0.6, 0.2, 0.2))); // Mái
    fLogicDetector->SetVisAttributes(new G4VisAttributes(G4Colour(0, 1, 0, 0.2))); // Máy dò

    return physWorld;
}