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

// ================================================================= 
// Constructor: Khởi tạo các giá trị mặc định và Messenger
// ================================================================= 
DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), 
  fLogicShield(nullptr), fLogicDetector(nullptr), fSolidShield(nullptr), fPhysDetector(nullptr),
  fShieldMatName("G4_Pb"), fShieldThickness(10*cm), fDetZPos(11*cm), fGunZPos(-15*cm), fGunEnergy(0.5*MeV)
{
    // Tạo Messenger để điều khiển qua file .mac
    fMessenger = new G4GenericMessenger(this, "/shield/", "Control shielding geometry");
    
    fMessenger->DeclareMethod("setMaterial", &DetectorConstruction::SetMaterial, "Thay đổi vật liệu tấm chắn");
    fMessenger->DeclareMethodWithUnit("setThickness", "cm", &DetectorConstruction::SetThickness, "Thay đổi độ dày tấm chắn");
    fMessenger->DeclareMethodWithUnit("setDetZ", "cm", &DetectorConstruction::SetDetectorZ, "Thay đổi vị trí máy dò");
    fMessenger->DeclareMethodWithUnit("setGunZ", "cm", &DetectorConstruction::SetGunZPosition, "Thay đổi vị trí súng bắn");
    fMessenger->DeclareMethodWithUnit("setGunEnergy", "MeV", &DetectorConstruction::SetGunEnergy, "Thay đổi năng lượng súng");
}

DetectorConstruction::~DetectorConstruction() {
    delete fMessenger;
}

// ================================================================= 
// Construct: Xây dựng hình học
// ================================================================= 
G4VPhysicalVolume* DetectorConstruction::Construct() {
    auto nist = G4NistManager::Instance();
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");

    // 1. Thế giới (World)
    auto solidWorld = new G4Box("World", 1*m, 1*m, 1*m);
    auto logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    // 2. Tấm chắn (Shield) - Tâm đặt tại gốc tọa độ (0,0,0)
    G4Material* shieldMat = nist->FindOrBuildMaterial(fShieldMatName);
    fSolidShield = new G4Box("Shield", 40*cm, 40*cm, fShieldThickness/2.0);
    fLogicShield = new G4LogicalVolume(fSolidShield, shieldMat, "Shield");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), fLogicShield, "Shield", logicWorld, false, 0, true);

    // 3. Máy dò (Detector)
    auto solidDet = new G4Box("Detector", 40*cm, 40*cm, 1*mm);
    fLogicDetector = new G4LogicalVolume(solidDet, worldMat, "Detector");
    
    // Đặt máy dò tại vị trí fDetZPos (được điều khiển từ macro)
    fPhysDetector = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, fDetZPos), 
                                      fLogicDetector, "Detector", logicWorld, false, 0, true);
    
    // Hiển thị máy dò màu xanh lá mờ
    fLogicDetector->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5)));

    return physWorld;
}

// ================================================================= 
// Các hàm Setter (Cập nhật khi gọi lệnh từ vis.mac)
// ================================================================= 

void DetectorConstruction::SetMaterial(G4String materialName) {
    auto nist = G4NistManager::Instance();
    G4Material* newMat = nist->FindOrBuildMaterial(materialName);
    if (newMat && fLogicShield) {
        fLogicShield->SetMaterial(newMat);
        fShieldMatName = materialName;
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    }
}

void DetectorConstruction::SetThickness(G4double thickness) {
    fShieldThickness = thickness;
    if (fSolidShield) {
        fSolidShield->SetZHalfLength(fShieldThickness / 2.0);
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}

void DetectorConstruction::SetDetectorZ(G4double val) {
    fDetZPos = val;
    if (fPhysDetector) {
        // Cập nhật vị trí Physical Volume ngay lập tức
        fPhysDetector->SetTranslation(G4ThreeVector(0, 0, fDetZPos));
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}

void DetectorConstruction::SetGunZPosition(G4double val) { 
    fGunZPos = val; 
}

void DetectorConstruction::SetGunEnergy(G4double val) { 
    fGunEnergy = val; 
}