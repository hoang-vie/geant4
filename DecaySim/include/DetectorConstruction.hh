#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4GenericMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    G4VPhysicalVolume* Construct() override;
    
    G4LogicalVolume* GetScoringVolume() const { return fLogicDetector; }
    void SetDetectorDistance(G4double dist);

private:
    G4LogicalVolume* fLogicDetector;
    G4VPhysicalVolume* fPhysDetector;
    G4GenericMessenger* fMessenger;
    G4double fDetDistance;
};
#endif