#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "G4ElectroMagneticField.hh"
#include "G4EqMagElectricField.hh"
#include "G4ClassicalRK4.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4IntegrationDriver.hh"
#include "G4RunManager.hh" 

// =========================================================================
// TẠO CLASS TRƯỜNG ĐIỆN TỪ HỖN HỢP
// =========================================================================
class UniformEMField : public G4ElectroMagneticField {
public:
    UniformEMField(G4ThreeVector B, G4ThreeVector E) : fB(B), fE(E) {}
    // Gán giá trị B (index 0,1,2) và E (index 3,4,5) cho Geant4
    void GetFieldValue(const G4double*, G4double* bField) const override {
        bField[0] = fB.x(); bField[1] = fB.y(); bField[2] = fB.z();
        bField[3] = fE.x(); bField[4] = fE.y(); bField[5] = fE.z();
    }
    G4bool DoesFieldChangeEnergy() const override { return true; } // Điện trường làm thay đổi năng lượng
private:
    G4ThreeVector fB;
    G4ThreeVector fE;
};

// =========================================================================

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), fLogicWorld(nullptr), fLogicFieldBox(nullptr), 
  fFieldManager(nullptr), fEMField(nullptr), fFieldValueB(0.0), fFieldValueE(0.0) {
    
    fMessenger = new G4GenericMessenger(this, "/field/", "Electromagnetic Field Control");
    
    fMessenger->DeclarePropertyWithUnit("magValue", "tesla", fFieldValueB, "Set Magnetic Field (Y-axis)");
    fMessenger->DeclareProperty("elecValue", fFieldValueE, "Set Electric Field in kV/cm (X-axis)");
    fMessenger->DeclareMethod("update", &DetectorConstruction::UpdateField, "Apply the field");
}
DetectorConstruction::~DetectorConstruction() { 
    delete fMessenger; 
    delete fFieldManager; 
    if(fEMField) delete fEMField;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    auto nist = G4NistManager::Instance();
    auto vacuum = nist->FindOrBuildMaterial("G4_Galactic");

    // 1. WORLD KHÔNG GIAN (Vô hình)
    auto solidWorld = new G4Box("World", 1.5*m, 1.5*m, 1.5*m);
    fLogicWorld = new G4LogicalVolume(solidWorld, vacuum, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), fLogicWorld, "World", nullptr, false, 0, true);
    fLogicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    // 2. VÙNG TRƯỜNG ĐIỆN TỪ (Field Box) - Rộng 2m
    auto solidFieldBox = new G4Box("FieldBox", 1.0*m, 1.0*m, 1.0*m);
    fLogicFieldBox = new G4LogicalVolume(solidFieldBox, vacuum, "FieldBox");
    // Đặt Field Box vào trong World
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), fLogicFieldBox, "FieldBox", fLogicWorld, false, 0, true);

    // Chỉnh màu FieldBox: Khung lưới màu Xanh lơ nhạt để dễ nhận diện
    auto boxVis = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.3));
    boxVis->SetForceWireframe(true);
    fLogicFieldBox->SetVisAttributes(boxVis);

    return physWorld;
}

void DetectorConstruction::UpdateField() {
    if (fFieldManager) { delete fFieldManager; fFieldManager = nullptr; }
    if (fEMField) { delete fEMField; fEMField = nullptr; }

    fFieldManager = new G4FieldManager();
    
    fEMField = new UniformEMField(G4ThreeVector(0., fFieldValueB, 0.),
                                  G4ThreeVector(fFieldValueE * (kilovolt/cm), 0., 0.));

    auto equation = new G4EqMagElectricField(fEMField);
    auto stepper = new G4ClassicalRK4(equation, 8);
    G4double minStep = 0.01 * mm;
    auto intDriver = new G4IntegrationDriver<G4ClassicalRK4>(minStep, stepper, stepper->GetNumberOfVariables());
    auto chordFinder = new G4ChordFinder(intDriver);

    fFieldManager->SetDetectorField(fEMField);
    fFieldManager->SetChordFinder(chordFinder);

    fLogicFieldBox->SetFieldManager(fFieldManager, true);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();

    G4cout << "\n---> FIELDS APPLIED TO BOX:" << G4endl;
    G4cout << "     Magnetic (Y): " << fFieldValueB / tesla << " Tesla" << G4endl;
    G4cout << "     Electric (X): " << fFieldValueE << " kV/cm\n" << G4endl;
}