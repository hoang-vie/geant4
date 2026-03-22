#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4GenericMessenger;
class G4FieldManager;
class G4ElectroMagneticField; // Thêm dòng này

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    
    void UpdateField(); // Kích hoạt trường

private:
    G4LogicalVolume* fLogicWorld;
    G4LogicalVolume* fLogicFieldBox; // Vùng chứa trường
    
    G4GenericMessenger* fMessenger;
    G4FieldManager* fFieldManager;
    G4ElectroMagneticField* fEMField;

    // Biến lưu cường độ trường
    G4double fFieldValueB; // Từ trường
    G4double fFieldValueE; // Điện trường
};
#endif