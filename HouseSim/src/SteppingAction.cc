#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh" // THÊM DÒNG NÀY ĐỂ ĐỊNH NGHĨA MeV

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(), fEventAction(eventAction), fScoringVolume(nullptr) 
{}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    // 1. Kiểm tra nếu hạt vừa mới bước vào thể tích "World"
    G4VPhysicalVolume* postVolume = step->GetPostStepPoint()->GetPhysicalVolume();
    G4VPhysicalVolume* preVolume = step->GetPreStepPoint()->GetPhysicalVolume();

    // Nếu volume đứng sau là World, và volume đứng trước KHÔNG phải World 
    // nghĩa là hạt vừa thoát ra khỏi hệ thống che chắn
    if (postVolume && postVolume->GetName() == "World") {
        if (preVolume && preVolume->GetName() != "World") {
            
            G4String pName = step->GetTrack()->GetDefinition()->GetParticleName();
            G4double energy = step->GetPreStepPoint()->GetKineticEnergy();

            // In thông báo khi có hạt "vượt ngục" thành công
            G4cout << "--- Hạt " << pName 
                   << " đã xuyên qua tường! Năng lượng thoát ra: " 
                   << energy/MeV << " MeV" << G4endl;
        }
    }

    // 2. Phần code cũ để tính năng lượng lắng đọng (Edep) bên trong khối Detector
    if (!fScoringVolume) {
        const auto detConstruction = static_cast<const DetectorConstruction*>(
            G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fScoringVolume = detConstruction->GetScoringVolume();
    }

    G4LogicalVolume* currentVolume = step->GetPreStepPoint()->GetTouchableHandle()
                                     ->GetVolume()->GetLogicalVolume();

    if (currentVolume == fScoringVolume) {
        G4double edep = step->GetTotalEnergyDeposit();
        fEventAction->AddEdep(edep);
    }
}