#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction() : G4UserRunAction(), fRunHitCount(0) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    
    // =========================================================
    // Ntuple 0: Chi tiết tia đập vào máy dò (GIỮ NGUYÊN)
    // =========================================================
    analysisManager->CreateNtuple("ParticleHits", "Details");
    analysisManager->CreateNtupleSColumn("Particle");
    analysisManager->CreateNtupleDColumn("Energy_keV");
    analysisManager->CreateNtupleDColumn("Time_ns");
    analysisManager->FinishNtuple();

    // =========================================================
    // Ntuple 1: Thống kê đếm Poisson (GIỮ NGUYÊN)
    // =========================================================
    analysisManager->CreateNtuple("PoissonStats", "Counts per Run");
    analysisManager->CreateNtupleIColumn("RunID");
    analysisManager->CreateNtupleIColumn("TotalDetected");
    analysisManager->FinishNtuple();

    // =========================================================
    // Ntuple 2: THÊM MỚI - LƯU HẠT ĐỂ CÂN BẰNG PHƯƠNG TRÌNH PHÂN RÃ
    // =========================================================
    analysisManager->CreateNtuple("DecayParticles", "Core Decay Particles");
    analysisManager->CreateNtupleSColumn("Particle");
    analysisManager->CreateNtupleDColumn("Energy_keV");
    analysisManager->CreateNtupleDColumn("Time_ns");
    analysisManager->FinishNtuple();

    analysisManager->OpenFile("RadioactiveData");
}

RunAction::~RunAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}

void RunAction::BeginOfRunAction(const G4Run*) {
    fRunHitCount = 0; 
}

void RunAction::EndOfRunAction(const G4Run* run) {
    auto analysisManager = G4AnalysisManager::Instance();
    
    // Bơm dữ liệu vào Ntuple 1 (ID = 1)
    analysisManager->FillNtupleIColumn(1, 0, run->GetRunID());
    analysisManager->FillNtupleIColumn(1, 1, fRunHitCount);
    analysisManager->AddNtupleRow(1);

    if (run->GetRunID() % 50 == 0) { 
        G4cout << ">>> Completed " << run->GetRunID() << " runs..." << G4endl;
    }
}