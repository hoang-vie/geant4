#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), fLogicShield(nullptr), fLogicDetector(nullptr), fSolidShield(nullptr), 
  fShieldMatName("G4_Pb"), fShieldThickness(1.0*cm), fGunZPos(-5.0*cm), fGunEnergy(5.0*MeV) {
    
    fMessenger = new G4GenericMessenger(this, "/shield/", "Control shielding geometry");
    fMessenger->DeclareMethod("setMaterial", &DetectorConstruction::SetMaterial, "Change shielding material");
    fMessenger->DeclareMethodWithUnit("setThickness", "cm", &DetectorConstruction::SetThickness, "Change shielding thickness");

    fMessenger->DeclareMethodWithUnit("setGunZ", "cm", &DetectorConstruction::SetGunZPosition, "Change Gun Z Position");
    fMessenger->DeclareMethodWithUnit("setGunEnergy", "MeV", &DetectorConstruction::SetGunEnergy, "Change Gun Energy");
    
}

DetectorConstruction::~DetectorConstruction() {
    delete fMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    auto nist = G4NistManager::Instance();
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

    auto solidWorld = new G4Box("World", 1*m, 1*m, 1*m);
    auto logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    // 1. TẤM CHẮN (SHIELD)
    G4Material* shieldMat = nist->FindOrBuildMaterial(fShieldMatName);
    fSolidShield = new G4Box("Shield", 40*cm, 40*cm, fShieldThickness/2.0);
    fLogicShield = new G4LogicalVolume(fSolidShield, shieldMat, "Shield");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), fLogicShield, "Shield", logicWorld, false, 0, true);

    // 2. TẤM DETECTOR (Ghi nhận hạt xuyên qua)
    G4double detZPos = (fShieldThickness/2) + 0.5*cm + 0.5*mm; 
    auto solidDet = new G4Box("Detector", 40*cm, 40*cm, 1*mm);
    fLogicDetector = new G4LogicalVolume(solidDet, worldMat, "Detector");
    // Đặt Detector phía sau tấm chắn (Cách gốc tọa độ 10cm)
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, detZPos), fLogicDetector, "Detector", logicWorld, false, 0, true);
    fLogicDetector->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5))); // Xanh lá mờ

    return physWorld;
}

void DetectorConstruction::SetMaterial(G4String materialName) {
    auto nist = G4NistManager::Instance();
    G4Material* newMat = nist->FindOrBuildMaterial(materialName);
    
    if (newMat && fLogicShield) {
        fLogicShield->SetMaterial(newMat);
        fShieldMatName = materialName;
        // Báo cho Geant4 cập nhật Vật lý
        G4RunManager::GetRunManager()->PhysicsHasBeenModified(); 
    }
}

void DetectorConstruction::SetThickness(G4double thickness) {
    fShieldThickness = thickness;
    
    if (fSolidShield) {
        // G4Box lưu trữ 1 nửa độ dày (Half-length)
        fSolidShield->SetZHalfLength(fShieldThickness / 2.0);
        // Báo cho Geant4 cập nhật Hình học
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}