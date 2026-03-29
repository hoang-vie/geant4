#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), fLogicDetector(nullptr), fPhysDetector(nullptr), fDetDistance(2.0*cm) {
    fMessenger = new G4GenericMessenger(this, "/geom/", "Geometry Control");
    fMessenger->DeclareMethodWithUnit("setDetDistance", "cm", &DetectorConstruction::SetDetectorDistance, "Change distance");
}

DetectorConstruction::~DetectorConstruction() { delete fMessenger; }

G4VPhysicalVolume* DetectorConstruction::Construct() {
    auto nist = G4NistManager::Instance();
    G4Material* air = nist->FindOrBuildMaterial("G4_AIR");

    auto solidWorld = new G4Box("World", 1*m, 1*m, 1*m);
    auto logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Nguồn phóng xạ: Hình cầu bán kính 5mm tại gốc tọa độ
    auto solidSource = new G4Orb("Source", 2*mm);
    auto logicSource = new G4LogicalVolume(solidSource, air, "Source");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), logicSource, "Source", logicWorld, false, 0, true);
    logicSource->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 1.0, 0.0))); 
    
    
    // Detector: 10x10 cm, dày 1mm
    auto solidDet = new G4Box("Detector", 5*cm, 5*cm, 0.5*mm);
    fLogicDetector = new G4LogicalVolume(solidDet, air, "Detector");
    fPhysDetector = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, fDetDistance), fLogicDetector, "Detector", logicWorld, false, 0, true);
    
    auto detVis = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
    detVis->SetForceWireframe(true);
    fLogicDetector->SetVisAttributes(detVis);

    return physWorld;
}

void DetectorConstruction::SetDetectorDistance(G4double dist) {
    fDetDistance = dist;
    if (fPhysDetector) {
        fPhysDetector->SetTranslation(G4ThreeVector(0, 0, fDetDistance));
        G4RunManager::GetRunManager()->GeometryHasBeenModified(); 
    }
}