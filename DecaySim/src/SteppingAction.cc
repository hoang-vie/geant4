#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction), fScoringVolume(nullptr) {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    if (!fScoringVolume) {
        const auto* det = static_cast<const DetectorConstruction*>(
            G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fScoringVolume = det->GetScoringVolume();
    }

    auto* analysisManager = G4AnalysisManager::Instance();

    // =========================================================
    // KHỐI 1: BỘ LỌC ĐỈNH PHÂN RÃ (PHYSICS TRUTH FILTER)
    // =========================================================
    const G4VProcess* proc = step->GetPostStepPoint()->GetProcessDefinedStep();
    
    // Bắt quả tang ngay tại khoảnh khắc nứt vỡ của hạt nhân
    if (proc && proc->GetProcessName() == "RadioactiveDecay")
    {
        const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();

        if (secondaries && !secondaries->empty())
        {
            // --- BƯỚC 1: QUÉT TÌM DẤU ẤN VÀ CHỌN LỌC HẠT SƠ CẤP ---
            G4bool hasAntiNu = false; // Chìa khóa của Beta-
            G4bool hasNu     = false; // Chìa khóa của Beta+ và EC

            G4double maxE_minus = -1.0; 
            const G4Track* trueBetaMinus = nullptr; 
            
            for (const G4Track* sec : *secondaries)
            {
                G4String name = sec->GetDefinition()->GetParticleName();
                G4double ke   = sec->GetKineticEnergy();

                if      (name == "anti_nu_e") { hasAntiNu = true; }
                else if (name == "nu_e")      { hasNu     = true; }
                
                // Màng lọc Beta-: Chỉ lấy electron có động năng cao nhất 
                // (Đảm bảo bỏ qua mọi electron Auger/IC rác sinh ra cùng lúc)
                else if (name == "e-") {
                    if (ke > maxE_minus) { 
                        maxE_minus = ke; 
                        trueBetaMinus = sec; 
                    }
                }
            }

            // --- BƯỚC 2: CẤP QUYỀN GHI VÀO NTUPLE 2 (CÂN BẰNG PHƯƠNG TRÌNH) ---
            for (const G4Track* sec : *secondaries)
            {
                // Vứt bỏ hạt nhân con (Baryon > 4)
                if (sec->GetDefinition()->GetBaryonNumber() > 4) continue;

                G4String name = sec->GetDefinition()->GetParticleName();
                bool shouldRecord = false;

                // 1. Alpha và Neutrino: Bản chất hạt nhân, không có hạt rác mô phỏng
                if (name == "alpha" || name == "anti_nu_e" || name == "nu_e") {
                    shouldRecord = true;
                }
                // 2. Beta-: Yêu cầu ĐỒNG THỜI có phản neutrino VÀ là hạt e- động năng đỉnh
                else if (name == "e-" && hasAntiNu && sec == trueBetaMinus) {
                    shouldRecord = true;
                }
                // 3. Beta+: Hễ có Neutrino và sinh Positron -> 100% là hàng chuẩn (Vỏ không sinh e+)
                else if (name == "e+" && hasNu) {
                    shouldRecord = true;
                }
                // 4. Gamma: Lọc nhẹ để loại bỏ Tia X huỳnh quang (X-Rays) từ lớp vỏ
                // Hầu hết tia X < 10 keV, trong khi Gamma phân rã thường > vài chục keV
                else if (name == "gamma") {
                    if (sec->GetKineticEnergy() > 10.0 * keV) { // Bạn có thể chỉnh mức này
                        shouldRecord = true;
                    }
                }

                // Ghi nhận chứng vào hồ sơ
                if (shouldRecord) {
                    analysisManager->FillNtupleSColumn(2, 0, name);
                    analysisManager->FillNtupleDColumn(2, 1, sec->GetKineticEnergy() / keV);
                    analysisManager->FillNtupleDColumn(2, 2, sec->GetGlobalTime() / ns);
                    analysisManager->AddNtupleRow(2);
                }
            }
        }
    }

    // =========================================================
    // KHỐI 2: ĐẾM HẠT ĐẬP VÀO DETECTOR (NTUPLE 0 & 1)
    // =========================================================
    if (!fScoringVolume) return;

    G4Track* track = step->GetTrack();
    G4LogicalVolume* preVolume  = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    G4LogicalVolume* postVolume = nullptr;
    
    if (step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()) {
        postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    }

    if (postVolume == fScoringVolume && preVolume != fScoringVolume)
    {
        G4String pName = track->GetDefinition()->GetParticleName();

        analysisManager->FillNtupleSColumn(0, 0, pName);
        analysisManager->FillNtupleDColumn(0, 1, step->GetPreStepPoint()->GetKineticEnergy() / keV);
        analysisManager->FillNtupleDColumn(0, 2, step->GetPreStepPoint()->GetGlobalTime()    / ns);
        analysisManager->AddNtupleRow(0);

        fEventAction->GetRunAction()->CountHit();
    }
}