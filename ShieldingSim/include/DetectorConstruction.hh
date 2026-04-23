#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Box;
class G4GenericMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    
    // Hàm lấy thể tích ghi nhận (Scoring Volume) cho SteppingAction
    G4LogicalVolume* GetScoringVolume() const { return fLogicDetector; }

    // Các hàm thiết lập thông số từ Messenger (gọi từ macro)
    void SetMaterial(G4String materialName);
    void SetThickness(G4double thickness);
    void SetDetectorZ(G4double val);       // THÊM: Để chỉnh vị trí máy dò
    void SetGunZPosition(G4double z);      // Chỉ khai báo ở đây
    void SetGunEnergy(G4double energy);    // Chỉ khai báo ở đây

    // Các hàm lấy thông số (Getter) cho các class khác
    G4double GetGunZPosition() const { return fGunZPos; }
    G4double GetGunEnergy() const { return fGunEnergy; }
    G4double GetDetectorZ() const    { return fDetZPos; }

private:
    // Con trỏ quản lý Volume
    G4LogicalVolume* fLogicShield;
    G4LogicalVolume* fLogicDetector;
    G4Box* fSolidShield;
    G4VPhysicalVolume* fPhysDetector;   // THÊM: Để dịch chuyển máy dò động
    
    G4GenericMessenger* fMessenger;
    
    // Biến lưu trữ thông số
    G4String fShieldMatName;     
    G4double fShieldThickness;
    G4double fDetZPos;           // THÊM: Lưu vị trí máy dò
    G4double fGunZPos;
    G4double fGunEnergy;
};

#endif