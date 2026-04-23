#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction() : G4UserRunAction() {
    auto accManager = G4AccumulableManager::Instance();
    accManager->Register(fInitAlpha); accManager->Register(fInitBeta);
    accManager->Register(fInitGamma); accManager->Register(fInitPositron);
    accManager->Register(fDetAlpha);  accManager->Register(fDetBeta);
    accManager->Register(fDetGamma);  accManager->Register(fDetPositron);

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->CreateNtuple("Transmission", "Hits");
    analysisManager->CreateNtupleSColumn("Particle");
    analysisManager->CreateNtupleDColumn("Energy_MeV");
    analysisManager->CreateNtupleDColumn("PosX_cm");
    analysisManager->CreateNtupleDColumn("PosY_cm");
    analysisManager->FinishNtuple();
    analysisManager->SetNtupleMerging(true);
}

void RunAction::BeginOfRunAction(const G4Run*) {
    G4AccumulableManager::Instance()->Reset();
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("ShieldingResults");
}

void RunAction::AddInitialParticle(const G4String& type) {
    if (type == "alpha") fInitAlpha += 1;
    else if (type == "e-") fInitBeta += 1;
    else if (type == "e+") fInitPositron += 1;
    else if (type == "gamma") fInitGamma += 1;
}

void RunAction::AddDetectedParticle(const G4String& type) {
    if (type == "alpha") fDetAlpha += 1;
    else if (type == "e-") fDetBeta += 1;
    else if (type == "e+") fDetPositron += 1;
    else if (type == "gamma") fDetGamma += 1;
}

void RunAction::EndOfRunAction(const G4Run* run) {
    G4AccumulableManager::Instance()->Merge();
    
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    if (IsMaster() && run->GetNumberOfEvent() > 0) {
        G4cout << "\n================Thong ke ================" << G4endl;
        G4cout << "Hat \t| Ban dau \t| Truyen qua \t| Hieu suat " << G4endl;
        G4cout << "------------------------------------------------" << G4endl;
        
        auto printRow = [](G4String name, G4int init, G4int det) {
            G4double trans = (init > 0) ? (100.0 * det / init) : 0.0;
            G4cout << name << "\t\t| " << init << "\t\t| " << det << "\t\t| " << trans << " %" << G4endl;
        };

        printRow("Alpha", fInitAlpha.GetValue(), fDetAlpha.GetValue());
        printRow("Beta (e-)", fInitBeta.GetValue(), fDetBeta.GetValue());
        printRow("Beta (e+)", fInitPositron.GetValue(), fDetPositron.GetValue());
         printRow("Gamma", fInitGamma.GetValue(), fDetGamma.GetValue());
        G4cout << "================================================" << G4endl;
    }
}