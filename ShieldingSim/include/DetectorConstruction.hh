#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Box;
class G4GenericMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    G4LogicalVolume* GetScoringVolume() const { return fLogicDetector; }

    void SetMaterial(G4String materialName);
    void SetThickness(G4double thickness);
    
    // THÊM: Các hàm quản lý súng
    void SetGunZPosition(G4double z) { fGunZPos = z; }
    G4double GetGunZPosition() const { return fGunZPos; }

    void SetGunEnergy(G4double energy) { fGunEnergy = energy; }
    G4double GetGunEnergy() const { return fGunEnergy; }

private:
    G4LogicalVolume* fLogicShield;
    G4LogicalVolume* fLogicDetector;
    G4Box* fSolidShield;
    G4GenericMessenger* fMessenger;
    
    G4String fShieldMatName;     
    G4double fShieldThickness;
    
    G4double fGunZPos;
    G4double fGunEnergy;
};
#endif