#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include <iomanip>

SteppingAction::SteppingAction(EventAction* eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction)
{}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    // Lấy tiến trình gây ra bước này
    const G4VProcess* proc = step->GetPostStepPoint()->GetProcessDefinedStep();
    
    // Chỉ xử lý nếu đây là tiến trình phân rã phóng xạ
    if (proc && proc->GetProcessName() == "RadioactiveDecay") 
    {
        G4Track* track = step->GetTrack();
        const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
        
        if (secondaries && !secondaries->empty()) 
        {
            // Lấy con trỏ RunAction để thực hiện ghi file
            RunAction* runAction = static_cast<RunAction*>(
                const_cast<G4UserRunAction*>(G4RunManager::GetRunManager()->GetUserRunAction())
            );

            if (!runAction) return;

            // 1. THU THẬP THỜI GIAN PHÂN RÃ (Cho Quy luật N(t))
            // Chỉ lấy mốc thời gian của hạt nhân mẹ để tính toán sự suy giảm nguồn
            if (track->GetTrackID() == 1) {
                G4double timeSeconds = step->GetPostStepPoint()->GetGlobalTime() / CLHEP::s;
                runAction->AddDecayTime(timeSeconds);
            }

            // 2. XÁC ĐỊNH TÊN HẠT NHÂN CON
            G4String daughterName = "unknown";
            for (const G4Track* sec : *secondaries) {
                if (sec->GetDefinition()->GetParticleType() == "nucleus") {
                    daughterName = sec->GetDefinition()->GetParticleName();
                    // Làm sạch tên (loại bỏ trạng thái kích thích [E...])
                    if (daughterName.find("[") != std::string::npos) {
                        daughterName = daughterName.substr(0, daughterName.find("["));
                    }
                    break; 
                }
            }

            // 3. NHẬN DIỆN CHÍNH XÁC CÁC LOẠI TIA (Lọc nhiễu Beta)
            G4bool hasAntiNu = false; G4bool hasNu = false; 
            G4double maxE_minus = -1.0;
            const G4Track* trueBetaMinus = nullptr;

            for (const G4Track* sec : *secondaries) {
                G4String pName = sec->GetDefinition()->GetParticleName();
                if (pName == "anti_nu_e") hasAntiNu = true;
                if (pName == "nu_e")      hasNu     = true;
                if (pName == "e-" && sec->GetKineticEnergy() > maxE_minus) {
                    maxE_minus = sec->GetKineticEnergy(); 
                    trueBetaMinus = sec;
                }
            }

            // 4. GHI DỮ LIỆU REAL-TIME VÀO DULIEUPHANRA.DAT
            for (const G4Track* sec : *secondaries) 
            {
                G4String name = sec->GetDefinition()->GetParticleName();
                G4double energy = sec->GetKineticEnergy() / CLHEP::keV;
                G4double t_global = sec->GetGlobalTime() / CLHEP::s;

                // Phân loại hạt dựa trên bản chất vật lý
                G4bool isAlpha = (name == "alpha");
                G4bool isBeta  = (name == "e-" && hasAntiNu && sec == trueBetaMinus) || (name == "e+" && hasNu);
                G4bool isGamma = (name == "gamma" && energy > 1.0); 
                G4bool isNeutrino = (name == "anti_nu_e" || name == "nu_e");

                if (isAlpha || isBeta || isGamma || isNeutrino) 
                {
                    // Ghi vào file theo đúng trình tự thời gian thực mà Geant4 xử lý
                    runAction->WriteToDatFile(daughterName, name, energy, t_global);
                }
            }
        }
    }
}