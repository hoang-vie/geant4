#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction() : G4UserRunAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root"); // Cài đặt xuất file ROOT
    
    // Khởi tạo bảng dữ liệu (Ntuple)
    analysisManager->CreateNtuple("TrackingData", "Step-by-step data");
    analysisManager->CreateNtupleIColumn("EventID");
    analysisManager->CreateNtupleIColumn("TrackID");
    analysisManager->CreateNtupleSColumn("ParticleName");
    analysisManager->CreateNtupleDColumn("PosX_cm");
    analysisManager->CreateNtupleDColumn("PosY_cm");
    analysisManager->CreateNtupleDColumn("PosZ_cm");
    analysisManager->CreateNtupleDColumn("Time_ns");
    analysisManager->CreateNtupleDColumn("KinEnergy_MeV");
    analysisManager->FinishNtuple();
}

void RunAction::BeginOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("ParticleTrajectories"); // Tạo file ParticleTrajectories.root
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}